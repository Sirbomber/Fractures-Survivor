#include <windows.h>
#include <Outpost2DLL.h>
#include <OP2Helper.h>
#include <hfl.h>
#include <OP2Types.h>
#include "ExtLocation.h"
#include "HookHelper.h"
#include "BlightHook.h"

bool blightHooked = false;
OP2BlightMgr *blightObj;
MapTile *infectedTiles;

FightGroup microbePoofGroup;

namespace
{
	bool blightVisible = true;
	OP2BlightMgr::OP2BlightMgrVtbl oldBlightObjVtbl, *blightObjVtbl;
	OP2Patch *patchNoBlightOverlay, *patchForcedBlightOverlay;


	struct BlightHook
	{
		static void __thiscall SpreadTo(OP2BlightMgr *classPtr, LOCATION where);
		static void __thiscall InfectAreaNear(OP2BlightMgr *classPtr, LOCATION where);
		static void __thiscall ShowMinimapCoverage(OP2BlightMgr *classPtr);
	};
}


int TileOffset(LOCATION where)
{
	return TileOffset(SplitLoc(where));
}
int TileOffset(int tileX, int tileY)
{
	OP2Map *map = (OP2Map*)mapObj;
	return ((map->tileXMask & tileX) & 31) + (((((map->tileXMask & tileX) >> 5) << map->logTileHeight) + tileY) << 5);

	/*int xLower = map->tileXMask & tileX;
	int xUpper = xLower >> 5;
	xLower &= 31;
	xUpper <<= map->logTileHeight;
	xUpper += tileY;
	xUpper <<= 5;
	return xUpper + xLower;*/
}


