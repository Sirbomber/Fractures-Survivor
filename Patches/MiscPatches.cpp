#include <windows.h>
#include <winuser.h>
#include <Outpost2DLL.h>
#include <OP2Helper.h>
#include <hfl.h>
#include <OP2App\CConfig.h>
#include <OP2Types.h>
#include "HookHelper.h"
#include "UnitHookHelper.h"
#include "ExtLocation.h"
#include "Cutscene.h"
#include "MiscPatches.h"

//char **stringTable = (char**)OP2Addr(0x4E88F8);
//stringTable[id] = "my string here";

namespace
{
	struct StructureFuncs
	{
		static int __thiscall AdvancedLab_GetRolloverText(OP2Unit *classPtr, char *destBuffer, int bufferSize);
		static bool __thiscall CheckSpontaneouslyExplode(OP2Unit *classPtr);
		static void __thiscall DoDockDamage(OP2Unit *classPtr);
	};
}


// ConVec hover text hack
namespace {
	struct ConVecFuncs {
		static int __thiscall GetRolloverText(OP2Unit *classPtr, char *destBuffer, int bufferSize);
	};
}

int __thiscall ConVecFuncs::GetRolloverText(OP2Unit *classPtr, char *destBuffer, int bufferSize) {
	if (classPtr->ownerNum == TethysGame::LocalPlayer() && classPtr->cargoOrWeapon)
	{
		return scr_snprintf(destBuffer, bufferSize, "%s (%s)",
							unitInfoArray[classPtr->vtbl->GetUnitTypeInfo(classPtr)->unitType]->unitName,
							unitInfoArray[classPtr->cargoOrWeapon]->unitName);
	}
	else
	{
		return oldUnitVtbl[classPtr->vtbl->GetUnitTypeInfo(classPtr)->unitType].GetRolloverText(classPtr, destBuffer, bufferSize);
	}
}


bool SetStructurePatches()
{
	static bool structurePatches = false;

	if (structurePatches)
		return true;

	InitUnitVtblAddresses();

	OP2PatchManager::PatchReferencedVirtualFunction(unitVtbl[mapConVec]->GetRolloverText, ConVecFuncs::GetRolloverText, false);

	for (int i = mapCommonOreMine; i <= mapTokamak; i ++)
	{
		if (i == mapAdvancedLab)
		{
			OP2PatchManager::PatchReferencedVirtualFunction(unitVtbl[i]->GetRolloverText, StructureFuncs::AdvancedLab_GetRolloverText, false);
		}

		if ((DWORD)unitVtbl[i]->CheckSpontaneouslyExplode == OP2Addr(0x409400))
		{
			OP2PatchManager::PatchReferencedVirtualFunction(unitVtbl[i]->CheckSpontaneouslyExplode, StructureFuncs::CheckSpontaneouslyExplode, false);
		}

		if ((DWORD)unitVtbl[i]->DoEvent == OP2Addr(0x482D90))
		{
			OP2PatchManager::PatchReferencedVirtualFunction(unitVtbl[i]->DoEvent, StructureFuncs::DoDockDamage, false);
		}
	}

	// Hide gaia structure status icons (they are normally visible until/unless Gaia unallies with the local player)
	OP2PatchManager::PatchFunction((void*)0x4091E8, &Unit_DrawUnit_StatusLight_Trampoline, false);

	structurePatches = true;
	return true;
}


int __thiscall StructureFuncs::AdvancedLab_GetRolloverText(OP2Unit *classPtr, char *destBuffer, int bufferSize)
{
	if (classPtr->ownerNum == 6)
	{
		memset(destBuffer, 0, bufferSize);
		return scr_snprintf(destBuffer, bufferSize, "TEH MES HAL");
	}
	else
		return oldUnitVtbl[classPtr->vtbl->GetUnitTypeInfo(classPtr)->unitType].GetRolloverText(classPtr, destBuffer, bufferSize);
}


bool __thiscall StructureFuncs::CheckSpontaneouslyExplode(OP2Unit *classPtr)
{
	if (classPtr->flags & UNIT_PREVENTEXPLOSIONONDEATH)
		return false;

	return oldUnitVtbl[classPtr->vtbl->GetUnitTypeInfo(classPtr)->unitType].CheckSpontaneouslyExplode(classPtr);
}


