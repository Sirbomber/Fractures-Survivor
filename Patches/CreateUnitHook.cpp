
#include "Tethys/API/Unit.h"
#include "Tethys/API/Player.h"
#include "Tethys/Resource/SpriteManager.h"

#include "Patcher.h"
#include "Util.h"

using namespace Tethys;
using namespace Tethys::TethysAPI;
using namespace Patcher;

#if CCF2_BUILD
struct OP2Unit;
void CheckNewUnitForAI(OP2Unit* pUnit, int player);
#endif

// =====================================================================================================================
int __fastcall Unit_SetCreator(
  MapObject*  pThis,  int,
  uint8       creatorId)
{
  pThis->creatorNum_ = creatorId;
  pThis->ownerNum_   = creatorId;

  TethysUtil::SetBitFlag(pThis->flags_, MoFlagEden, Player[creatorId].IsEden());

#if CCF2_BUILD
  if ((pThis->GetTypeID() == MapID::Panther) && Player[creatorId].HasTechnology(8202)) {
    // Panther Weapons Systems Upgrade
    pThis->flags_ |= MoFlagDoubleFireRate;
  }
  CheckNewUnitForAI((OP2Unit*)(pThis), creatorId);
#elif SWARMZ_BUILD
  const int unitType = pThis->GetTypeID();

  if ((((unitType >= MapID::Scorpion) && (unitType <= MapID::Panther)) ||
       (unitType == MapID::GuardPost)) && Player[creatorId].HasTechnology(13006))
  {
    pThis->flags_ |=  MoFlagDoubleFireRate;
  }
  else if ((unitType == MapID::Tiger) && Player[creatorId].HasTechnology(13007)) {
    pThis->flags_ &= ~MoFlagDoubleFireRate;
  }
#endif

  return pThis->flags_;
}

// =====================================================================================================================
bool SetCreateUnitHook(
  bool enable)
{
  static PatchContext patcher;
  bool success = true;

  if (enable && (patcher.NumPatches() == 0)) {
#if 0  // This is in 1.4.0
    // Replace {Lynx,Panther,Tiger}.SelectTurretGraphic() vftable entries with our function
    for (auto* pVtbl : { MapObj::Lynx::Vtbl(), MapObj::Panther::Vtbl(), MapObj::Tiger::Vtbl() }) {
      patcher.Write(&pVtbl->pfnSelectTurretGraphic, ThiscallLambdaPtr(
        [](TankVehicle* pThis, MapChildEntity* pWeaponUnit, int rotation) {
          // Select the weapon type animation index.
          auto*const pWeaponType = MapEntityType::GetInstance(pThis->weapon_);
            (pThis->flags_ & MoFlagDoubleFireRate) ? pWeaponType->SelectTigerGraphic(pThis, rotation)   :
            (pThis->GetTypeID() != MapID::Lynx)    ? pWeaponType->SelectPantherGraphic(pThis, rotation)
                                                   : pWeaponType->SelectLynxGraphic(pThis, rotation);

          const int rotationIndex           = (pThis->rotation_ + 8) / 16; // Round to nearest multiple of 16 (22.5 deg)
          const int animationIndex          = pThis->GetChassisAnimationIndex(rotationIndex);
          const FrameOptionalInfo frameInfo = SpriteManager::GetInstance()->GetFrameOptionalInfo(animationIndex, 0);

          // Update the selected turret animation global variable with the offsets.
          OP2Mem<0x4EA7BC, int&>() += frameInfo.offsetX;
          OP2Mem<0x4EA7C0, int&>() += frameInfo.offsetY;
        }));
    }
#endif

    // Replace Unit.SetOwner with a replacement that also sets the dual turret flag on units when desired
    patcher.Hook(0x43A090, &Unit_SetCreator);

    success = (patcher.GetStatus() == Status::Ok);
  }

  if ((enable == false) || (success == false)) {
    success &= (patcher.RevertAll() == Status::Ok);
  }

  return success;
}
