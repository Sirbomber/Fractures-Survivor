
/*
 *  Safely emulates the infamous "acid trip" bug originally discovered in Swarm, and further enhances it by making it
 *  affect sprite graphics as well, and repeat when the end of all the graphics data has been overwritten.  Also does
 *  other stupid bullshit.
 */

#include <bitset>
#include <set>
#include <cstdlib>
#include <cstdio>

#include "Tethys/API/GameMap.h"
#include "Tethys/API/TethysGame.h"
#include "Tethys/API/Unit.h"
#include "Tethys/API/Enumerators.h"

#include "Tethys/Game/PathFinder.h"

#include "Tethys/Resource/GFXBitmap.h"
#include "Tethys/Resource/GFXPalette.h"
#include "Tethys/Resource/GFXSurface.h"
#include "Tethys/Resource/SpriteManager.h"

#include "Patcher.h"
#include "Util.h"

using namespace Tethys;
using namespace TethysAPI;
using namespace Patcher;

static constexpr uint32 StartTileset = 1;
static constexpr uint32 NumTilesets  = 13;

static constexpr uint32 PaletteSize = 512;

static constexpr uint32 WaypointArrayLen = 2048;
static constexpr uint32 WaypointsPerChunk = 16;

using PtrPair = std::pair<void*, void*>;

struct GfxState {
  int      prtMaxPaletteIndex;
  size_t   prtPaletteSize;
  void*    pNewPrtGfx;
  void*    pCurPrtPalette;
  void*    pNextPrtPalette;

  void*    pNewTilesetGfx[NumTilesets];
  PtrPair  tilesetPaletteRegions[NumTilesets];

  static constexpr uint32 MaxNumAllocations = 5;

  uint32   curAllocation = (MaxNumAllocations - 1);
  PtrPair  allocations[MaxNumAllocations];
  void*    pNextChunk;
}  static g_gfxState = { };

static std::bitset<MaxMapWidth * MaxMapHeight> g_corruptLocations;
static bool g_alwaysCorrupt = false;
static bool g_randomLights  = false;

static Rgb555 g_curColor    = { };
static Rgb555 g_targetColor = { };

// =====================================================================================================================
void ToggleAlwaysCorrupt() {
  g_alwaysCorrupt = (g_alwaysCorrupt == false);
}

// =====================================================================================================================
void ToggleRandomLights() {
  g_randomLights = (g_randomLights == false);
}

// =====================================================================================================================
void RecordScorpionLocations(
  int aiPlayerId)
{
  static constexpr std::pair<int, int> Offsets[] = {
    {  0,  0 },
    { -1, -1 },  {  0, -1 },  { 1, -1 },  {-1,  0 },
    {  1,  0 },  { -1,  1 },  { 0,  1 },  { 1,  1 },
    { -2,  0 },  {  2,  0 },  { 0,  2 },  { 0, -2 },
  };

  Unit u;
  for (PlayerVehicleEnum vecEnum(aiPlayerId); vecEnum.GetNext(u);) {
    const auto type = u.GetType();
    if (u.IsLive() && ((type == MapID::Scorpion) || (type == MapID::Spider))) {
      const auto loc = u.GetLocation();
      for (const auto [dx, dy] : Offsets) {
        g_corruptLocations.set(((loc.y + dy) * MaxMapWidth) + (loc.x + dx));
      }
    }
  }
}

// =====================================================================================================================
static void CatBytes(
  uint8**      ppWriter,
  const void*  pSrc,
  uint32       count)
{
  memcpy(*ppWriter, pSrc, count);
  *ppWriter += count;
}

