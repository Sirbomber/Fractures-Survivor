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
#include "Patches\ExtLocation.h"
#include "Patches\RandomResources.h"
#include "Patches\HookHelper.h"
#include "Patches\UnitHookHelper.h"
#include "Patches\DualTurretUnit.h"
#include "Patches\BlightHook.h"
#include "Patches\MiscPatches.h"
#include "Patches\DIRTRepair.h"
#include "Patches\Cutscene.h"
#include "Patches\MessageHook.h"

#include "PlayerEvents\PlayerEvents.h"

// ----------------------------------------------------------------------------

Export char MapName[]			= "fraxsurv.map";
Export char LevelDesc[]		    = "5P, SRV, 'Fractured Alliance'";
Export char TechtreeName[]		= "survtech.txt";
Export AIModDesc DescBlock	    = { MultiSpaceRace, 5, 12, 0 };

Export int DescBlockEx[] = { 1, 0, 0, 0, 0, 0, 0, 0 }; // Add one extra player slot for AI

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{		
		hInstDLL = hinstDLL; // Save the DLL's base address (for cutscene playback)

		if (HFLInit() == HFLCANTINIT
			|| !SetTurretAnimationHook()
			|| !RandomRes::SetBeaconHook()
			|| !SetBlightHook(true)
			|| !SetMissionEndHook()
			//|| !SetMessageHook(true)
			)
			return FALSE;

		// Super speed hack
		//OP2PatchManager::Patch<unsigned char>((void*)0x49C374, 0xEB, 0x73, false);
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{
		LockWindowUpdate(NULL); // Unlock Dans_RULE_UIFrame (locked by PlayAvi)

		SetBlightHook(false);
		OP2PatchManager::DoUnpatchAll(true);

		HFLCleanup();

		TethysGame::SetMicrobeSpreadSpeed(0);
	}

	

    return TRUE;
}

// -----------------------------------------------------------------------

/*
struct EvacTrans
{
	Unit id;

	int numK;		// # of children
	int numW;		// # of workers
	int numS;		// # of SOLGRS -- I mean, scientists

	EvacTrans *next,
			  *prev;
};

Trigger PlayerCCTriggers[5];
EvacTrans *head = NULL,
          *tail = NULL;
*/

FraxSurvAI fxsAI;

Export int InitProc()
{
#ifdef _DEBUG
	((OP2Game*)gameObj)->gameStartInfo.startupFlags.moraleOn = true;
	((OP2Game*)gameObj)->gameStartInfo.startupFlags.dayAndNightOn = true;
	((OP2Game*)gameObj)->gameStartInfo.startupFlags.diastersOn = true;
	((OP2Game*)gameObj)->gameStartInfo.startupFlags.initialVehicles = 12;
	Player[0].GoEden();
	Player[0].SetColorNumber(1);
	Player[1].SetColorNumber(3);
#endif

    // Initialization stuff goes in InitProc.  Mind-blowing.
    DoSurvSetup();      // Handles morale settings and the playlist
    SetupInitRes();     // Sets up player's initial resources
    SetupVictory();     // Creates victory and defeat conditions
    SetAllLava();       // Set dark gray rock to allow lava to spread onto it
	SetupDisasters();   // Add a few disasters, just to shake things up (like earthquakes!  get it?  ahahahahaha...)

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

	SetupMines();       // Place random mines (needs to be done after other things have been placed)
	SetupDIRTRepair();
	
	// Force RCC effect
	((OP2Game*)gameObj)->forceRCC = 1;
	//CreateTimeTrigger(1, 1, 12000, "UnforceRCC");

	// Infected vehicle stuff
	CreateTimeTrigger(1, 0, 10 * 4, "FindInfectedTankTargets");
	CreateTimeTrigger(1, 0, 2200, 4600, "SpawnInfectedLynx");

	// Force Eden effect
	CreateTimeTrigger(1, 0, 10 * 5, "PlymzToEden");

	return 1; // return 1 if OK; 0 on failure

}

// ----------------------------------------------------------------------------

Export void UpdateCycle()
{
	fxsAI.UpdateCycle();
}

Export void AIProc()
{
    for (int i = 0; i < TethysGame::NoPlayers(); i ++)
	{
		BlightHook_InfectionProc(i);
		RandomRes::LocateBeaconProc(i);
	}
}

void __cdecl GetSaveRegions(struct BufferDesc &bufDesc)
{
	bufDesc.bufferStart = 0;	// Pointer to a buffer that needs to be saved
	bufDesc.length = 0;			// sizeof(buffer)
}

Export void None()
{
}

Export void UnforceRCC()
{
	((OP2Game*)gameObj)->forceRCC = 0;
}

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

// -------------------------------------------------------------

