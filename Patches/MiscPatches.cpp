
#include "Tethys/API/Unit.h"
#include "Tethys/API/GameMap.h"
#include "Tethys/API/TethysGame.h"
#include "Tethys/Resource/CConfig.h"

#include "Cutscene.h"

#include <utility>
#include <cstring>

#include "Patcher.h"
#include "Util.h"

using namespace Tethys;
using namespace TethysAPI;
using namespace Patcher;

template <size_t... Is>  auto& GetMapObjVtbls(std::index_sequence<Is...>)
  { static MapObject::VtblType*const Vtbls[] = { MapObjFor<MapID(Is)>::Vtbl()... };  return Vtbls; }
auto& GetMapObjVtbls() { return GetMapObjVtbls(std::make_index_sequence<MapID::MaxObject>{}); }

// =====================================================================================================================
bool SetSuperSpeedPatch(
  bool enable)
{
  static PatchContext patcher;
  bool success = true;

  if (enable && (patcher.NumPatches() == 0)) {
    patcher.Write(&g_gameFrame.iniSettings_.frameSkip, 2);
    success = (patcher.Write<uint8>(0x49C374, 0xEB) == Status::Ok); // 0x73
  }

  if ((enable == false) || (success == false)) {
    success &= ((patcher.RevertAll() == Status::Ok));
  }

  return success;
}

// =====================================================================================================================
bool SetStructureDamagePatches(
  bool enable)
{
  static PatchContext patcher;
  bool success = true;

  if (enable && (patcher.NumPatches() == 0)) {
    for (int type = MapID::CommonOreMine; type <= MapID::Tokamak; ++type) {
      auto*const pVtbl = GetMapObjVtbls()[type];
      if (type == MapID::GuardPost) {
        patcher.Write(
          &pVtbl->pfnCheckSpontaneouslyExplode,
          ThiscallLambdaPtr([](Building* pThis) {
            return ((pThis->flags_ & MoFlagBldNoExplosionOnDeath) ||
              (pThis->damage_ < ((pThis->GetType()->playerStats_[pThis->creatorNum_].hp*2)/3))) ?
              false : Building::Vtbl()->pfnCheckSpontaneouslyExplode(pThis);
          }));
      }
      else if (reinterpret_cast<const void*>(pVtbl->pfnCheckSpontaneouslyExplode) == Op2Mem(0x409400)) {
        patcher.Write(
          &pVtbl->pfnCheckSpontaneouslyExplode,
          ThiscallLambdaPtr([](Building* pThis) {
            return ((pThis->flags_ & MoFlagBldNoExplosionOnDeath) == 0) &&
                   Building::Vtbl()->pfnCheckSpontaneouslyExplode(pThis);
          }));
      }

#if 0
      // This is in 1.4.0
      if (reinterpret_cast<const void*>(pVtbl->pfnDoEvent) == Op2Mem(0x482D90)) {
        patcher.Write(&pVtbl->pfnDoEvent, ThiscallLambdaPtr([](Building* pThis) {
          Location dockLocation;

          // ** TODO Investigate possible crashes related to this?
          if ((pThis != nullptr) && (pThis->GetDockLocation(&dockLocation) != 0)) {
            Unit unitOnDock = GameMap::GetUnitOnTile(dockLocation);

            if (unitOnDock.IsVehicle() && (Player[pThis->ownerNum_].IsAlliedTo(unitOnDock.GetOwner()) == false)) {
              Building::Vtbl()->pfnDoEvent(pThis);
            }
          }
        }));
      }
#endif
    }

    // Prevent Guard Posts from being disabled due to damage.
    patcher.LowLevelHook(0x483732, [](Eax<MapObjectType*> pMoType)
      { return (pMoType->type_ == MapID::GuardPost) ? 0x483744: 0; });

    // Hide Gaia-owned structure status icons.
    // ** TODO is there a better place for this?
    patcher.LowLevelHook(0x4091E8, [](Edx<MapObject*> pThis) { return (pThis->ownerNum_ == 6) ? 0x4092E8 : 0; });

    // Always show Command Center idle button.
    // ** TODO is there a better place for this?
    patcher.Write<uint8>(0x455D7D, 0x00);

    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) || (success == false)) {
    success &= ((patcher.RevertAll() == Status::Ok));
  }

  return success;
}

// =====================================================================================================================
bool SetAdvancedLabMouseoverPatch(
  bool enable)
{
  static PatchContext patcher;
  bool success = true;

  if (enable) {
    static auto*const pfnOldGetRolloverText = MapObj::AdvancedLab::Vtbl()->pfnGetMouseOverStr;
    patcher.Write(
      &MapObj::AdvancedLab::Vtbl()->pfnGetMouseOverStr, ThiscallLambdaPtr([](MapObject* pThis, char* pDst, int size) {
        return (pThis->ownerNum_ == 6) ? snprintf(pDst, size, "TAC Hot Lab") : pfnOldGetRolloverText(pThis, pDst, size);
      }));

    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) || (success == false)) {
    success &= ((patcher.RevertAll() == Status::Ok));
  }

  return success;
}

