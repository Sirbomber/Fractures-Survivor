
#include <utility>
#include <bitset>

#include "Tethys/API/Unit.h"
#include "Tethys/API/GameMap.h"
#include "Tethys/API/TethysGame.h"
#include "Tethys/API/Enumerators.h"
#include "Tethys/API/Mission.h"

#include "Tethys/Game/BlightLavaManager.h"
#include "Tethys/Game/UnitGroup.h"

#include "Tethys/Resource/SoundID.h"
#include "Tethys/Resource/GFXPalette.h"

#include "Tethys/UI/GameFrame.h"

#include "Patcher.h"
#include "Util.h"

using namespace Tethys;
using namespace Tethys::TethysAPI;
using namespace Patcher;

static constexpr uint32  VehicleInfectedFlag = (1u << 30);                // Unit flag used to mark infected vehicles.
static constexpr MapRect PoofRect            = { { 1, 10 }, { 4, 28 } };  // Poof rect for non-combat infected vehicles.
static constexpr Rgb555  BlightColor         = { 0x1B, 0x00, 0x0C };      // Blight minimap color.
static constexpr int     MicrobeArmorTechID  = 6202;

static std::bitset<MaxMapWidth * MaxMapHeight> g_infectedTiles;  // Replaces tile.expand bit.
static bool g_blightVisible = true;


// =====================================================================================================================
void SetBlightVisible(
  bool visible)
{
  g_blightVisible = visible;
  if (visible) {
    BlightManager::GetInstance()->DrawOnMiniMap();
  }
}

// =====================================================================================================================
static bool IsTileInfected(
  int  x,
  int  y)
{
  return g_infectedTiles.test((y * g_mapImpl.tileWidth_) + x);
}

// =====================================================================================================================
static bool IsTileInfected(
  Location  location)
{
  return IsTileInfected(location.x, location.y);
}

// =====================================================================================================================
static void SetTileInfected(
  int   x,
  int   y,
  bool  state)
{
  g_infectedTiles.set((y * g_mapImpl.tileWidth_) + x, state);
}

// =====================================================================================================================
static void SetTileInfected(
  Location  location,
  bool      state)
{
  SetTileInfected(location.x, location.y, state);
}

// =====================================================================================================================
static Location MakeWrappedLocation(
  int  x,
  int  y)
{
  return { static_cast<int>(x & g_mapImpl.tileXMask_), static_cast<int>(y & (g_mapImpl.tileHeight_ - 1)) };
}

// =====================================================================================================================
static uint16& VehicleInfectionTimer(
  MapObject* pMo)
{
  return static_cast<uint16*>(PtrInc(pMo, MapObjectSize))[-1];
}

