
#include "Tethys/API/Unit.h"
#include "Tethys/API/Player.h"
#include "Tethys/API/TethysGame.h"
#include "Tethys/Game/MapObject.h"
#include "Tethys/Game/MapObjectManager.h"
#include "Tethys/Resource/SoundManager.h"

#include "Patcher.h"
#include "Util.h"

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <utility>

using namespace Tethys;
using namespace TethysAPI;
using namespace Patcher;

static constexpr auto MissileTypeBegin = static_cast<MapID>(0x74);  // One past MapID::MaxObject

static constexpr char NukeName[]     = "Nuclear Missile";
static constexpr char NukeCodeName[] = "NUKE_MISSILE";

// ** TODO Look into why single-player cost scaling isn't being applied
static constexpr uint32 NukeCommonCost = 15000;
static constexpr uint32 NukeRareCost   = 10000;

static constexpr uint32 NukeTechId = 12201; // Rocket Atmospheric Re-Entry System (12201)


static int g_nukeUpgrade1TechId = NukeTechId;
static int g_nukeUpgrade2TechId = NukeTechId;

static bool g_inited = false;

static std::unordered_set<int>                                g_missileIds = { MapID::EMPMissile };
static std::unordered_map<int, std::string>                   g_missileCodeName;
static std::unordered_map<std::string, int>                   g_codeNameToMissileId;
static std::unordered_map<int, MapObjType::EMPMissile>        g_missileMoType;
static std::unordered_map<int, MapObj::EMPMissile::VtblType>  g_missileVtbl;

static constexpr size_t MaxUnitTypes         = 255;
static constexpr uint32 EndOfBuildListMarker = 0xFFFFFF9C;

static MapObjectType*      g_pMoTypeArray[MaxUnitTypes + 1] = { };

static std::vector<uint32> g_spaceportBuildList = {
  MapID::EDWARDSatellite,
  MapID::SolarSatellite,
  MapID::IonDriveModule,
  MapID::FusionDriveModule,
  MapID::CommandModule,
  MapID::FuelingSystems,
  MapID::HabitatRing,
  MapID::SensorPackage,
  MapID::Skydock,
  MapID::StasisSystems,
  MapID::OrbitalPackage,
  MapID::PhoenixModule,

  MapID::RareMetalsCargo,
  MapID::CommonMetalsCargo,
  MapID::FoodCargo,
  MapID::EvacuationModule,
  MapID::ChildrenModule,

  MapID::SULV,
  MapID::RLV,
  MapID::EMPMissile,

  EndOfBuildListMarker,
};

// =====================================================================================================================
bool SetMissilePatches(
  bool enable)
{
  static PatchContext patcher;
  bool success = true;

  if (enable && (patcher.NumPatches() == 0)) {
#if 0  // These are in 1.4.0
    static constexpr uint32 NewRocketAnimsEden[] = {
      // Missile launch,  SULV launch,  RLV launch,  Missile land,  ?
      1868,               1868,         1871,        1873,          0,
      // RLV land,        Missile fly,  SULV fly,    RLV fly,       ?
      1864,               1872,         1872,        1874,          0
    };

    // Fix missile not landing for Eden.  Cause was missing animation table entries for missiles for Eden.
    patcher.Write(0x4E3F88, NewRocketAnimsEden);

    // Set Meteor Defenses to ignore missiles of players the MD's owner is allied to
    // In EmpMissile::DoEvent()
    patcher.LowLevelHook(0x4802A2, [](Eax<int> owner, Ecx<int> target)
                                     { return Player[owner].IsAlliedTo(target) ? 0x480414 : 0; });
#endif

    // Set Observatory tracking to never miss EMP Missiles (so Meteor Defense always hits them)
    patcher.Write<uint16>(0x4803D5, 0x9090);

    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) || (success == false)) {
    success &= ((patcher.RevertAll() == Status::Ok));
  }

  return success;
}