// Replacement member function for Blight.SpreadTo
void __thiscall BlightHook::SpreadTo(OP2BlightMgr *classPtr, LOCATION where)
{
	if (!isInited || !blightHooked)
		return;

	// A whole lotta pointers
	OP2Map *map = (OP2Map*)mapObj;
	int *spreadOffsetTable = (int*)OP2Addr(0x4EA478);
	OP2MapCell *cellTypeData = (OP2MapCell*)OP2Addr(0x4DEBA8);

	//char *Map_lightLevelBrightnessAdjustment = (char*)OP2Addr(0x54F854);
	char *Map_lightLevelBrightnessAdjustment = *reinterpret_cast<char**>(OP2Addr(0x49DEED));
	int *actualDaylightPosition = (int*)OP2Addr(0x5472DC);

	void (__fastcall *DoDamageWall)(int x, int y, int damage) = (void (__fastcall*)(int,int,int))OP2Addr(0x4A24C0);

	void *minimapObj = (void*)OP2Addr(0x574458);
	void (__thiscall *Minimap_SetColor)(void *thisPtr, LOCATION where, short color) = (void (__thiscall*)(void*,LOCATION,short))OP2Addr(0x48D2B0);

	void *detailPaneViewport = (void*)(*(DWORD*)OP2Addr(0x5758C8) + 0x110);
	int *detailPaneUnkWidth = (int*)(*(DWORD*)OP2Addr(0x5758C8) + 0x940);
	int *detailPaneUnkHeight = (int*)(*(DWORD*)OP2Addr(0x5758C8) + 0x944);
	void (__thiscall *Viewport_UpdateBackgroundRectToRedraw)(void *thisPtr, OP2Rect *rect) = (void (__thiscall*)(void*,OP2Rect*))OP2Addr(0x46F670);

	MapTile tile;
	LocationEx location, curLocation, spreadLocation;
	unsigned int microbeBitmask = 0;
	int bUseAltSpreadOffset, relativeDaylightPosition;
	OP2Rect updateRect;

	// This is a copy of the internal spread offset table. Not being used currently, but if you want to use different values,
	// then this array could be modified and you could point the function to this instead of the internal table
	static int mySpreadOffsetTable[2 * 2 * 15] = {
		-1000, -1000,	-1000, -1000,
		1, 0,			0, 1,
		0, 0,			1, 1,
		0, 1,			1, 1,
		0, 0,			1, 1,
		1, 0,			1, 1,
		0, 0,			1, 1,
		1, 1,			1, 1,
		0, 1,			1, 0,
		1, 0,			0, 1,
		0, 0,			0, 1,
		0, 1,			0, 1,
		0, 0,			1, 0,
		1, 0,			1, 0,
		-1000, -1000,	-1000, -1000
	};

	location = LocationEx(where).Wrapped();
	if (!infectedTiles[TileOffset(location)].expand)
		return;

	int i = 0;
	for (int y = location.y; y <= location.y + 1; y ++)
	{
		for (int x = location.x; x <= location.x + 1; x ++)
		{
			curLocation = LocationEx(x, y).WrappedXY();
			if (GameMapEx::GetTileEx(curLocation).microbe)
				microbeBitmask |= (1 << i);

			i ++;
		}
	}

	if (!microbeBitmask)
	{
		infectedTiles[TileOffset(location)].expand = 0;
		return;
	}
	else if (microbeBitmask == 15)
		return; // Do nothing if all four tiles have the microbe bit set

	classPtr->_08 = classPtr->_08 ^ 1;
	bUseAltSpreadOffset = classPtr->_08 & 2; // I don't see what this is supposed to accomplish, Blight._08 is always 0 or 1 so this operation just sets the local to 0 either way

	spreadLocation = LocationEx(location.x + spreadOffsetTable[0 + (2 * (bUseAltSpreadOffset + (2 * (microbeBitmask))))],
		location.y + spreadOffsetTable[1 + (2 * (bUseAltSpreadOffset + (2 * (microbeBitmask))))]).Wrapped();
	if (spreadLocation.y < 0 || spreadLocation.y > (map->tileHeight - 1)
		|| spreadLocation.y == location.y - 1000)
		return;

	tile = GameMapEx::GetTileEx(spreadLocation);
	relativeDaylightPosition = LocationEx(spreadLocation.x - *actualDaylightPosition, spreadLocation.y).Wrapped().x;

	if ((cellTypeData[GameMap::GetCellType(spreadLocation)].blightSpeed
		- Map_lightLevelBrightnessAdjustment[relativeDaylightPosition]) > (TethysGame::Tick() % 40))
	{
		/* OP2's behavior
		for (int y = spreadLocation.y; y >= spreadLocation.y - 1; y --)
		{
			if (y < 0)
				break;
			for (int x = spreadLocation.x; x <= spreadLocation.x + 1; x ++)
			{
				curLocation = LocationEx(x, y).Wrapped();
				if (GameMap::GetCellType(curLocation) == cellMicrobeWall)
				{
					DoDamageWall(SplitLoc(curLocation), 15);
					return;
				}
			}
		}*/

		if (tile.cellType == cellMicrobeWall)
		{
			DoDamageWall(SplitLoc(spreadLocation), 15);
			// Arklon had this at 90...
			return;
		}

		if (tile.lava)
			return;

		classPtr->vtbl->SetBlight(classPtr, spreadLocation);
		if (blightVisible)
		{
			if (!map->paddingOffsetTileX || (map->paddingOffsetTileX < spreadLocation.x && (map->paddingOffsetTileX + (map->tileWidth / 2)) >= spreadLocation.x))
				Minimap_SetColor(minimapObj, spreadLocation, 12315);

			if (detailPaneViewport)
			{
				updateRect.right = ((spreadLocation.x * 32) & (map->pixelWidthMask - 1)) - (map->pixelWidthMask & *detailPaneUnkWidth);
				if (((int)map->pixelWidthMask / -2) > updateRect.right)
					updateRect.right &= map->pixelWidthMask;
				if (updateRect.right > ((int)map->pixelWidthMask / 2))
					updateRect.right -= map->pixelWidthMask;
				updateRect.right += 40;
				updateRect.bottom = (spreadLocation.y * 32) - *detailPaneUnkHeight + 40;
				updateRect.left = updateRect.right - 48;
				updateRect.top = updateRect.bottom - 48;

				Viewport_UpdateBackgroundRectToRedraw(detailPaneViewport, &updateRect);
			}
		}
	}
}