// =====================================================================================================================
// ** TODO This isn't functionally equivalent to builtin or old hack, spreads in odd diagonal patterns?
static void __fastcall Blight_SpreadTo_Hook(
  BlightManager*  pThis,  int,
  Location        at)
{
  static constexpr std::pair<int8, int8> SpreadOffsetTable[] = {
    { INT8_MIN, INT8_MIN },  { INT8_MIN, INT8_MIN },
          { 1, 0 },                { 0, 1 },
          { 0, 0 },                { 1, 1 },
          { 0, 1 },                { 1, 1 },
          { 0, 0 },                { 1, 1 },
          { 1, 0 },                { 1, 1 },
          { 0, 0 },                { 1, 1 },
          { 1, 1 },                { 1, 1 },
          { 0, 1 },                { 1, 0 },
          { 1, 0 },                { 0, 1 },
          { 0, 0 },                { 0, 1 },
          { 0, 1 },                { 0, 1 },
          { 0, 0 },                { 1, 0 },
          { 1, 0 },                { 1, 0 },
    { INT8_MIN, INT8_MIN },  { INT8_MIN, INT8_MIN },
  };

  const Location wrapped = MakeWrappedLocation(at.x, at.y);
  if (IsTileInfected(wrapped)) {
    uint32 microbeBitmask = 1;
    for (int y = wrapped.y, i = 0; y <= wrapped.y + 1; ++y) {
      for (int x = wrapped.x; x <= wrapped.x + 1; ++x) {
        if (GameMap::GetMicrobe(MakeWrappedLocation(x, y))) {
          microbeBitmask <<= 1;
        }
      }
    }
    microbeBitmask -= 1;

    if (microbeBitmask == 0) {
      SetTileInfected(wrapped, false);
    }
    else if (microbeBitmask != 15) {
      pThis->field_08 ^= 1;
#if SWARMZ_BUILD
      const int mode =  pThis->field_08 & 1;
#else
      const int mode = (pThis->field_08 & 2) ? 1 : 0; // ** TODO Bug? _08 is always either 0 or 1, so this just forces it to 0?
#endif

      const auto     [dx, dy]    = SpreadOffsetTable[(microbeBitmask * 2) + mode];
      const Location spreadingTo = MakeWrappedLocation(wrapped.x + dx, wrapped.y + dy);

      if ((dx != INT8_MIN) && (spreadingTo.y > 0) && (spreadingTo.y < g_mapImpl.tileHeight_)) {
        const auto blightSpeed = MapImpl::GetCellTypeInfo(GameMap::GetCellType(spreadingTo))->blightSpeed;
        const auto lightLevel  = g_mapImpl.GetLightLevel(spreadingTo);

        bool hasMicrobeWall = false;

        if ((blightSpeed - lightLevel) > (TethysGame::Tick() % 40)) {
          // Damage microbe walls.
          for (int y = spreadingTo.y; y >= (spreadingTo.y - 1); --y) {
            for (int x = spreadingTo.x; x >= (spreadingTo.x + 1); ++x) {
              const Location curLocation = MakeWrappedLocation(x, y);
              if (GameMap::GetCellType(curLocation) == CellType::MicrobeWall) {
                GameMap::DamageWall(curLocation, 15);
                hasMicrobeWall = true;
                break;
              }
            }
          }

          // Don't spread onto tiles with lava or microbe walls.
          if ((hasMicrobeWall == false) && (GameMap::GetLavaPresent(spreadingTo) == false)) {
            pThis->Spread(spreadingTo);

            if (g_blightVisible) {
              if ((g_mapImpl.paddingOffsetTileX_ == 0) ||
                  ((g_mapImpl.paddingOffsetTileX_ < spreadingTo.x) &&
                   ((g_mapImpl.paddingOffsetTileX_ + (g_mapImpl.tileWidth_ / 2)) >= spreadingTo.x)))
              {
                g_miniMap.SetColor(spreadingTo, BlightColor);
              }

              // Mark the detail pane for redraw.
              auto*const pDetailPaneViewport = g_gameFrame.GetDetailPaneViewport();

              RECT rect  = { };
              rect.right = ((spreadingTo.x * 32) & (g_mapImpl.pixelWidthMask_ - 1)) -
                           (g_mapImpl.pixelWidthMask_ & pDetailPaneViewport->pixelX_) + 40;

              const auto halfWidthMask = static_cast<int>(g_mapImpl.pixelWidthMask_ / 2);
              if (-halfWidthMask > rect.right) {
                rect.right &= g_mapImpl.pixelWidthMask_;
              }
              if (rect.right > halfWidthMask) {
                rect.right -= g_mapImpl.pixelWidthMask_;
              }

              rect.bottom = (spreadingTo.y * 32) - pDetailPaneViewport->pixelY_ + 40;
              rect.left   = rect.right  - 48;
              rect.top    = rect.bottom - 48;

              pDetailPaneViewport->MarkForRedraw(rect);
            }
          }
        }
      }
    }
  }
}