// =====================================================================================================================
static void ReallocTilesets(
  size_t  additionalSize = 0,
  bool    movePrt        = true)
{
  static auto**&     ppTilesets = g_mapImpl.ppTilesetBitmaps_;
  static auto*const  pGfxObj    = SpriteManager::GetInstance();
  static auto*const  pPrt       = pGfxObj->pBitmap_;

  static size_t tilesetSize = 0;
  static size_t prtGfxSize  = 0;

  // Calculate allocation sizes if they haven't been already.
  if (tilesetSize == 0) {
    for (uint32 i = StartTileset; i < NumTilesets; ++i) {
      tilesetSize += (ppTilesets[i]->numTiles_ * ppTilesets[i]->bytesPerTile_);
      tilesetSize += (PaletteSize * ppTilesets[i]->numLightLevels_);
    }
  }

  if (movePrt && (prtGfxSize == 0)) {
    for (int i = 0; i < pGfxObj->numImages_; ++i) {
      g_gfxState.prtMaxPaletteIndex = max(g_gfxState.prtMaxPaletteIndex, pGfxObj->imageInfo_[i].paletteIndex);
    }

    prtGfxSize = pPrt->imageBufferSize_;
    g_gfxState.prtPaletteSize = (pGfxObj->paletteSize_ * sizeof(uint16) * (g_gfxState.prtMaxPaletteIndex + 1));
  }

  static constexpr size_t MinWaypointAllocSize = sizeof(PathContext) * WaypointArrayLen;
  size_t minGfxAllocSize = tilesetSize;
  if (movePrt) {
    minGfxAllocSize += prtGfxSize + g_gfxState.prtPaletteSize;
  }
  static const size_t allocSize = Align(additionalSize + max(MinWaypointAllocSize, minGfxAllocSize),
                                        sizeof(PathContext) * WaypointsPerChunk);

  // Allocate the new buffer.
  uint8*const pBuffer = static_cast<uint8*>(malloc(allocSize));

  if (pBuffer != nullptr) {
    g_gfxState.curAllocation = ((g_gfxState.curAllocation + 1) % g_gfxState.MaxNumAllocations);
    free(g_gfxState.allocations[g_gfxState.curAllocation].first);
    g_gfxState.allocations[g_gfxState.curAllocation] = { pBuffer, pBuffer + allocSize };
    g_gfxState.pNextChunk = pBuffer;

    uint8* pWriter = pBuffer;
    if (movePrt) {
      // Copy PRT palettes.  Because OP2 constantly modifies it in-place, we must defer swapping in the corrupted
      // version until the next time this function is called, so it should come first in memory.
      if (g_gfxState.pNextPrtPalette != nullptr) {
        g_gfxState.pCurPrtPalette = g_gfxState.pNextPrtPalette;
        pGfxObj->pPaletteData_ = static_cast<uint8*>(g_gfxState.pCurPrtPalette);
      }

      g_gfxState.pNextPrtPalette = pWriter;
      CatBytes(&pWriter, pGfxObj->pPaletteData_, g_gfxState.prtPaletteSize);
    }

    for (auto i = StartTileset; i < NumTilesets; ++i) {
      // Move tileset graphics.
      void*const pOldTilesetGfx = (g_gfxState.pNewTilesetGfx[i] != nullptr) ? g_gfxState.pNewTilesetGfx[i]
                                                                            : ppTilesets[i]->pPixelData_;
      g_gfxState.pNewTilesetGfx[i] = pWriter;
      CatBytes(&pWriter, pOldTilesetGfx, (ppTilesets[i]->numTiles_ * ppTilesets[i]->bytesPerTile_));

      // Move tileset palettes.
      void*const pOldPalette = (g_gfxState.tilesetPaletteRegions[i].first != nullptr) ?
        g_gfxState.tilesetPaletteRegions[i].first : ppTilesets[i]->pLightLevelPal_;

      g_gfxState.tilesetPaletteRegions[i].first  = pWriter;
      CatBytes(&pWriter, pOldPalette, (PaletteSize * ppTilesets[i]->numLightLevels_));
      g_gfxState.tilesetPaletteRegions[i].second = pWriter;
    }

    if (movePrt) {
      // Move PRT graphics.
      void*const pOldPrtGfx = (g_gfxState.pNewPrtGfx != nullptr) ? g_gfxState.pNewPrtGfx : pPrt->pPixelData_;
      g_gfxState.pNewPrtGfx = pWriter;
      const size_t imageBufferOffset = PtrDelta(pPrt->pImageBuffer_, pPrt->pPixelData_);

      CatBytes(&pWriter, pOldPrtGfx, pPrt->imageBufferSize_);
      pPrt->pPixelData_   = static_cast<uint8*>(g_gfxState.pNewPrtGfx);
      pPrt->pImageBuffer_ = pPrt->pPixelData_ + imageBufferOffset;
    }
  }
}