// Replacement member function for Blight.InfectAreaNear
void __thiscall BlightHook::InfectAreaNear(OP2BlightMgr *classPtr, LOCATION where)
{
	OP2Map *p = (OP2Map*)mapObj;
	MAP_RECT rect = MAP_RECT(where.x - 1, where.y - 1, where.x, where.y);
	MapTile tile;
	LocationEx curLocation;
	UnitEx unit;

	bool foundMicrobe;

	if (rect.y1 < 0)
		rect.y1 = 0;
	if (rect.y2 >= p->tileHeight)
		rect.y2 = p->tileHeight - 1;

	for (int x = rect.x1; x <= rect.x2; x ++)
	{
		for (int y = rect.y1; y <= rect.y2; y ++)
		{
			curLocation = LocationEx(x, y).Wrapped();
			tile = GameMapEx::GetTileEx(curLocation);
			unit.unitID = tile.unitIndex;

			foundMicrobe = false;
			for (int i = 0; !foundMicrobe && i <= 1; i ++)
			{
				for (int j = 0; !foundMicrobe && j <= 1; j ++)
				{
					curLocation = LocationEx(x + i, y + j).Wrapped();
					tile = GameMapEx::GetTileEx(curLocation);
					if (tile.microbe)
					{
						foundMicrobe = true;

						if (tile.unitIndex != 0 && unit.IsLive())
						{
							BlightHook_DoInfect(unit);
						}
					}
				}
			}
			curLocation = LocationEx(x, y).Wrapped();
			infectedTiles[TileOffset(curLocation)].expand = foundMicrobe;
		}
	}

	return;
}


void BlightHook_InfectionProc(int playerId)
{
	UnitEx curUnit;
	OP2Unit *internalUnit;
	int relativeDaylightPosition;
	static unsigned int tickOfLastInfectedMsg = 0, tickOfLastCall[7] = { 0, 0, 0, 0, 0, 0, 0 };

	char *Map_lightLevelBrightnessAdjustment = (char*)OP2Addr(0x54F854);
	int *actualDaylightPosition = (int*)OP2Addr(0x5472DC);
	OP2MapCell *cellTypeData = (OP2MapCell*)OP2Addr(0x4DEBA8);

	if (!blightHooked || !isInited)
		return;

	PlayerBuildingEnum bldgEnum(playerId, mapNone);
	while (bldgEnum.GetNext(curUnit))
	{
		if (!curUnit.IsLive())
			continue;

		internalUnit = &(*unitArray)[curUnit.unitID];

		if (internalUnit->flags & UNIT_ISINFECTED)
		{
			curUnit.SetDamage(curUnit.GetDamage() + TethysGame::GetRand(5) + 5);
			if (curUnit.GetDamage() >= curUnit.GetUnitInfo().GetHitPoints(curUnit.CreatorID()) || curUnit.GetDamage() < 0
				|| curUnit.GetLastCommand() == ctMoDevelop || curUnit.GetLastCommand() == ctMoUnDevelop)
				curUnit.DoDeath();
		}
		else if (infectedTiles[TileOffset(curUnit.Location())].expand)
		{
			BlightHook_DoInfect(curUnit);
		}
	}

	PlayerVehicleEnum vecEnum(playerId);
	while (vecEnum.GetNext(curUnit) && playerId != 6)
	{
		if (!curUnit.IsLive())
			continue;

		internalUnit = &(*unitArray)[curUnit.unitID];

		if (internalUnit->flags & UNIT_ISUNDOCKING)
		{
			continue;
		}

		if (!(internalUnit->flags & UNIT_EX_INFECTEDVEHICLE))
		{
			if (curUnit.GetCurAction() == moMove || curUnit.GetCurAction() == weaponMove)
			{
				if (infectedTiles[TileOffset(curUnit.Location())].expand)
				{
					BlightHook_DoInfect(curUnit);
				}
			}
		}
		else
		{
			if (internalUnit->Vehicle.timerInfection > 0)
			{
					if ((unsigned int)TethysGame::Tick() - tickOfLastCall[playerId] > 0)
					{
						relativeDaylightPosition = LocationEx(curUnit.Location().x - *actualDaylightPosition, curUnit.Location().y).Wrapped().x;

						internalUnit->Vehicle.timerInfection -= ((unsigned int)TethysGame::Tick() - tickOfLastCall[playerId]) * 
							(cellTypeData[GameMap::GetCellType(curUnit.Location())].blightSpeed - Map_lightLevelBrightnessAdjustment[relativeDaylightPosition]);
					}

					if (!infectedTiles[TileOffset(curUnit.Location())].expand ||
						internalUnit->Vehicle.timerInfection <= 0)
					{
						// Transfer the vehicle to gaia
						internalUnit->vtbl->TransferUnit(internalUnit, 6);
						TethysGame::AddMapSound(sndMicrobe, curUnit.Location());

						if (TethysGame::LocalPlayer() == playerId)
						{
							if ((unsigned int)TethysGame::Tick() - tickOfLastInfectedMsg > 256
								|| tickOfLastInfectedMsg > (unsigned int)TethysGame::Tick())
							{
								char nameBuf[0x20], msgBuf[0x64];
								internalUnit->vtbl->GetSelectedUnitDisplayString(internalUnit, nameBuf, sizeof(nameBuf));
								scr_snprintf(msgBuf, sizeof(msgBuf), "%s infected", nameBuf);
								TethysGame::AddMessage(SplitLoc(LocationEx(curUnit.Location()).ToPixels()), msgBuf, playerId, sndSavnt211);

								tickOfLastInfectedMsg = (unsigned int)TethysGame::Tick();
							}
						}
					}
			}
		}
	}

	tickOfLastCall[playerId] = (unsigned int)TethysGame::Tick();
}


