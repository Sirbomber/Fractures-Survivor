// Fractures Survivor
// Players: 5
// Gametype: Survivor (Space Race)
// Map: Fractures (modified)
// Techtree: Survtech
// Created by: Sirbomber
// Additional support: Arklon, BlackBox

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Outpost2DLL.h>
#include <OP2Helper.h>
#include <hfl.h>
#include "functions.h"
#include "AI\FraxSurvAi.h"
#include "Patches\Patcher.h"
#include "Patches\RandomResources.h"
#include "Patches\Patches.h"
#include "Patches\DIRTRepair.h"
#include "Patches\Cutscene.h"

#include "PlayerEvents\PlayerEvents.h"

// ----------------------------------------------------------------------------

Export char MapName[] = "fraxsurv.map";
Export char LevelDesc[] = "5P, SRV, 'Fractured Alliance v2.0 TEST'";
Export char TechtreeName[] = "survtech.txt";
#ifndef _DEBUG
Export AIModDesc DescBlock	    = { MultiSpaceRace, 5, 12, 0 };
Export int DescBlockEx[] = { 1, 0, 0, 0, 0, 0, 0, 0 }; // Add one extra player slot for AI
#else
Export AIModDesc DescBlock = { Colony, 6, 12, 0 };
#endif

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{		
		if (fdwReason == DLL_PROCESS_ATTACH) {
			g_hInst = hinstDLL; // Save the DLL's base address

			bool result = (HFLInit() == HFLLOADED);
			result = result && SetBlightPatch(true);
			result = result && SetCreateUnitHook(true);
			result = result && SetStructureDamagePatches(true);
			result = result && SetMissionEndHook(true, "MEF.AVI", "MEBL.AVI");
			result = result && SetMissilePatches(true);
			result = result && RandomRes::SetHiddenBeaconPatch(true);

			if (result == false) {
				return FALSE;
			}
		}
		else if (fdwReason == DLL_PROCESS_DETACH) {
			// ** TODO this should be in 1.4.0
			TethysGame::SetMicrobeSpreadSpeed(0);
		}

		return TRUE;
	}
}

// -----------------------------------------------------------------------

FraxSurvAI fxsAI;

Export int InitProc()
{
	#ifdef _DEBUG
	((OP2Game*)gameObj)->gameStartInfo.startupFlags.moraleOn = true;
	((OP2Game*)gameObj)->gameStartInfo.startupFlags.dayAndNightOn = true;
	((OP2Game*)gameObj)->gameStartInfo.startupFlags.diastersOn = true;
	((OP2Game*)gameObj)->gameStartInfo.startupFlags.initialVehicles = 12;
	Player[0].GoEden();
	Player[0].SetTechLevel(13);
	Player[0].SetColorNumber(1);
	Player[1].SetColorNumber(3);
	ExtPlayer[0].SetSatelliteCount(mapEDWARDSatellite, 1);
	#endif

	// Map/object/disaster/player initialization
	DoSurvSetup();      // Handles morale settings and the playlist
	SetupTutorial();     // Creates victory and defeat conditions
	SetAllLava();       // Set dark gray rock to allow lava to spread onto it
	ConfigureDisasterCreator();
	DoRandomBases();    // Randomize and place bases
	SetupWreckage();    // Place wreckage
	SetupBlight();      // Trigger to spawn Blight

	// Determine AI player number.
	int numAI = -1;
	for (int i = 0; i < TethysGame::NoPlayers(); i++)
	{
		Player[i].AllyWith(6);
		Player[6].AllyWith(i);
		if (!Player[i].IsHuman() && numAI < 0)
		{
			numAI = i;
		}
		else
		{
			CreatePlayerEvents(i);
			CreateCountTrigger(1, 1, i, mapEDWARDSatellite, mapAny, 1, cmpGreaterEqual, "DiscoverAllBeacons");
			Player[i].MarkResearchComplete(12003);		// Dual-Turret Panther enabler
			Player[i].MarkResearchComplete(12002);		// Wall HP tech enabler
			Player[i].MarkResearchComplete(12005);		// Microbe armor enabler
		}
	}

	for (int i = 0; i < numAI; i++)
	{
		for (int j = 0; j < numAI; j++)
		{
			if (i != j)
				Player[i].AllyWith(j);
		}
	}

	Player[numAI].GoPlymouth();
	fxsAI.SetupAI(numAI);

	PlaceOre();
	SetupDirtRepair();

	// Force RCC effect
	((OP2Game*)gameObj)->forceRCC = 1;

	// Disable the RCC and set Leisure Studies as unresearchable
	bool rccDisabled = false,
		 recDisabled = false;
	for (int i = 0; i < ((OP2Research*)researchObj)->numTechs; ++i)
	{
		if (rccDisabled && recDisabled)
		{
			break;
		}
		else if (((OP2Research*)researchObj)->techInfos[i]->techID == 9706)
		{
			unitInfoArray[mapRobotCommand]->techId = i;
			rccDisabled = true;
		}
		else if (((OP2Research*)researchObj)->techInfos[i]->techID == 3306)
		{
			((OP2Research*)researchObj)->techInfos[i]->plyCost = -1;
			((OP2Research*)researchObj)->techInfos[i]->edenCost = -1;
			recDisabled = true;
		}
	}

	// Infected vehicle stuff
	CreateTimeTrigger(1, 0, 10 * 4, "FindInfectedTankTargets");
	CreateTimeTrigger(1, 0, 2200, 4600, "SpawnInfectedLynx");

	// Force Eden effect
	CreateTimeTrigger(1, 0, 10 * 5, "PlymzToEden");

	return 1; // return 1 if OK; 0 on failure

}