// =====================================================================================================================
static void __fastcall Nuke_CreateBlast(
  Rocket*  pThis)
{
  const auto MakeExplosion = [pThis](int x, int y, MapID type = MapID::Supernova, SoundID soundID = SoundID::Count) {
  auto*const pMapObject    = static_cast<Explosive*>(
    g_mapObjManager.CreateMapObject(type, (x * 32), ((y * 32) + 1), pThis->ownerNum_, MapID::None, 0, 1));

    if (pMapObject != nullptr) {
      pMapObject->flags_   |= MoFlagOffensive;
      pMapObject->ownerNum_ = 6;
      // Weapon source must be set to itself, or else it will be repositioned to be centered over the source.
      pMapObject->SetWeaponSource(pMapObject->index_);
      pMapObject->DoExplode(pMapObject);
    }

    if (soundID != SoundID::Count) {
      TethysGame::AddMapSound(soundID, Location(x, y));
    }
  };

  const int x     = (pThis->pixelX_ / 32);
  const int y     = (pThis->pixelY_ / 32);
  const int owner = pThis->ownerNum_;

  if ((owner == 6) || Player[owner].HasTechnology((g_nukeUpgrade2TechId))) {
    MakeExplosion(x, y - 1, MapID::LargeBuildingExplosion);
  }

  MakeExplosion(x, y, MapID::Supernova, SoundID::Bigexp2);

  if ((owner == 6) || Player[owner].HasTechnology((g_nukeUpgrade1TechId))) {
    MakeExplosion(x - 1, y, MapID::MediumBuildingExplosion);

    static constexpr std::pair<int, int> Deltas[] = {
      { -4,  0 }, {  4, 0 }, { 0, -4 }, { 0, 4 },
      { -3, -3 }, { -3, 3 }, { 3, -3 }, { 3, 3 },
    };

    int numSounds = 0;
    for (const auto [dx, dy] : Deltas) {
      MakeExplosion((x + dx), (y + dy), MapID::Supernova, (numSounds++ < 2) ? SoundID::Bigexp3 : SoundID::Count);
    }
  }

  if ((owner == 6) || Player[owner].HasTechnology((g_nukeUpgrade2TechId))) {
    static constexpr std::pair<int, int> Deltas[] = {
      { -8,  0 }, {  8, 0 }, { 0, -8 }, { 0, 8 },
      { -7, -3 }, { -7, 3 }, { 7, -3 }, { 7, 3 },
      { -3, -7 }, { -3, 7 }, { 3, -7 }, { 3, 7 },
      { -5, -5 }, { -5, 5 }, { 5, -5 }, { 5, 5 },
    };

    for (const auto [dx, dy] : Deltas) {
      MakeExplosion((x + dx), (y + dy));
    }
  }
}

// =====================================================================================================================
// Make sure building a nuke gets to the launchpad and not the storage area
static int __fastcall Spaceport_FinishProduction(
  MapObj::Spaceport*  pThis,  int,
  MapID              type,
  int                 a)
{
  if (g_missileIds.count(type) || (type == MapID::SULV) || (type == MapID::RLV)) {
    pThis->objectOnPad_ = type;

    if (pThis->ownerNum_ == TethysGame::LocalPlayer()) {
      const SoundID soundID = (type == MapID::RLV) ? SoundID::Savant48 : SoundID::Savant49;
      g_soundManager.AddSavantSound(soundID, pThis->pixelX_, pThis->pixelY_, -1);
    }

    return 1;
  }
  else {
    return FactoryBuilding::Vtbl()->pfnFinishProduction(pThis, type, a);
  }
}

// =====================================================================================================================
static int __fastcall Spaceport_GetRocketAnimId(
  MapObj::Spaceport*  pThis,  int,
  int                 objectOnPad)
{
  const bool isEden = ((pThis->flags_ & MoFlagEden) != 0);

  switch (objectOnPad) {
  case MapID::SULV:  return isEden ? 1322 : 1325;
  case MapID::RLV:   return isEden ? 1321 : 1439;
  default:           return (g_missileIds.count(objectOnPad)) ? (isEden ? 1322 : 1857) : 1322;
  }
}

// =====================================================================================================================
static MapObject* __fastcall MissileMoFactory_CreateUnit(
  MapObjectType*  pThis,  int,
  int             pixelX,
  int             pixelY,
  int             unitIndex)
{
  MapObject*const pNewUnit = MapObjType::EMPMissile::GetInstance()->Create(pixelX, pixelY, unitIndex);
  if (pNewUnit != nullptr) {
    pNewUnit->Vfptr() = &g_missileVtbl[pThis->type_];
  }
  return pNewUnit;
}