void BlightHook_DoInfect(UnitEx & unit)
{
	if (!isInited || !blightHooked || !unit.IsLive() ||
		unit.unitID <= 0 || unit.unitID > ((OP2Map*)mapObj)->lastUsedUnitIndex)
		return;

	void (__thiscall *Building_KillWorkers)(OP2Unit *thisPtr) = (void (__thiscall*)(OP2Unit*))OP2Addr(0x482BF0);

	void *unitSelectionObj = (void*)OP2Addr(0x547210);
	int (__thiscall *UnitSelection_GetNumSelectedUnits)(void *thisPtr) = (int (__thiscall*)(void*))OP2Addr(0x41A9A0);
	int* (__thiscall *UnitSelection_GetSelectedUnitIndexArray)(void *thisPtr) = (int* (__thiscall*)(void*))OP2Addr(0x41A9B0);
	void (__thiscall *UnitSelection_DeselectAll)(void *thisPtr, void *hwnd) = (void (__thiscall*)(void*,void*))OP2Addr(0x41AA00);

	OP2Unit *internalUnit = &(*unitArray)[unit.unitID];

	if (unit.IsBuilding() && !(internalUnit->flags & UNIT_ISINFECTED))
	{
		TethysGame::AddMapSound(sndMicrobe, unit.Location());

		internalUnit->flags |= UNIT_ISINFECTED;
		Building_KillWorkers(internalUnit);
		unit.SetDamage(unit.GetDamage() + (unit.GetUnitInfo().GetHitPoints(unit.CreatorID()) / 3));
		if (unit.GetLastCommand() == ctMoDevelop || unit.GetLastCommand() == ctMoUnDevelop
			|| unit.GetDamage() >= unit.GetUnitInfo().GetHitPoints(unit.CreatorID()))
		{
			internalUnit->flags |= UNIT_ISDEAD;
			unit.DoDeath();
			internalUnit->flags |= UNIT_HASSPECIALTARGET2;
		}
		else
		{
			internalUnit->action = 0;
			internalUnit->command = 0;
			if (UnitSelection_GetNumSelectedUnits(unitSelectionObj) == 1 && UnitSelection_GetSelectedUnitIndexArray(unitSelectionObj)[0] == unit.unitID)
				UnitSelection_DeselectAll(unitSelectionObj, NULL);

			if (internalUnit->ownerNum != 6)
			{
				if (unit.GetType() == mapVehicleFactory)
				{
					internalUnit->vtbl->TransferUnit(internalUnit, 6);
				}
			}
		}
	}
	else if (unit.IsVehicle() && !(internalUnit->flags & UNIT_EX_INFECTEDVEHICLE))
	{
		internalUnit->flags |= UNIT_EX_INFECTEDVEHICLE;
		internalUnit->Vehicle.timerInfection = 100 * 100;
		if(Player[internalUnit->creatorNum].HasTechnology(6202))
		{
			internalUnit->Vehicle.timerInfection += 300 * 100;
		}
	}
}