// =====================================================================================================================
template <size_t N>
static PtrPair* PaletteRangeBinSearch(
  const PtrPair  (&array)[N],
  const void*    pQuery)
{
  const auto CompareInRange = [](const void* pQuery, const void* pItem) -> int {
    const auto*const pRange = static_cast<const PtrPair*>(pItem);

    // Include region just before the palettes too.
    size_t delta = PtrDelta(pRange->second, pRange->first);
    if (PtrDelta(pRange->first, nullptr) < delta) {
      delta = 0;
    }

    return (pQuery >= pRange->second) ? 1 : ((pQuery < PtrDec(pRange->first, delta)) ? -1 : 0);
  };

  return static_cast<PtrPair*>(bsearch(pQuery, &array[0], N, sizeof(PtrPair), CompareInRange));
}

// =====================================================================================================================
static bool __fastcall FreeWaypointList_AddToList(
  PathContextList*  pThis,  int,
  PathContext*      pWaypoint)
{
  // Do this first to ensure it happens deterministically in netplay.
  const bool randResult = (TethysGame::GetRand(5) == 0);

  if (pWaypoint != nullptr) {
    const auto [pBegin, pEnd] = g_gfxState.allocations[g_gfxState.curAllocation];

    if ((pWaypoint >= PtrInc(pBegin, g_gfxState.prtPaletteSize)) && (pWaypoint < pEnd)) {
      if (randResult || PaletteRangeBinSearch(g_gfxState.tilesetPaletteRegions, pWaypoint)) {
        // Add the waypoint to the list to be recycled.
        pWaypoint->pFreeListNext = pThis->pFreeListHead_;
        pThis->pFreeListHead_    = pWaypoint;
      }
    }
  }

  return (pThis != nullptr);
}

// =====================================================================================================================
static PathContext* __fastcall FreeWaypointList_ExtractFromList(
  PathContextList*  pThis)
{
  static auto*const pGfxObj = SpriteManager::GetInstance();

  if (pThis->pFreeListHead_ == nullptr) {
    if ((pThis->pNextAllocAddr_ == nullptr) ||
        (pThis->pNextAllocAddr_ >= g_gfxState.allocations[g_gfxState.curAllocation].second))
    {
      // Out of space.  We must reallocate a new buffer.
      ReallocTilesets();
      pThis->pBaseAllocAddr_ = static_cast<PathContext*>(g_gfxState.allocations[g_gfxState.curAllocation].first);
      pThis->pNextAllocAddr_ = pThis->pBaseAllocAddr_;
    }

    static constexpr size_t AllocSize = (sizeof(PathContext) * WaypointsPerChunk);

    pThis->pFreeListHead_   = pThis->pNextAllocAddr_;
    pThis->pNextAllocAddr_ += WaypointsPerChunk;
    g_gfxState.pNextChunk   = pThis->pNextAllocAddr_;

    // Initialize linked list.
    for (uint32 i = 0; (i < (WaypointsPerChunk - 1)); ++i) {
      pThis->pFreeListHead_[i].pFreeListNext = &pThis->pFreeListHead_[i + 1];
    }
    pThis->pFreeListHead_[WaypointsPerChunk - 1].pFreeListNext = nullptr;

    // Randomize PRT image palettes.
    for (int i = 0; i < pGfxObj->numImages_; ++i) {
      pGfxObj->imageInfo_[i].paletteIndex = (rand() % (g_gfxState.prtMaxPaletteIndex + 1));
    }
  }

  PathContext*const result = pThis->pFreeListHead_;
  pThis->pFreeListHead_        = result->pFreeListNext;
  return result;
}