// =====================================================================================================================
bool SetStarshipCostPatch(
  int numColonistsRequired,
  int numFoodRequired)
{
  static PatchContext patcher;
  const bool enable = (numColonistsRequired >= 0) && (numFoodRequired >= 0);
  bool success      = true;

  if (enable) {
    if (auto value = static_cast<uint16>(numColonistsRequired);  value > 0) {
      // In Player.ProcessCommandPacket(), UICommand::MouseCommand::Launch.IsEnabled(), Unit.Launch()
      for (uintptr loc : { 0x40F7EA, 0x4574CB, 0x47FF8D }) {
        patcher.Write(loc, value);
      }
    }

    if (numFoodRequired > 0) {
      // Food module cost
      // In Player::ProcessCommandPacket() (x2), BuildListReport::CanBuildItem(), BuildListReport::GetStatusString()
      for (uintptr loc : { 0x40EA34, 0x40EA3C, 0x4686B4, 0x4688D4 }) {
        patcher.Write<Patcher::uint32>(loc, numFoodRequired);
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
bool SetMissionEndHook(
  bool            enable,
  void (__cdecl*  pfnHook)())
{
  static PatchContext patcher;
  bool success = true;

  if (enable && (patcher.NumPatches() == 0)) {
    // Disable automatic victory on being the last remaining player in MP
    patcher.Write<uint8>(0x4033A9, 0x00);

    if (pfnHook != nullptr) {
      // Insert the user-specified hook during mission end, primarily intended for playing a cutscene.
      patcher.LowLevelHook(0x49CB44, pfnHook);
    }

    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) || (success == false)) {
    success &= ((patcher.RevertAll() == Status::Ok));
  }

  return success;
}

// =====================================================================================================================
bool SetMissionEndHook(
  bool         enable,
  const char*  pWinCutscene,
  const char*  pLoseCutscene)
{
  void (__cdecl*  pfnHook)() = nullptr;

  enable &= (pWinCutscene != nullptr) && (pLoseCutscene != nullptr);
  if (enable) {
    static char pWinCutscenePath[MAX_PATH]  = "";
    static char pLoseCutscenePath[MAX_PATH] = "";

    strncpy_s(&pWinCutscenePath[0],  sizeof(pWinCutscenePath),  pWinCutscene,  _TRUNCATE);
    strncpy_s(&pLoseCutscenePath[0], sizeof(pLoseCutscenePath), pLoseCutscene, _TRUNCATE);

    pfnHook = [] {
      if (g_configFile.GetInt("Game", "ShowMovies", 1) != 0) {
        switch (g_gameImpl.gameTermReasons_) {
        case GameTermReasons::MissionAccomplished:
          g_gameImpl.gameTermReasons_ = GameTermReasons::AutoDemo;  // Prevent the game from playing another cutscene
        case GameTermReasons::Victory:
          PlayAvi(pWinCutscenePath);
          break;

        case GameTermReasons::MissionFailed:
          g_gameImpl.gameTermReasons_ = GameTermReasons::AutoDemo;  // Prevent the game from playing another cutscene
        case GameTermReasons::Defeat:
          PlayAvi(pLoseCutscenePath);
          break;

        default:
          break;
        }
      }
    };
  }

  return SetMissionEndHook(enable, pfnHook);
}

// =====================================================================================================================
// Silences disaster caution/alert/warning messages.
bool SetInstantDisasterNoWarnPatch(
  bool enable)
{
  static PatchContext patcher;
  bool success = true;

  if (enable && (patcher.NumPatches() == 0)) {
    Disaster::VtblType*const pVtbls[] = {
      MapObj::Lightning::Vtbl(), MapObj::Vortex::Vtbl(), MapObj::Earthquake::Vtbl(), MapObj::Eruption::Vtbl(),
      MapObj::Meteor::Vtbl()
    };
    for (auto* pVtbl : pVtbls) {
      patcher.Write(
        &pVtbl->pfnWarn,
        ThiscallLambdaPtr([](Disaster* pThis, int warn1StrIdx, int warn1SoundId, int warn2StrIdx, int warn2SoundId) {
          pThis->flags_ |= MoFlagEntDisasterDidFirstWarn | MoFlagEntDisasterDidSecondWarn;
          return ((TethysGame::GetRand(100 + 1) >= 90) || (pThis->flags_ & MoFlagEntDisasterStarted)) ? 1 : 0;
        }));
    }

    // Lightning, vortex, earthquake, eruption, meteor, missile launch, incoming missile
    for (const uintptr_t loc : { 0x433107, 0x48F6B3, 0x41397B, 0x4A82FF, 0x44A3E3, 0x43B2B0, 0x480514 }) {
      patcher.HookCall(
        loc,
        ThiscallLambdaPtr([](void* pThis, int pixelX, int pixelY, const char* pText, int soundIndex) { return 1; }));
    }

    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) || (success == false)) {
    success &= ((patcher.RevertAll() == Status::Ok));
  }

  return success;
}

// =====================================================================================================================
// Forces world map X-axis wraparound on any map.  Because the mission DLL is loaded after the map, you will need to
// call MapImpl::LoadMap() in InitProc() to reload the map, which must be done before any units are created.
// ** TODO Fix minimap issues with 128xY, possibly fix major rendering/crash issues with 64xY
bool SetForcedWorldMapPatch(
  bool enable)
{
  static PatchContext patcher;
  bool success = true;

  if (enable) {
    // In Map::AllocateSpaceForMap(), force world map loading path
    patcher.LowLevelHook(0x435532, [] { return 0x435550; });
    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) || (success == false)) {
    success &= ((patcher.RevertAll() == Status::Ok));
  }

  return success;
}