// =====================================================================================================================
MapID AddMissile(
  void*        pfnCreateBlast,  // void (__thiscall*)(OP2Unit* pThis)
  const char*  pUnitName,
  const char*  pCodeName,
  int          common,
  int          rare,
  int          techId,
  int          forceMapId = 0)
{
  static uint32 numMissileTypes = 0;

  static std::unordered_map<void*, MapID>                 pfnCreateBlastToMapId;
  static std::unordered_map<int, MapObjectType::VtblType>  missileMoFactoryVtbl;

  MapID missileType = MapID::None;

  const auto it = pfnCreateBlastToMapId.find(pfnCreateBlast);
  if (it == pfnCreateBlastToMapId.end()) {
    // Allocate a MapID for this missile type.
    missileType = static_cast<MapID>((forceMapId != 0) ? forceMapId : (MissileTypeBegin + (numMissileTypes++)));
    assert(static_cast<size_t>(missileType) < MaxUnitTypes);
    g_missileIds.insert(missileType);
    pfnCreateBlastToMapId[pfnCreateBlast] = missileType;

    // Initialize the Unit Info from a copy of the EMP missile one.
    g_missileMoType.emplace(missileType, MapObjType::EMPMissile{ });
    memcpy(&g_missileMoType[missileType], MapObjType::EMPMissile::GetInstance(), sizeof(MapObjectType));

    // We have to define our own CreateUnit and GetCodeName virtual functions.
    // Copy the vtbl over.
    missileMoFactoryVtbl[missileType] = { };
    memcpy(&missileMoFactoryVtbl[missileType],
           MapObjType::EMPMissile::Vtbl(),
           sizeof(MapObjType::EMPMissile::VtblType));
    g_missileMoType[missileType].Vfptr() = &missileMoFactoryVtbl[missileType];

    // Initialize our vtbl.
    reinterpret_cast<void*&>(missileMoFactoryVtbl[missileType].pfnCreate) = &MissileMoFactory_CreateUnit;
    missileMoFactoryVtbl[missileType].pfnGetCodeName =
      ThiscallLambdaPtr([](MapObjectType* pThis) -> const char* { return g_missileCodeName[pThis->type_].data(); });

    // Initialize other data members that we will need.
    g_missileMoType[missileType].type_ = missileType;
    strncpy_s(g_missileMoType[missileType].unitName_,    40, pUnitName, _TRUNCATE);
    strncpy_s(g_missileMoType[missileType].produceName_, 40, pUnitName, _TRUNCATE);

    for (int i = 0; i < 7; ++i) {
      g_missileMoType[missileType].playerStats_[i].commonCost = common;
      g_missileMoType[missileType].playerStats_[i].rareCost   = rare;
    }

    for (int i = 0; i < g_research.numTechs_; ++i) {
      if (g_research.ppTechInfos_[i]->techID == techId) {
        g_missileMoType[missileType].requiredTechID_ = i;
        break;
      }
    }

    g_codeNameToMissileId[pCodeName] = missileType;
    g_missileCodeName[missileType]   = pCodeName;

    // Initialize our Unit vtbl (again, copied from EMP missile vtbl)
    g_missileVtbl[missileType] = { };
    memcpy(&g_missileVtbl[missileType],
           MapObj::EMPMissile::Vtbl(),
           sizeof(MapObj::EMPMissile::VtblType));

    reinterpret_cast<void*&>(g_missileVtbl[missileType].pfnGetType) = ThiscallLambdaPtr(
      [](Rocket* pThis) { return &g_missileMoType[pfnCreateBlastToMapId[pThis->Vfptr()->pfnLand]]; });
    reinterpret_cast<void*&>(g_missileVtbl[missileType].pfnLand)    = pfnCreateBlast;

    // Insert new entry in the spaceport build list.
    g_spaceportBuildList.back() = missileType;
    g_spaceportBuildList.push_back(EndOfBuildListMarker);

    // Update the unit info table to add our new unit.
    // ** TODO need to undo this
    MapObjectManager::GetMapObjTypeTable()[missileType] = &g_missileMoType[missileType];
  }
  else {
    missileType = it->second;
  }

  return missileType;
}

// =====================================================================================================================
void SetNukeUpgradeTechIds(
  int nukeUpgrade1TechId = NukeTechId,
  int nukeUpgrade2TechId = NukeTechId)
{
  g_nukeUpgrade1TechId = nukeUpgrade1TechId;
  g_nukeUpgrade2TechId = nukeUpgrade2TechId;
}

// =====================================================================================================================
MapID GetMissileId(
  const char* pCodeName)
{
  const auto it = g_codeNameToMissileId.find(pCodeName);
  return (it != g_codeNameToMissileId.end()) ? static_cast<MapID>(it->second) : MapID::None;
}