void __thiscall StructureFuncs::DoDockDamage(OP2Unit *classPtr)
{
	int (__thiscall *Unit_GetDockLocation)(OP2Unit *classPtr, LOCATION *returnedLocation) = (int (__thiscall*)(OP2Unit*,LOCATION*))OP2Addr(0x482F40);

	LOCATION dockLocation;
	UnitEx unitOnDock;

	if (Unit_GetDockLocation(classPtr, &dockLocation) == 0)
		return; // Couldn't find dock location

	unitOnDock.unitID = GameMapEx::GetTileEx(dockLocation).unitIndex;
	if (unitOnDock.unitID == 0 || !unitOnDock.IsLive())
		return;

	if (ExtPlayer[classPtr->ownerNum].IsAlliedTo(unitOnDock.OwnerID()))
		return;

	return oldUnitVtbl[classPtr->vtbl->GetUnitTypeInfo(classPtr)->unitType].DoEvent(classPtr);
}


bool SetWallPatch()
{
	static bool wallPatch = false;

	if (wallPatch)
		return true;

	// Fix the "cannot build walls in a 3x3 area around another wall being built" bug
	OP2PatchManager::Patch<unsigned short>((void*)0x438E07, 0x9090, false);
	// Make earthworkers invincible to stop the invisible perma-wall bug
	OP2PatchManager::PatchFunction((void*)0x43BD79, &Unit_MoBuildWall_Trampoline, false);
	OP2PatchManager::PatchFunction((void*)0x43BE1C, &Unit_MoBuildWallDone_Trampoline, false); // One item completed, still others in queue
	OP2PatchManager::PatchFunction((void*)0x43BE5E, &Unit_MoBuildWallDoneAll_Trampoline, false); // All items completed

	wallPatch = true;
	return true;
}

bool SetMeteorDefensePatch()
{
	static bool mdPatch = false;

	if (mdPatch)
		return true;

	OP2PatchManager::Patch<unsigned char>((void*)OP2Addr(0x4803C0), 0x64, false);

	mdPatch = true;
	return true;
}

bool SetPalettePatch()
{
	static bool palettePatch = false;

	// todo: COPER <-> TITAENUM patch
	palettePatch = true;
	return true;
}


bool SetMissionEndHook()
{
	static bool missionEndHook = false;

	if (missionEndHook)
		return true;

	OP2PatchManager::Patch<unsigned char>((void*)0x4033A9, 0x00, false);
	OP2PatchManager::PatchFunction((void*)0x49CB44, &MissionEndTrampoline, false);

	missionEndHook = true;
	return true;
}


bool SetEvacuationModulePatch()
{
	static bool evacuationModulePatch = false;

	if (evacuationModulePatch)
		return true;

	OP2PatchManager::Patch<unsigned short>((void*)0x40F7EA, 0x0190, false); // in Player.ProcessCommandPacket; check if you have 200 colonists before proceeding (validity check)
	OP2PatchManager::Patch<unsigned short>((void*)0x4574CB, 0x0190, false); // in UICommand::MouseCommand::Launch.IsEnabled; check if you have 200 colonists, if so, enable launch button
	OP2PatchManager::Patch<unsigned short>((void*)0x47FF8D, 0x0190, false); // in Unit.Launch; number of colonists to remove

	evacuationModulePatch = true;
	return true;
}


bool SetAllyMessagePatch()
{
	static bool allyMessagePatch = false;

	if (allyMessagePatch)
		return true;

	// Skip "player X has allied with you/broken the alliance" if X >= 6 (Gaia), or if tick == 0
	OP2PatchManager::PatchFunction((void*)0x40FFC5, &AllianceFormedBrokenFromMessage_Trampoline, false);
	// Skip "alliance formed with player X/broken with player X" if X >= 6 (Gaia), or if tick == 0
	OP2PatchManager::PatchFunction((void*)0x410051, &AllianceFormedBrokenWithMessage_Trampoline, false);

	allyMessagePatch = true;
	return true;
}


bool SetFoodStoresArePlentifulPatch()
{
	static bool foodStoresArePlentifulPatch = false;

	if (foodStoresArePlentifulPatch == true)
		return true;

	OP2PatchManager::Patch<unsigned char>((void*)0x4721D4, 0xEB, 0x7F, false);

	foodStoresArePlentifulPatch = true;
	return true;
}


void __declspec(naked) Unit_DrawUnit_StatusLight_Trampoline()
{
	static OP2Unit *classPtr;
	__asm
	{
		mov classPtr, edx

		pushad
	}
	static int localPlayerNum;
	static DWORD jumpToAddr;

	if (classPtr->ownerNum == 6)
		jumpToAddr = OP2Addr(0x4092E8);
	else
		jumpToAddr = OP2Addr(0x4091ED);

	localPlayerNum = ((OP2Game*)gameObj)->localPlayer;
	__asm
	{
		popad

		// Reissue overwritten instructions
		mov eax, localPlayerNum
		// Jump back to original function
		jmp jumpToAddr
	}
}