/* Note: When OP2 calls Blight.ShowMinimapCoverage (when you change the minimap view settings), it directly calls the original in a non-virtual way,
 and doesn't read the blight manager vtbl to get the address of it. */
// Replacement member function for Blight.ShowMinimapCoverage
void __thiscall BlightHook::ShowMinimapCoverage(OP2BlightMgr *classPtr)
{
	if (!blightVisible || !isInited || !blightHooked)
		return;

	void *minimapObj = (void*)OP2Addr(0x574458);
	void (__thiscall *Minimap_SetColor)(void *thisPtr, LOCATION where, short color) = (void (__thiscall*)(void*,LOCATION,short))OP2Addr(0x48D2B0);

	for (int x = 0; x < ((OP2Map*)mapObj)->tileWidth; x ++)
	{
		for (int y = 0; y < ((OP2Map*)mapObj)->tileHeight; y ++)
		{
			if (infectedTiles[TileOffset(LOCATION(x, y))].expand && !GameMapEx::GetTileEx(LOCATION(x, y)).lava)
				Minimap_SetColor(minimapObj, LOCATION(x, y), 12315);
		}
	}
}


bool SetBlightHook(bool boolOn)
{
	if (!isInited)
		return false;
	if (boolOn == blightHooked)
		return true;

	blightObj = (OP2BlightMgr*)OP2Addr(0x57B7D8);
	blightObjVtbl = blightObj->vtbl;

	if (boolOn)
	{
		// Force the map blight overlay to be drawn
		patchForcedBlightOverlay = OP2PatchManager::Patch<unsigned int>((void*)0x4700E2, 0xFFFFFFFF, 0x20000000, false);
		// Queue up a patch that hides the blight overlay
		patchNoBlightOverlay = OP2PatchManager::Patch<unsigned int>((void*)0x4700E2, 0x00000000, 0xFFFFFFFF, true);

		// Overwrite the beginning of Blight.ShowMinimapCoverage with a jump to our function (necessary to hook it this way since the game always calls it in a non-virtual way)
		OP2PatchManager::PatchFunction(blightObjVtbl->ShowMinimapCoverage, &BlightHook::ShowMinimapCoverage, false);

		// Replace blight vtbl entries with our functions
		OP2PatchManager::PatchReferencedVirtualFunction(blightObjVtbl->SpreadTo, BlightHook::SpreadTo, false);
		OP2PatchManager::PatchReferencedVirtualFunction(blightObjVtbl->InfectAreaNear, BlightHook::InfectAreaNear, false);
		OP2PatchManager::PatchReferencedVirtualFunction(blightObjVtbl->ShowMinimapCoverage, BlightHook::ShowMinimapCoverage, false);

		// Patch LevelDll.RaiseUnitTransferredEvent so the game doesn't crash when we transfer infected units to gaia (P6)
		OP2PatchManager::PatchFunction((void*)0x4031C6, &LevelDll_RaiseUnitTransferredEvent_Trampoline, false);

		// olololOLOLLOOLOLOL
		//OP2PatchManager::Patch<unsigned char>((void*)0x4A8A93, 0x2C, 0x28, false);
		//OP2PatchManager::Patch<unsigned char>((void*)0x4A8AA7, 0x28, 0x2C, false);

		// Allocate the array for storing our tile infection data
		infectedTiles = new MapTile[((OP2Map*)mapObj)->numTiles];
		memset(infectedTiles, 0, sizeof(MapTile) * ((OP2Map*)mapObj)->numTiles);

		// Set up the ScGroup non-combat infected units will get transferred to
		microbePoofGroup = CreateFightGroup(6);
		microbePoofGroup.DoExitMap();
		microbePoofGroup.SetRect(MAP_RECT(MapLocation(1,10), MapLocation(2,28)));
	}
	else
	{
		// Deallocate the tile infection data array
		delete [] infectedTiles;
	}

	blightHooked = boolOn;

	return true;
}