// =====================================================================================================================
bool SetNukePatch(
  bool enable)
{
  static PatchContext patcher;
  static PatchContext typePatcher(MapObjectManager::GetMapObjTypeTable());
  bool success = true;

  if (enable && (patcher.NumPatches() == 0)) {
#if 0  // This is in 1.4.0
    class DummyObjectType : public MapObjType::MaxObjectType {
    public:
      ibool Save(StreamIO* pSavedGame) override { return 1; }
      ibool Load(StreamIO* pSavedGame) override { return 1; }
    } static g_dummyMapObjType;

    // Extend the unit info array.
    static auto*const pOldMoFactoryArray = Op2Mem<MapObjectType**>(0x4E1348);
    memcpy(&g_pMoFactoryArray[0], pOldMoFactoryArray, sizeof(MapObjectType*[mapMaxObject]));
    for (size_t i = mapMaxObject; i <= MaxUnitTypes; g_pMoFactoryArray[i++] = &g_dummyMapObjType);
    patcher.ReplaceReferencesToGlobal(0x4E1348, sizeof(MapObjectType*[mapMaxObject]), &g_pMoFactoryArray[0]);
    patcher.ReplaceReferencesToGlobal(0x4E1514, &g_pMoFactoryArray[MaxUnitTypes]);
#endif
    typePatcher.Touch(
      MapObjectManager::GetMapObjTypeTable(), sizeof(MapObjectType*) * MapObjectManager::NumMapObjTypes());

    // Patch extra entries into the SP build list.
    patcher.LowLevelHook(0x4682DB, [](Edi<void*>& pBuildList) { pBuildList = &g_spaceportBuildList[0]; });

    // Patch spaceport vtbl with our "finished production" method.
    patcher.Write(&MapObj::Spaceport::Vtbl()->pfnFinishProduction, &Spaceport_FinishProduction);

    // Patch Spaceport::GetRocketAnimId so we can supply graphics for other missiles.
    patcher.Hook(0x47F670, &Spaceport_GetRocketAnimId);

    // Show the attack cursor for missiles.
    patcher.LowLevelHook(0x457512, [](Ebx<int> objectOnPad) { return g_missileIds.count(objectOnPad) ? 0x457517 : 0; });

    // Disables the option to build missiles when there is a rocket on the pad.
    patcher.LowLevelHook(0x468654, [](Eax<int> unitType) { return g_missileIds.count(unitType) ? 0x46865E : 0; });

    // Hides the option to build missiles when there is a rocket on the pad.
    patcher.LowLevelHook(0x456993, [](Ebp<int> unitType) { return g_missileIds.count(unitType) ? 0x456C39 : 0; });

    // Make missile graphic show up in lower right corner of launch command pane for missiles.
    patcher.LowLevelHook(0x463ECD, [](Edx<int> objectOnPad) {
      return g_missileIds.count(objectOnPad) ? (Player[TethysGame::LocalPlayer()].IsEden() ? 0x463ED2 : 0x463F0B) : 0;
    });

    // Hide displaying cargo for missiles on launch command pane.
    patcher.LowLevelHook(0x463F7D, [](Edx<int> objectOnPad) { return g_missileIds.count(objectOnPad) ? 0x463F82 : 0; });

    // Hide displaying cargo for missiles on Spaceport main command pane.
    patcher.LowLevelHook(0x45BFE4, [](Ecx<int> objectOnPad) { return g_missileIds.count(objectOnPad) ? 0x45BFE9 : 0; });

    // Hook GetLaunchAnim checks for mapEMPMissile.
    patcher.LowLevelHook(0x4807FC, [](Eax<int> unitType) { return g_missileIds.count(unitType) ? 0x48082B : 0; });
    patcher.LowLevelHook(0x480854, [](Ecx<int> unitType) { return g_missileIds.count(unitType) ? 0x480859 : 0; });

    // Hook ProcessCommandPacket checks for mapEMPMissile.
    patcher.LowLevelHook(0x40EA10, [](Ebx<int> toProduce) { return g_missileIds.count(toProduce) ? 0x40EA15 : 0; });

    // Hook ProcessForGameCycle checks for mapEMPMissile.
    patcher.LowLevelHook(0x43B253, [](Ecx<int>  unitType) { return g_missileIds.count(unitType) ? 0x43B25C : 0; });
    patcher.LowLevelHook(0x43C8B7, [](Eax<int>  unitType) { return g_missileIds.count(unitType) ? 0x43C8F4 : 0; });
    patcher.LowLevelHook(0x43C1B7, [](Eax<int8> beingProduced)
      { return g_missileIds.count(beingProduced) ? 0x43C1BF : 0; });

    g_inited = true;
    AddMissile(&Nuke_CreateBlast, NukeName, NukeCodeName, NukeCommonCost, NukeRareCost, NukeTechId, MapID::BFG);

    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) || (success == false)) {
    success &= (patcher.RevertAll()     == Status::Ok);
    success &= (typePatcher.RevertAll() == Status::Ok);
  }

  return success;
}