#if !SWARMZ_BUILD
// =====================================================================================================================
static void Blight_DoInfect_Hook(
  Unit&  unit)
{
  MapObject*const pMapObj = unit.GetMapObject();

  if (unit.IsLive()) {
    if (unit.IsBuilding() && ((pMapObj->flags_ & MoFlagBldInfected) == 0)) {
      TethysGame::AddMapSound(SoundID::Microbe, unit.GetLocation());

      pMapObj->flags_ |= MoFlagBldInfected;
      static_cast<Building*>(pMapObj)->KillOccupants();
      unit.AddDamage(unit.GetMaxHitpoints() / 3);

      if (unit.IsUnderConstruction()) {
        // Destroy buildings that aren't fully constructed.
        pMapObj->flags_ |= MoFlagDead;
        unit.DoDeath();
        pMapObj->flags_ |= MoFlagMarkedForRedraw;
      }
      else {
        pMapObj->action_  = {};
        pMapObj->command_ = 0;

        // Deselect the building if the player has it selected.
        auto*const pSelectedGroup = UnitGroup::GetSelectedUnitGroup();
        if ((pSelectedGroup->GetNumUnits() == 1) && (pSelectedGroup->GetUnitIDs()[0] == unit.id_)) {
          pSelectedGroup->Clear();

		  // Give VFs to Gaia
		  if (unit.GetOwner() != 6)
		  {
			  if (unit.GetType() == MapID::VehicleFactory)
			  {
				  pMapObj->TransferUnit(6);
			  }
		  }
        }
      }
    }
    else if (unit.IsVehicle() && ((pMapObj->flags_ & VehicleInfectedFlag) == 0)) {
      // Start the infection timer for vehicles that enter blight.
      pMapObj->flags_ |= VehicleInfectedFlag;
      VehicleInfectionTimer(pMapObj) = 0;
    }
  }
}
#else
// =====================================================================================================================
// SwarmZ version
static void Blight_DoInfect_Hook(
  Unit&  unit)
{
  MapObject*const pMapObj = unit.GetMapObject();

  if (unit.IsLive()) {
    if (unit.IsBuilding() && ((pMapObj->flags_ & MoFlagESGed) == 0)) {
      TethysGame::AddMapSound(SoundID::Microbe, unit.Location());
      pMapObj->flags_ |= MoFlagESGed;
    }
    else if ((unit.GetType() == MapID::Scorpion) && ((pMapObj->flags_ & VehicleInfectedFlag) == 0)) {
      // Start the infection timer for scorpions that enter blight.
      pMapObj->flags_ |= VehicleInfectedFlag;
      VehicleInfectionTimer(pMapObj) = 0;
    }
  }
}
#endif

// =====================================================================================================================
static void __fastcall Blight_InfectAreaNear_Hook(
  BlightManager*  pThis,  int,
  Location        at)
{
  const MapRect rect((at.x - 1), max((at.y - 1), 0), at.x, min(at.y, (g_mapImpl.tileHeight_ - 1)));

  for (int x = rect.x1; x <= rect.x2; ++x) {
    for (int y = rect.y1; y <= rect.y2; ++y) {
      const Location curLocation = MakeWrappedLocation(x, y);
      Unit unit = GameMap::GetUnitOnTile(curLocation);

      bool foundBlight = false;

      for (int dy = 0; dy <= 1; ++dy) {
        for (int dx = 0; dx <= 1; ++dx) {
          if (GameMap::GetMicrobe(MakeWrappedLocation(x + dx, y + dy))) {
            if (unit.IsLive()) {
              Blight_DoInfect_Hook(unit);
            }

            foundBlight = true;
            break;
          }
        }
      }

      SetTileInfected(curLocation, foundBlight);
    }
  }
}

// =====================================================================================================================
static void __fastcall Blight_ShowMinimapCoverage_Hook(
  BlightManager*  pThis)
{
  if (g_blightVisible) {
    for (int y = 0; y < GameMap::GetHeight(); ++y) {
      for (int x = 0; x < GameMap::GetWidth(); ++x) {
        const Location curLocation(x, y);
        if (IsTileInfected(x, y) && (GameMap::GetLavaPresent(curLocation) == false)) {
          g_miniMap.SetColor(curLocation, BlightColor);
        }
      }
    }
  }
}