void SetBlightVisible(bool isVisible)
{
	if (!blightHooked || isVisible == blightVisible)
		return;

	void *detailPaneViewport = (void*)(*(DWORD*)OP2Addr(0x5758C8) + 0x110);
	void (__thiscall *Viewport_MarkEverywhereForRedraw)(void *thisPtr) = (void (__thiscall*)(void*))OP2Addr(0x46FCC0);

	blightVisible = isVisible;

	if (isVisible)
	{
		// Force the map blight overlay to be drawn
		patchForcedBlightOverlay->Patch(true);

		if (detailPaneViewport)
			Viewport_MarkEverywhereForRedraw(detailPaneViewport); // Mark the detail pane for redraw

		// Draw the blight coverage on the minimap
		blightObj->vtbl->ShowMinimapCoverage(blightObj);
	}
	else
	{
		// Bypass the map blight overlay drawing code
		patchNoBlightOverlay->Patch(true);
	}
}


Unit infectedUnit;
void __declspec(naked) LevelDll_RaiseUnitTransferredEvent_Trampoline()
{
	static int destPlayerNum;
	static OP2Unit *transferredUnit;
	__asm
	{
		mov destPlayerNum, eax
		mov transferredUnit, esi

		pushad
	}

	infectedUnit.unitID = transferredUnit->unitIndex;
	if (destPlayerNum == 6)
	{
		if (!(transferredUnit->flags & UNIT_ISOFFENSIVE))
			infectedUnit.DoSelfDestruct();
			//microbePoofGroup.TakeUnit(infectedUnit);
	}

	static DWORD jumpToAddr;
	if (Player[destPlayerNum].IsHuman() || destPlayerNum == 6)
		jumpToAddr = OP2Addr(0x4031F2);
	else
		jumpToAddr = OP2Addr(0x4031E1);

	__asm
	{
		popad

		// Jump back to original function
		jmp jumpToAddr
	}
}


// Function to handle infected vehicles belonging to gaia, as AI functions do not work with it
void FindInfectedTankTargets()
{
	UnitClassifactions (__fastcall *GetUnitClassification)(map_id unitType, map_id cargoOrWeapon) = (UnitClassifactions (__fastcall*)(map_id,map_id))OP2Addr(0x49D270);

	UnitEx curUnit, curTargetUnit;
	OP2Unit *internalUnit;
	bool doTargetReadjustment;

	PlayerVehicleEnum microbeVecEnum(6);

	MAP_RECT clipRect = MAP_RECT(LocationEx(-64, 64).Clipped(), LocationEx(512, 512).Clipped());
	static unsigned long testDistance = clipRect.Width() * clipRect.Height();
	static unsigned int tickOfLastCall = 0, tickOfLastTargetReadjustment = 0;
	
	if ((unsigned int)TethysGame::Tick() - tickOfLastTargetReadjustment < 120)
		doTargetReadjustment = false; // Not re-adjusting attacking unit targets right now (only once every 120 ticks)
	else
	{
		doTargetReadjustment = true;
		tickOfLastTargetReadjustment = (unsigned int)TethysGame::Tick();
	}

	while (microbeVecEnum.GetNext(curUnit))
	{
		if (!curUnit.IsLive() // Dead units can show up in an enumerator
			|| !curUnit.IsVehicle()) // Mining beacons, etc. get enumerated by a PlayerVehicleEnum in addition to vehicles
			continue;

		internalUnit = &(*unitArray)[curUnit.unitID];

		// Vehicles that were created by an AI player (or a dropped player) don't like being controlled; also, if the creator player has different upgrades
		// than what Gaia has, particularly weapon range upgrades, the vehicle may not act right
		if (internalUnit->creatorNum != 6)
			internalUnit->creatorNum = 6;

		if (!(internalUnit->flags & UNIT_EX_INFECTEDVEHICLE)) // Non-infected vehicle
			continue;

		if ((unsigned int)TethysGame::Tick() - tickOfLastCall > 0)
			internalUnit->Vehicle.timerInfection += ((unsigned int)TethysGame::Tick() - tickOfLastCall);
		if (internalUnit->Vehicle.timerInfection >= 250 * 100)
		{
			// Vehicle has been attempting to complete its current action for too long, destroy it
			curUnit.DoSelfDestruct();
			continue;
		}

		if ((curUnit.GetLastCommand() == ctMoAttackObj && !doTargetReadjustment)
			|| !(internalUnit->flags & UNIT_ISOFFENSIVE))
			continue;

		if (!curUnit.IsBusy())
			internalUnit->Vehicle.timerInfection = 0; // Vehicle completed its action, reset self-destruct timer

		ClosestEnumerator closestUnitEnum(curUnit.Location());
		while (closestUnitEnum.GetNext(curTargetUnit, testDistance))
		{
			if (!curTargetUnit.IsBuilding() && !curTargetUnit.IsVehicle())
				continue;

			if (!ExtPlayer[6].IsAlliedTo(curTargetUnit.OwnerID()))
			{
				curUnit.DoAttack(LOCATION(curTargetUnit.unitID, -1));
				break;
			}
		}
	}

	tickOfLastCall = (unsigned int)TethysGame::Tick();
}

