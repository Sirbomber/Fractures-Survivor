#include <windows.h>
#include <memory.h>
#include <hfl.h>
#include "DualTurretUnit.h"
#include "OP2Types\OP2Types.h"
#include "HookHelper.h"
#include "UnitHookHelper.h"

namespace
{
	// New DualTurretGP virtual function table
	const int GPVtblSize = 65;
	void* dualTurretGPvftbl[GPVtblSize];


	struct DualTurretUnit
	{
		static void __thiscall GetTurretAnimationIndex(OP2Unit *baseUnit, OP2Unit *weaponUnit, int rotation);
		static void __thiscall DrawDualTurretGPUnit(void *thisGuardPost, void *graphicsObject);
		static int __thiscall SetCreator(OP2Unit *thisUnit, char creatorId);
		static OP2Unit* __thiscall Panther_CreateUnit(OP2UnitInfo *thisPtr, int pixelX, int pixelY, int unitIndex);
	};
}


void* SetupDualTurretGP()
{
	// Copy the vtbl we want to edit
	memcpy(&dualTurretGPvftbl, (void*)OP2Addr(0x4CFFB0), sizeof(dualTurretGPvftbl));
	// Update the DrawUnit function pointer
	dualTurretGPvftbl[6] = &DualTurretUnit::DrawDualTurretGPUnit;

	// Return the new virtual function table
	return dualTurretGPvftbl;
}


bool SetTurretAnimationHook()
{
	static bool turretAnimHook = false;

	if (turretAnimHook)
		return true;

	InitUnitVtblAddresses();
	
	// Replace GetTurretAnimationIndex with our function
	OP2PatchManager::PatchReferencedVirtualFunction(unitVtbl[mapLynx]->GetTurretAnimationIndex, DualTurretUnit::GetTurretAnimationIndex, false);
	OP2PatchManager::PatchReferencedVirtualFunction(unitVtbl[mapPanther]->GetTurretAnimationIndex, DualTurretUnit::GetTurretAnimationIndex, false);
	OP2PatchManager::PatchReferencedVirtualFunction(unitVtbl[mapTiger]->GetTurretAnimationIndex, DualTurretUnit::GetTurretAnimationIndex, false);

	// Replace Unit.SetOwner with a replacement that also sets the dual turret flag on units when desired
	OP2PatchManager::PatchFunction((void*)0x43A090, &DualTurretUnit::SetCreator, false);

	turretAnimHook = true;
	return true;
}


// Replacement member function for Unit:Tank
void __thiscall DualTurretUnit::GetTurretAnimationIndex(OP2Unit *baseUnit, OP2Unit *weaponUnit, int rotation)
{
	OP2FrameOptionalInfo frameInfo;
	int rotationIndex, animationIndex;

	if (!isInited)
		return;

	OP2FrameOptionalInfo (__thiscall *Graphics_GetFrameOptionalInfo)(void *graphics, int animIndex, int frameIndex) = (OP2FrameOptionalInfo (__thiscall*)(void*,int,int))OP2Addr(0x404F00);
	// Globals
	int* animationOffsetX = (int*)OP2Addr(0x4EA7BC);
	int* animationOffsetY = (int*)OP2Addr(0x4EA7C0);

	// Get the chassis rotation
	rotationIndex = (baseUnit->rotation + 8) / 16;  // Round to nearest multiple of 16 (22.5 degrees)

	// Get the weapon type animation index
	if (baseUnit->flags & UNIT_DOUBLEFIRERATE) // Check for double fire rate
		unitInfoArray[baseUnit->cargoOrWeapon]->vtbl->GetTigerTurretAnimationIndex(unitInfoArray[baseUnit->cargoOrWeapon], baseUnit, rotation);
	else
		unitInfoArray[baseUnit->cargoOrWeapon]->vtbl->GetPantherTurretAnimationIndex(unitInfoArray[baseUnit->cargoOrWeapon], baseUnit, rotation);

	animationIndex = baseUnit->vtbl->GetChassisAnimationIndex(baseUnit, rotationIndex);
	frameInfo = Graphics_GetFrameOptionalInfo(graphicsObj, animationIndex, 0);

	// Update the global variable with the offsets
	*animationOffsetX += frameInfo.offsetX;
	*animationOffsetY += frameInfo.offsetY;
}


// Replacement member function for Unit:GuardPost
void __thiscall DualTurretUnit::DrawDualTurretGPUnit(void *thisGuardPost, void *graphicsObject)
{
	// ...
}

// Replacement function for Unit.SetCreator
int __thiscall DualTurretUnit::SetCreator(OP2Unit *thisUnit, char creatorId)
{
	thisUnit->creatorNum = creatorId;
	thisUnit->ownerNum = creatorId;

	if (Player[creatorId].IsEden() ||
		thisUnit->cargoOrWeapon == mapLaser ||
		thisUnit->cargoOrWeapon == mapRailGun ||
		thisUnit->cargoOrWeapon == mapAcidCloud ||
		thisUnit->cargoOrWeapon == mapThorsHammer)
	{
		thisUnit->flags |= UNIT_ISEDEN;
	}

	else
	{
		thisUnit->flags &= ~UNIT_ISEDEN;
	}

	if ((thisUnit->vtbl->GetUnitTypeInfo(thisUnit)->unitType == mapPanther && Player[creatorId].HasTechnology(8202) == 1))
		thisUnit->flags |= UNIT_DOUBLEFIRERATE; // Set the double fire rate flag for the unit

	return thisUnit->flags;
}