// =====================================================================================================================
void BlightInfectionProc(
  int playerId)
{
  auto*const pMap = GameMap::GetImpl();

  static uint32 tickOfLastMsg     = 0;
  static uint32 tickOfLastCall[7] = { };

  const uint32 tick = static_cast<uint32>(TethysGame::Tick());

  Unit u;

#if !SWARMZ_BUILD
  // Decay infected structures.
  for (PlayerBuildingEnum bldgEnum(playerId, MapID::None); bldgEnum.GetNext(u);) {
    if (u.IsInfected()) {
      u.AddDamage(TethysGame::GetRand(5) + 5);
      if ((u.GetDamage() < 0) || u.IsUnderConstruction()) {
        u.DoDeath();
      }
    }
  }
#endif

  // Process vehicles.
  for (PlayerVehicleEnum vecEnum(playerId); vecEnum.GetNext(u);) {
    if (u.IsLive()) {
      MapObject*const pMapObj  = u.GetMapObject();
      const auto      location = u.GetLocation();

      if ((pMapObj->flags_ & VehicleInfectedFlag) == 0) {
        if ((u.GetAction() == ActionType::Move) || (u.GetAction() == ActionType::WeaponMove)) {
          // Begin infecting vehicles that enter Blight.
          if (IsTileInfected(location)) {
            Blight_DoInfect_Hook(u);
          }
        }
      }
      else {
        auto&      timer           = VehicleInfectionTimer(pMapObj);
        const bool hasMicrobeArmor = Player[playerId].HasTechnology(MicrobeArmorTechID);

        if (IsTileInfected(location)) {
          if ((tick - tickOfLastCall[playerId]) > 0) {
            // Tick down the unit's infection infection timer.
            auto*const pCellTypeInfo = MapImpl::GetCellTypeInfo(GameMap::GetCellType(location));
            const int  increment     =
              ((tick - tickOfLastCall[playerId]) * (pCellTypeInfo->blightSpeed - g_mapImpl.GetLightLevel(location)));

            if ((timer + increment) > UINT16_MAX) {
              timer = UINT16_MAX;
            }
            else if (increment > 0) {
              timer += increment;
            }
          }

          if (timer >= (hasMicrobeArmor ? 60000 : 10000)) {
            // Complete the infection by transferring the unit to gaia.
            timer = 0;
            pMapObj->TransferUnit(6);
            TethysGame::AddMapSound(SoundID::Microbe, location);

            if ((TethysGame::LocalPlayer() == playerId) && (((tick - tickOfLastMsg) > 256) || (tickOfLastMsg > tick))) {
              char nameBuf[36] = "";
              char msgBuf[36]  = "";

              pMapObj->GetSelectionStr(nameBuf, sizeof(nameBuf));
              snprintf(msgBuf, sizeof(msgBuf), "%s infected", nameBuf);
              TethysGame::AddMessage(msgBuf, SoundID::Savnt211, playerId, u);
              tickOfLastMsg = tick;
            }
          }
        }
        else if (hasMicrobeArmor && (timer < 20000)) {
          // If the player has microbe armor and is less than 33% infected, allow vehicle to "heal" on uninfected tiles.
          if (const int increment = (tick - tickOfLastCall[playerId]);  increment > 0) {
            timer = (increment > int(timer)) ? 0 : (timer - increment);
          }
        }
        else if (timer > 500) {
          // Self-destruct partially-infected vehicles that make it on to uninfected tiles.  Allow a grace period so
          // units intentionally travelling into the Blight that pass over an uninfected tile don't self-destruct.
          u.DoSelfDestruct();
        }
      }
    }
  }

  tickOfLastCall[playerId] = tick;
}

// =====================================================================================================================
// Function to handle infected vehicles belonging to Gaia, as AI functions do not fully work with it
// ** TODO See if FightGroups can be fixed for Gaia so this can be thrown out
MISSION_API void FindInfectedTankTargets() {
  static uint32 testDistance = GameMap::GetWidth() * GameMap::GetHeight();
  static uint32 tickOfLastCall = 0;
  static uint32 tickOfLastTargetReadjustment = 0;

  const bool refresh = ((static_cast<uint32>(TethysGame::Tick()) - tickOfLastTargetReadjustment) >= 120);
  if (refresh) {
    tickOfLastTargetReadjustment = static_cast<uint32>(TethysGame::Tick());
  }

  Unit curUnit;
  for (PlayerVehicleEnum microbeVecEnum(6); microbeVecEnum.GetNext(curUnit);) {
    if ((curUnit.IsLive() == false) || (curUnit.IsVehicle() == false)) {
      continue;
    }

    MapObject*const pMapObj = curUnit.GetMapObject();

    if (curUnit.HasFlag(VehicleInfectedFlag) == false) {
      // Non-infected vehicle
      continue;
    }

#if !SWARMZ_BUILD
    // Vehicles that were created by an AI player (or a dropped player) don't like being controlled; also, if the
    // creator player has different upgrades than what Gaia has, esp. weapon range upgrades, the AI can get confused.
    if (curUnit.GetCreator() != 6) {
      pMapObj->creatorNum_ = 6;
    }
#endif

    if ((static_cast<uint32>(TethysGame::Tick()) - tickOfLastCall) > 0) {
      VehicleInfectionTimer(pMapObj) += (static_cast<uint32>(TethysGame::Tick()) - tickOfLastCall);
    }

    if (VehicleInfectionTimer(pMapObj) >= (250 * 100)) {
      // Vehicle has been attempting to complete its current action for too long, destroy it
      curUnit.DoSelfDestruct();
      continue;
    }

    if (((curUnit.GetCommand() == CommandType::Attack) && (refresh == false)) ||
        (curUnit.HasFlag(MoFlagOffensive) == false))
    {
      continue;
    }

    if (curUnit.IsBusy() == false) {
      // Vehicle completed its action, reset the self-destruct timer
      VehicleInfectionTimer(pMapObj) = 0;
    }

    Unit curTarget;
    for (ClosestEnumerator closestUnitEnum(curUnit.GetLocation()); closestUnitEnum.GetNext(curTarget, testDistance);) {
      if ((curTarget.IsLive() == false) || ((curTarget.IsBuilding() || curTarget.IsVehicle()) == false)) {
        continue;
      }

      if (curTarget.IsBuilding() && (curTarget.HasFlag(MoFlagCanBeDamaged) == 0)) {
        continue;
      }

      if (Player[curUnit.GetOwner()].IsAlliedTo(curTarget.GetOwner()) == false) {
        curUnit.DoAttack(Location(curTarget.id_, -1));
        break;
      }
    }
  }

  tickOfLastCall = static_cast<uint32>(TethysGame::Tick());
}