// =====================================================================================================================
static uintptr_t __cdecl DrawScanlineHook(
  Ebp<Rgb555[256]>  palette,
  Ecx<uint32>       width,
  Edi<Rgb555*>&     pDstOut,
  Esi<uint8*>&      pSrcPaletteIdxOut)
{
  static uint32 iterations = 0;

  Rgb555* pDst           = pDstOut;
  uint8*  pSrcPaletteIdx = pSrcPaletteIdxOut;

  for (uint32 i = width; i > 0; --i, ++pDst, ++pSrcPaletteIdx) {
    if (*pSrcPaletteIdx != 0) {
      // 50% alpha blend.
      const auto& src = palette[*pSrcPaletteIdx];
      auto dst = *pDst;
      dst = {
        static_cast<uint16>((dst.b / 2) + (src.b / 2)),
        static_cast<uint16>((dst.g / 2) + (src.g / 2)),
        static_cast<uint16>((dst.r / 2) + (src.r / 2))
      };
      pDst->u16All = (dst.u16All ^ g_curColor.u16All);
    }
  }

  pSrcPaletteIdxOut = pSrcPaletteIdx;
  pDstOut           = pDst;

  if (true) { // ** TODO
    if (g_curColor.u16All == g_targetColor.u16All) {
      g_targetColor.u16All = (rand() & INT16_MAX);
    }
    if (g_curColor.u16All == 0) {
      g_curColor.u16All    = (rand() & INT16_MAX);
    }

    if (++iterations >= 1000000) {
      iterations = 0;

      if (g_curColor.r > g_targetColor.r) { --g_curColor.r; } else
      if (g_curColor.r < g_targetColor.r) { ++g_curColor.r; } else
      if (g_curColor.g > g_targetColor.g) { --g_curColor.g; } else
      if (g_curColor.g < g_targetColor.g) { ++g_curColor.g; } else
      if (g_curColor.b > g_targetColor.b) { --g_curColor.b; } else
      if (g_curColor.b < g_targetColor.b) { ++g_curColor.b; }
    }
  }

  return 0x5868B0;
}

// =====================================================================================================================
static uintptr_t __cdecl DrawScanlineInverseHook(
  Ebp<Rgb555[256]>  palette,
  Ecx<uint32>       width,
  Edi<Rgb555*>&     pDstOut,
  Esi<uint8*>&      pSrcPaletteIdxOut)
{
  Rgb555* pDst           = pDstOut;
  uint8*  pSrcPaletteIdx = pSrcPaletteIdxOut;

  for (uint32 i = width; i > 0; --i, ++pDst, ++pSrcPaletteIdx) {
    if (*pSrcPaletteIdx != 0) {
      // 50% alpha blend.
      const auto& src = palette[*pSrcPaletteIdx];
      auto dst = *pDst;
      dst = {
        static_cast<uint16>((dst.b / 2) + (src.b / 2)),
        static_cast<uint16>((dst.g / 2) + (src.g / 2)),
        static_cast<uint16>((dst.r / 2) + (src.r / 2))
      };
      pDst->u16All = (dst.u16All ^ (~g_curColor.u16All & INT16_MAX));
    }
  }

  pSrcPaletteIdxOut = pSrcPaletteIdx;
  pDstOut           = pDst;

  return 0x5868B0;
}

// =====================================================================================================================
template <size_t N>
static void RandomizeAllColors(
  PatchContext*    pPatcher,
  const uintptr_t  (&addresses)[N])
{
  for (const auto address : addresses) {
    const uint16 newColor = (*Op2Mem<uint16*>(address) ^ (rand() & INT16_MAX));
    pPatcher->Write(address, newColor);
  }
}

// =====================================================================================================================
template <size_t N>
static void RandomizeAllColorsUniform(
  PatchContext*    pPatcher,
  const uintptr_t  (&addresses)[N])
{
  const uint16 color = (rand() & INT16_MAX);
  for (const auto address : addresses) {
    pPatcher->Write<uint16>(address, *Op2Mem<uint16*>(address) ^ color);
  }
}