void __declspec(naked) Unit_MoBuildWall_Trampoline()
{
	static OP2Unit *thisUnit;
	__asm
	{
		mov thisUnit, esi

		pushad
	}
	if (thisUnit->tubeOrWallType != mapTube)
		thisUnit->flags &= ~UNIT_CANBEDAMAGED;

	static int fastProduction;
	fastProduction = ((OP2Game*)gameObj)->fastProduction;

	static DWORD jumpToAddr;
	jumpToAddr = OP2Addr(0x43BD7E);
	__asm
	{
		popad

		// Reissue overwritten instructions
		mov eax, fastProduction
		// Jump back to original function
		jmp jumpToAddr
	}
}


void __declspec(naked) Unit_MoBuildWallDone_Trampoline()
{
	static OP2Unit *thisUnit;
	__asm
	{
		mov thisUnit, esi

		pushad
	}
	if (thisUnit->tubeOrWallType != mapTube)
		thisUnit->flags |= UNIT_CANBEDAMAGED;

	static DWORD jumpToAddr;
	jumpToAddr = OP2Addr(0x43BE21);
	__asm
	{
		popad

		// Reissue overwritten instructions
		push 3
		mov eax, [esi + 0x18]
		// Jump back to original function
		jmp jumpToAddr
	}
}


void __declspec(naked) Unit_MoBuildWallDoneAll_Trampoline()
{
	static OP2Unit *thisUnit;
	__asm
	{
		mov thisUnit, esi

		pushad
	}
	if (thisUnit->tubeOrWallType != mapTube)
		thisUnit->flags |= UNIT_CANBEDAMAGED;

	static int localPlayerNum;
	localPlayerNum = ((OP2Game*)gameObj)->localPlayer; // Should be a bit faster than calling TethysGame::LocalPlayer()

	static DWORD jumpToAddr;
	jumpToAddr = OP2Addr(0x43BE64);
	__asm
	{
		popad

		// Reissue overwritten instructions
		mov ecx, localPlayerNum
		// Jump back to original function
		jmp jumpToAddr
	}
}


void __declspec(naked) PaletteTrampoline()
{
	__asm
	{
		pushad
	}
	__asm
	{
		popad
	}
}


void __declspec(naked) MissionEndTrampoline()
{
	__asm
	{
		pushad
	}
	static DWORD jumpToAddr;
	static int gameTermReasons = ((OP2Game*)gameObj)->gameTermReasons;

	if (gConfigFile.GetInt("Game", "ShowMovies", 1))
	{
		if (gameTermReasons == 3 || gameTermReasons == 7) // Mission accomplished or victory is sweet
			PlayAvi("MEF.AVI", true);
		else if (gameTermReasons == 4 || gameTermReasons == 8) // Mission failed or you have been defeated
			PlayAvi("MEBL.AVI", true);
	}

	// If single player, make the game think the mission was a demo so it doesn't play another cutscene after ours
	if (gameTermReasons == 3 || gameTermReasons == 4)
		gameTermReasons = 10;

	jumpToAddr = OP2Addr(0x49CB49);
	__asm
	{
		popad

		// Reissue overwritten instructions
		mov eax, gameTermReasons;
		// Jump back to original function
		jmp jumpToAddr;
	}
}


void __declspec(naked) AllianceFormedBrokenFromMessage_Trampoline()
{
	static int thisPlayerNum, playerNum;
	__asm
	{
		mov ecx, [ebp + 0]
		mov thisPlayerNum, ecx
		mov playerNum, esi

		pushad
	}
	static DWORD jumpToAddr;

	if (playerNum != ((OP2Game*)gameObj)->localPlayer
		|| thisPlayerNum >= 6
		|| ((OP2Game*)gameObj)->tick == 0)
		jumpToAddr = OP2Addr(0x410051); // skip
	else
		jumpToAddr = OP2Addr(0x40FFD1);
	__asm
	{
		popad

		// Jump back to original function
		jmp jumpToAddr;
	}
}


void __declspec(naked) AllianceFormedBrokenWithMessage_Trampoline()
{
	static int thisPlayerNum, playerNum;
	__asm
	{
		mov ecx, [ebp + 0]
		mov thisPlayerNum, ecx
		mov playerNum, esi

		pushad
	}
	static DWORD jumpToAddr;

	if (thisPlayerNum != ((OP2Game*)gameObj)->localPlayer
		|| playerNum >= 6
		|| ((OP2Game*)gameObj)->tick == 0)
		jumpToAddr = OP2Addr(0x4100C8); // skip
	else
		jumpToAddr = OP2Addr(0x41005D);

	__asm
	{
		popad

		// Jump back to original function
		jmp jumpToAddr;
	}
}