// ----------------------------------------------------------------------------

// Runs AI logic.  The time trigger that calls this runs every 100 ticks and is set up in FracSurvAi_MainBase.cpp
Export void UpdateCycle()
{
	fxsAI.UpdateCycle();
}

// Call the custom Blight logic and check if a Scout should reveal a hidden mining beacon.
Export void AIProc()
{
    for (int i = 0; i < TethysGame::NoPlayers(); i ++)
	{
		BlightInfectionProc(i);
		RandomRes::LocateBeaconProc(i);
	}
}

// Unused, but if someone wants to port this mission into a colony game, this would be required.
void __cdecl GetSaveRegions(struct BufferDesc &bufDesc)
{
	bufDesc.bufferStart = 0;	// Pointer to a buffer that needs to be saved
	bufDesc.length = 0;			// sizeof(buffer)
}

// Used when a trigger does not require a callback function (NoResponseToTrigger).
Export void None()
{
}

// Reveals all hidden beacons once at least one player has launched the EDWARD Satellite.
Export void DiscoverAllBeacons()
{
	UnitEx curBeacon;

	PlayerUnitEnum beaconEnum(6);
	for (int p = 0; p < TethysGame::NoPlayers(); p ++)
	{
		if (ExtPlayer[p].GetSatelliteCount(mapEDWARDSatellite) >= 1)
		{
			RandomRes::DiscoverAllBeacons(p);
			while (beaconEnum.GetNext(curBeacon))
			{
				if (curBeacon.GetType() == mapMiningBeacon)
					(*unitArray)[curBeacon.unitID].Beacon.surveyed |= (1 << p);
			}
		}
	}
}

// Sets the "is Eden" bit on any vehicles built by the Eden outpost Vehicle Factories.
Export void PlymzToEden()
{
	Unit Unit1;
	InRectEnumerator EdenVFsArea(MAP_RECT(1+31, 127-1, 30+31, 148-1));
	while (EdenVFsArea.GetNext(Unit1))
	{
		if (Unit1.OwnerID() == fxsAI.PlayerNum() &&
			(Unit1.GetWeapon() == mapLaser ||
			 Unit1.GetWeapon() == mapRailGun ||
			 Unit1.GetWeapon() == mapAcidCloud ||
			 Unit1.GetWeapon() == mapThorsHammer ||
			 Unit1.GetType() == mapEvacuationTransport))
		{
			OP2Unit *internalUnit = &(*unitArray)[Unit1.unitID];
			internalUnit->flags |= UNIT_ISEDEN;
		}
	}
}

// Abandoned Vehicle Factories infected by the Blight will start spawning Lynx every 200~400 marks.
// These Lynx are hostile to the players and the AI.
// TO DO: Check original owner's tech progress and possibly spawn other unit/weapon types?
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
			//int origOwner = intVF->creatorNum;
			//map_id vehicle, weapon;
			
			if (intVF->flags & UNIT_ISEDEN)
			{
				TethysGame::CreateUnit(Lynx, mapLynx, LOCATION(VF.Location().x + 2, VF.Location().y), 6, mapLaser, 0);
				intLynx = &(*unitArray)[Lynx.unitID];
				intLynx->flags |= UNIT_ISEDEN;
				intLynx->flags |= UNIT_EX_INFECTEDVEHICLE;

				TethysGame::CreateUnit(Lynx, mapLynx, LOCATION(VF.Location().x + 2, VF.Location().y + 1), 6, mapLaser, 0);
				intLynx = &(*unitArray)[Lynx.unitID];
				intLynx->flags |= UNIT_ISEDEN;
				intLynx->flags |= UNIT_EX_INFECTEDVEHICLE;

				TethysGame::CreateUnit(Lynx, mapLynx, LOCATION(VF.Location().x + 2, VF.Location().y + 2), 6, mapLaser, 0);
				intLynx = &(*unitArray)[Lynx.unitID];
				intLynx->flags |= UNIT_ISEDEN;
				intLynx->flags |= UNIT_EX_INFECTEDVEHICLE;
			}

			else
			{
				TethysGame::CreateUnit(Lynx, mapLynx, LOCATION(VF.Location().x + 2, VF.Location().y), 6, mapMicrowave, 0);
				intLynx = &(*unitArray)[Lynx.unitID];
				intLynx->flags &= ~UNIT_ISEDEN;
				intLynx->flags |= UNIT_EX_INFECTEDVEHICLE;

				TethysGame::CreateUnit(Lynx, mapLynx, LOCATION(VF.Location().x + 2, VF.Location().y + 1), 6, mapMicrowave, 0);
				intLynx = &(*unitArray)[Lynx.unitID];
				intLynx->flags &= ~UNIT_ISEDEN;
				intLynx->flags |= UNIT_EX_INFECTEDVEHICLE;

				TethysGame::CreateUnit(Lynx, mapLynx, LOCATION(VF.Location().x + 2, VF.Location().y + 2), 6, mapMicrowave, 0);
				intLynx = &(*unitArray)[Lynx.unitID];
				intLynx->flags &= ~UNIT_ISEDEN;
				intLynx->flags |= UNIT_EX_INFECTEDVEHICLE;
			}

		}
	}
}

// -------------------------------------------------------------