// =====================================================================================================================
bool RandomizeWeaponColors(
  bool enable)
{
  static PatchContext patcher;
  bool success = true;

  static constexpr uintptr_t CannonColorAddresses[]    = { 0x4A33BA, 0x4A33BC };
  static constexpr uintptr_t LaserColorAddresses[]     = { 0x4A360A, 0x4A360C };
  static constexpr uintptr_t MicrowaveColorAddresses[] = { 0x4A31EE, 0x4A31F0, 0x4A31F7, 0x4A31F9 };
  static constexpr uintptr_t MeteorDefColorAddresses[] = { 0x44AC83, 0x44AC85, 0x480722, 0x480724 };
  static constexpr uintptr_t ThorsColorAddresses[]     =
    { 0x4E9A30, 0x4E9A32, 0x4E9A34, 0x4E9A36, 0x4E9A38, 0x4E9A3A, 0x4E9A3C, 0x4E9A3E };

  if (enable) {
    RandomizeAllColors(&patcher, ThorsColorAddresses);
    RandomizeAllColorsUniform(&patcher, CannonColorAddresses);
    RandomizeAllColorsUniform(&patcher, LaserColorAddresses);
    RandomizeAllColorsUniform(&patcher, MicrowaveColorAddresses);
    RandomizeAllColorsUniform(&patcher, MeteorDefColorAddresses);
    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) || (success == false)) {
    success &= ((patcher.RevertAll() == Status::Ok));
  }

  return success;
}

// =====================================================================================================================
bool RandomizeUiColors(
  bool enable)
{
  static PatchContext patcher;
  bool success = true;

  static constexpr uintptr_t ListControlColorAddresses[] = { 0x430565, 0x43056A, 0x430613 };
  static constexpr uintptr_t HpBarColorAddresses[]       = { 0x459A12, 0x459A17, 0x459A35, 0x459A3C, 0x459A43 };
  static constexpr uintptr_t SelectBoxColorAddresses[]   = {
    0x43EC01, 0x43EE64, 0x43F193, 0x43ED86, 0x43EDB8, 0x43F1A7, 0x43F1A9, 0x43F28D, 0x43F28F, 0x43F2BB, 0x43F2BD,
    0x43F2DD, 0x43F2DF, 0x43F4F3
  };

  if (enable) {
    RandomizeAllColors(&patcher, ListControlColorAddresses);
    RandomizeAllColors(&patcher, HpBarColorAddresses);
    RandomizeAllColors(&patcher, SelectBoxColorAddresses);
    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) || (success == false)) {
    success &= ((patcher.RevertAll() == Status::Ok));
  }

  return success;
}

// =====================================================================================================================
bool RandomizeBlightColors(
  bool enable)
{
  static PatchContext patcher;
  bool success = true;

  if (enable) {
    for (const uintptr_t address : { 0x5865CC, 0x5866D6, 0x5866D8, 0x5866B5, 0x5866F5, 0x58657B }) {
      patcher.Write<uint16>(address, (~g_curColor.u16All & 0x7FFF));
    }
    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) || (success == false)) {
    success &= ((patcher.RevertAll() == Status::Ok));
  }

  return success;
}

// =====================================================================================================================
bool RandomizeWeaponLut(
  bool enable)
{
  static PatchContext patcher;
  bool success = true;

  if (enable) {
    // Ruins weapon accuracy/AI, and makes Thor's bounce all over the map, damaging random things.
    // Warning: this will desync!
    for (const uintptr_t address : { 0x4E9B30, 0x4E9C30 }) {
      auto*const pLut = Op2Mem<uint8*>(address);

      for (uint32 i = 0; i < 255; ++i) {
        patcher.Write<uint8>(&pLut[i], (pLut[i] ^ TethysGame::GetRand(256)));
      }
    }
    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) || (success == false)) {
    success &= ((patcher.RevertAll() == Status::Ok));
  }

  return success;
}