// =====================================================================================================================
bool SetBlightPatch(
  bool enable)
{
  static PatchContext patcher;

  static bool inited = false;
  static FightGroup microbePoofGroup;

  bool success = true;

  if (enable && (patcher.NumPatches() == 0)) {
    auto*const pBlightMgrVtbl = BlightManager::Vtbl();

    // Hide the Blight overlay if g_blightVisible == false.
    patcher.LowLevelHook(0x4700E0, [](Ecx<uint32>& mask) { mask &= g_blightVisible ? ~0 : 0;  return 0x4700E6; });

    // Even though this is a virtual function, it's usually called in a non-virtual way.
    patcher.Hook(pBlightMgrVtbl->pfnDrawOnMiniMap,   &Blight_ShowMinimapCoverage_Hook);

    patcher.Write(&pBlightMgrVtbl->pfnDrawOnMiniMap, &Blight_ShowMinimapCoverage_Hook);
#if 0
    patcher.Write(&pBlightMgrVtbl->pfnTrySpread,     &Blight_SpreadTo_Hook);
#else
    // In BlightManager::TrySpread()
    patcher.LowLevelHook(0x49DD9C, [](Edx<int> x, Ebx<int> y) { return IsTileInfected(x, y) ? 0x49DDA8 : 0x49E0F5; });
    patcher.LowLevelHook(0x49DE41, [](Esp<void*> pEsp) {
      const Location where = MakeWrappedLocation(*(int*)(PtrInc(pEsp, 0x28)), *(int*)(PtrInc(pEsp, 0x2C)));
      SetTileInfected(where, false);
      return 0x49E0F5;
    });
    patcher.LowLevelHook(0x49E058, [] { return g_blightVisible ? 0 : 0x49E069; });
#endif
    patcher.Write(&pBlightMgrVtbl->pfnToggleOnTile,  &Blight_InfectAreaNear_Hook);

    // Hook LevelDll.RaiseUnitTransferredEvent to 1) transfer infected non-combat vehicles to the poof group, and
    // 2) prevent the game from crashing when transferring to gaia.
    patcher.LowLevelHook(0x4031C6, [](Eax<int> dstPlayerID, Esi<MapObject*> pUnit) {
      if (inited == false) {
        // Unally gaia with everyone so infected vehicles are auto-targeted by players.
        for (int i = 0; i < TethysGame::NumPlayers(); ++i) {
          Player[i].AllyWith(6);
          Player[6].AllyWith(i);
        }

        // Set up the ScGroup non-combat infected units will get transferred to.
        microbePoofGroup = CreateFightGroup(6);
        microbePoofGroup.DoExitMap();
        microbePoofGroup.SetRect({ GameMap::At(PoofRect.x1, PoofRect.y1), GameMap::At(PoofRect.x2, PoofRect.y2) });

        inited = true;
      }

      if ((dstPlayerID == 6) && ((pUnit->flags_ & MoFlagOffensive) == 0)) {
        microbePoofGroup.TakeUnit(Unit(pUnit));
      }
      return (dstPlayerID == 6) ? 0x4031F2 : 0;
    });

    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) && (patcher.NumPatches() != 0)) {
    g_infectedTiles.reset();
  }

  if ((enable == false) || (success == false)) {
    success &= ((patcher.RevertAll() == Status::Ok));
  }

  return success;
}