Export void SpawnInfectedLynx()
{
	Unit VF,
		 Lynx;
	OP2Unit *intVF,
		    *intLynx;

	PlayerBuildingEnum InfectedVFs(6, mapVehicleFactory);
	while (InfectedVFs.GetNext(VF))
	{
		if (VF.IsLive())
		{
			intVF = &(*unitArray)[VF.unitID];
			if (intVF->flags & UNIT_ISEDEN)
			{
				TethysGame::CreateUnit(Lynx, mapLynx, LOCATION(VF.Location().x+2,VF.Location().y), 6, mapLaser, 0);
				intLynx = &(*unitArray)[Lynx.unitID];
				intLynx->flags |= UNIT_ISEDEN;
				intLynx->flags |= UNIT_EX_INFECTEDVEHICLE;

				TethysGame::CreateUnit(Lynx, mapLynx, LOCATION(VF.Location().x+2,VF.Location().y+1), 6, mapLaser, 0);
				intLynx = &(*unitArray)[Lynx.unitID];
				intLynx->flags |= UNIT_ISEDEN;
				intLynx->flags |= UNIT_EX_INFECTEDVEHICLE;

				TethysGame::CreateUnit(Lynx, mapLynx, LOCATION(VF.Location().x+2,VF.Location().y+2), 6, mapLaser, 0);
				intLynx = &(*unitArray)[Lynx.unitID];
				intLynx->flags |= UNIT_ISEDEN;
				intLynx->flags |= UNIT_EX_INFECTEDVEHICLE;
			}

			else
			{
				TethysGame::CreateUnit(Lynx, mapLynx, LOCATION(VF.Location().x+2,VF.Location().y), 6, mapMicrowave, 0);
				intLynx = &(*unitArray)[Lynx.unitID];
				intLynx->flags &= ~UNIT_ISEDEN;
				intLynx->flags |= UNIT_EX_INFECTEDVEHICLE;

				TethysGame::CreateUnit(Lynx, mapLynx, LOCATION(VF.Location().x+2,VF.Location().y+1), 6, mapMicrowave, 0);
				intLynx = &(*unitArray)[Lynx.unitID];
				intLynx->flags &= ~UNIT_ISEDEN;
				intLynx->flags |= UNIT_EX_INFECTEDVEHICLE;

				TethysGame::CreateUnit(Lynx, mapLynx, LOCATION(VF.Location().x+2,VF.Location().y+2), 6, mapMicrowave, 0);
				intLynx = &(*unitArray)[Lynx.unitID];
				intLynx->flags &= ~UNIT_ISEDEN;
				intLynx->flags |= UNIT_EX_INFECTEDVEHICLE;
			}

		}
	}
}