// =====================================================================================================================
bool SetAcidTripPatch(
  bool enable)
{
  static PatchContext patcher;

  auto*const pGfxObj  = SpriteManager::GetInstance();
  auto*const pPrt     = pGfxObj->pBitmap_;
  auto*const pListObj = PathContextList::GetInstance();

  static TileData curTile = { };
  static uint32   curX    = 0;
  static uint32   curY    = 0;

  // Hook function called prior to drawing a map tile.  Using a lambda allows us to access static variables.
  const auto DrawTileToBackBuffer_Hook = [](
    GFXClippedSurface*  pThis,
    GFXBitmap*          pBitmap,
    int                 pixelX,
    int                 pixelY,
    int                 lightLevel,
    uint8               blightOverlay,
    uint8               darknessOverlay)
  {
    static auto**const ppTilesets = *Op2Mem<GFXTilesetBitmap***>(0x54FC60);  // ** TODO MapImpl.pTilesetMappings_???

    if (curTile.microbe != 0) {
      darknessOverlay = 0;
      lightLevel      = (g_mapImpl.tileWidth_ / 2) + (rand() % (g_mapImpl.tileWidth_ / 4));
    }

    const bool corrupt = (g_alwaysCorrupt        ||
                          (darknessOverlay == 0) ||
                          //(curTile.microbe != 0) ||
                          g_corruptLocations.test((curY * MaxMapWidth) + curX));

    uint32 tilesetIndex      = StartTileset;
    size_t imageBufferOffset = 0;
    uint8* pOldTileset       = nullptr;
    void*  pOldPalette       = nullptr;

    if (corrupt && (g_gfxState.allocations[0].first != nullptr)) {
      for (; tilesetIndex < NumTilesets; ++tilesetIndex) {
        if (ppTilesets[tilesetIndex] == pBitmap) {
          // Swap the tileset graphics with the corrupted versions for this draw.
          imageBufferOffset = PtrDelta(ppTilesets[tilesetIndex]->pImageBuffer_, ppTilesets[tilesetIndex]->pPixelData_);
          pOldTileset       = ppTilesets[tilesetIndex]->pPixelData_;
          pOldPalette       = ppTilesets[tilesetIndex]->pLightLevelPal_;

          ppTilesets[tilesetIndex]->pPixelData_     = static_cast<uint8*>(g_gfxState.pNewTilesetGfx[tilesetIndex]);
          ppTilesets[tilesetIndex]->pImageBuffer_   = (ppTilesets[tilesetIndex]->pPixelData_ + imageBufferOffset);
          ppTilesets[tilesetIndex]->pLightLevelPal_ = g_gfxState.tilesetPaletteRegions[tilesetIndex].first;
          break;
        }
      }
    }

    if ((corrupt == false)                          ||
        (curTile.cellType == CellType::NormalWall)  ||
        (curTile.cellType == CellType::LavaWall)    ||
        (curTile.cellType == CellType::MicrobeWall) ||
        (curTile.cellType == CellType::Rubble))
    {
      pThis->DrawTile(pBitmap, pixelX, pixelY, lightLevel, blightOverlay, darknessOverlay);
    }
    else if ((curTile.cellType == uint32(CellType::Tube0)) && (curTile.wallOrBuilding == 0)) {
      // Don't draw blight over tube tiles so they're still distinguishable.
      pThis->DrawTile(pBitmap, pixelX, pixelY, lightLevel, 0 /*blightOverlay*/, darknessOverlay);
    }
    else {
      const int light = (g_randomLights ? (rand() % g_mapImpl.tileWidth_) : lightLevel);
      pThis->DrawTile(pBitmap, pixelX, pixelY, light, darknessOverlay, (darknessOverlay | blightOverlay));
    }

    if (pOldTileset != nullptr) {
      // Put back the original tileset graphics now that the draw is completed.
      ppTilesets[tilesetIndex]->pPixelData_     = pOldTileset;
      ppTilesets[tilesetIndex]->pImageBuffer_   = (pOldTileset + imageBufferOffset);
      ppTilesets[tilesetIndex]->pLightLevelPal_ = pOldPalette;
    }
  };


  bool success = true;

  if (enable && (patcher.NumPatches() == 0)) {
    static const auto UpdateCurTile = [](TileData* pCurTile, uint32* pStack)
                                        { curTile = *pCurTile;  curX = pStack[5];  curY = pStack[6]; };

    // In Viewport.DrawBackground()
    patcher.LowLevelHook(0x4701A8, [](Edx<TileData*> pTile, Esp<uint32*> pEsp) { UpdateCurTile(pTile, pEsp); });
    patcher.LowLevelHook(0x470486, [](Esi<TileData*> pTile, Esp<uint32*> pEsp) { UpdateCurTile(pTile, pEsp); });
    patcher.HookCall(0x47036B, ThiscallLambdaPtr(DrawTileToBackBuffer_Hook));
    patcher.HookCall(0x470639, ThiscallLambdaPtr(DrawTileToBackBuffer_Hook));

    static void (__thiscall*  pfnOldMinimapSetColor)(void* pThis, Location at, uint16 color) = nullptr;
    patcher.Hook(0x48D2B0,
                 ThiscallLambdaPtr([](void* pThis, Location at, uint16 color)
                                     { pfnOldMinimapSetColor(pThis, at, (color ^ (rand() & INT16_MAX))); }),
                 &pfnOldMinimapSetColor);

    // Hook acid cloud drawing code.
    patcher.LowLevelHook(*Op2Mem<void**>(0x586521), &DrawScanlineHook);

    // Hook the waypoint list manager push/pop functions to point to our GFX memory we're about to create.
    patcher.Hook(0x446BC0, &FreeWaypointList_AddToList);
    patcher.Hook(0x446B60, &FreeWaypointList_ExtractFromList);

    // Save the state of the GFX manager pointers we're going to be overwriting.
    patcher.Touch(&pGfxObj->pPaletteData_);
    patcher.Touch(&pGfxObj->imageInfo_[0], (sizeof(pGfxObj->imageInfo_[0]) * pGfxObj->numImages_));
    patcher.Touch(&pPrt->pPixelData_);

    // Move tilesets, etc. and overwrite pointers.
    if (g_gfxState.allocations[0].first == nullptr) {
      ReallocTilesets();
    }
    else {
      const size_t imageBufferOffset = PtrDelta(pPrt->pImageBuffer_, pPrt->pPixelData_);
      pPrt->pPixelData_   = static_cast<uint8*>(g_gfxState.pNewPrtGfx);
      pPrt->pImageBuffer_ = pPrt->pPixelData_ + imageBufferOffset;

      if (g_gfxState.pCurPrtPalette != nullptr) {
        // ** TODO This causes a crash?
        //pGfxObj->pPaletteData_ = static_cast<uint8*>(g_gfxState.pCurPrtPalette);
      }
    }

    patcher.Write(&pListObj->pBaseAllocAddr_,
                  static_cast<PathContext*>(g_gfxState.allocations[g_gfxState.curAllocation].first));
    patcher.Write(&pListObj->pNextAllocAddr_,    g_gfxState.pNextChunk);
    patcher.Write(&pListObj->pFreeListHead_, nullptr);

    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) || (success == false)) {
    const size_t imageBufferOffset = PtrDelta(pPrt->pImageBuffer_, pPrt->pPixelData_);
    success &= ((patcher.RevertAll() == Status::Ok));
    if (success) {
      pPrt->pImageBuffer_ = pPrt->pPixelData_ + imageBufferOffset;
    }
  }

  return success;
}

// =====================================================================================================================
bool SetMoreAcid(
  bool enable)
{
  static PatchContext patcher;
  bool success = true;

  if (enable && (patcher.NumPatches() == 0)) {
    // Using a std::set just in case the same function is reused in the function table multiple times.
    static std::set<void*> pFuncs = { *Op2Mem<void**>(0x586398), *Op2Mem<void**>(0x58639C), *Op2Mem<void**>(0x586605) };

    for (void*const pFunc : pFuncs) {
      patcher.LowLevelHook(pFunc, &DrawScanlineInverseHook);
    }

    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) || (success == false)) {
    success &= ((patcher.RevertAll() == Status::Ok));
  }

  return success;
}

// =====================================================================================================================
bool CleanupAcidTrip() {
  const bool success = SetAcidTripPatch(false) && SetMoreAcid(false);

  if (success) {
    for (const auto [pBegin, pEnd] : g_gfxState.allocations) {
      free(pBegin);
    }

    memset(&g_gfxState.allocations[0], 0, sizeof(g_gfxState.allocations));
    g_gfxState.curAllocation = 0;
  }

  return success;
}
