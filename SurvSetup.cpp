// This file contains all functions Survivor uses to initialize the game
// This includes disaster setup, game settings, player bases, etc.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Outpost2DLL.h>
#include <OP2Helper.h>
#include "functions.h"

#include "Patches\ExtLocation.h"
#include "Patches\RandomResources.h"

// Used below to randomize bases
#define numof(array) (sizeof(array)/sizeof(array[0]))
#define autosize(array) numof(array), array

// Global Variables and stuff
bool DEBUG_MODE = false;         // enable for testing purposes

// We're going to reuse these triggers a couple of times over the course of the game.
Trigger QuakeEnabler,
        StormEnabler,
        VortexEnabler,
        MeteorEnabler,
        Quakes,
        Storms,
        Vortexes,
        Meteors,
        Volcano,
        LavaAnimTimer,
        Owned;

// Blight-Related Triggers
Trigger BlightSpawn,
        BlightRate1,
        BlightRate2,
        BlightRate3,
        BlightRate4,
		BlightRate5,
		BlightRate6;

// -----------------------------------------------------------------------
// Pre-Game Setup
// -----------------------------------------------------------------------

// Initialize Morale, Day/night, Music
void DoSurvSetup()
{
	// Setup morale
	TethysGame::ForceMoraleGood(-1);
	if (TethysGame::UsesMorale() && DEBUG_MODE == false)
	{
	    CreateTimeTrigger(1, 1, 30000, "UnsteadyMorale");
	}

	// Force the day/night cycle.
    TethysGame::SetDaylightEverywhere(0);
    GameMap::SetInitialLightLevel(64);
    TethysGame::SetDaylightMoves(1);

	// Next, we'll setup the in-game music
	SongIds SurvSongs[] = { songStatic02, songStatic04, songEden32, songEden21, songEP41, songEP63, songPlymth12, songEP61 };
	TethysGame::SetMusicPlayList(8, 1, SurvSongs);

	// Set the volcano here
	LavaAnimTimer = CreateTimeTrigger( 1, 1, 22000, "PrepVolcanoes");

} // end DoSurvSetup

// 300 marks of steady morale so the players can evacuate without worrying about morale.
Export void UnsteadyMorale()
{
    TethysGame::FreeMoraleLevel(-1);

}

// Setup Victory and Defeat Conditions
void SetupVictory()
{
    if (DEBUG_MODE == false)
    {
        Trigger Skydock,
                IonDriv,
                FusDriv,
                FuelSys,
                CommMod,
                HabRing,
                SnsPack,
                StasSys,
                OrbPack,
                PhnxMod,
                ComCarg,
                RarCarg,
                FodCarg,
                EvacMod,

                Defeat;

        // Evacuation Module
        EvacMod = CreateCountTrigger(1, 1, -1, mapEvacuationModule, mapNone, 1, cmpGreaterEqual, "None");
        CreateVictoryCondition(1, 0, EvacMod, "Evacuate 200 colonists to the starship.");

        // Food Cargo
        FodCarg = CreateCountTrigger(1, 0, -1, mapFoodCargo, mapNone, 1, cmpGreaterEqual, "None");
        CreateVictoryCondition(1, 0, FodCarg, "Evacuate 10000 units of food to the starship.");

        // Rare Metals Cargo
        RarCarg = CreateCountTrigger(1, 0, -1, mapRareMetalsCargo, mapNone, 1, cmpGreaterEqual, "None");
        CreateVictoryCondition(1, 0, RarCarg, "Evacuate 10000 units of Rare Metals to the starship.");

        // Common Metals Cargo
        ComCarg = CreateCountTrigger(1, 0, -1, mapCommonMetalsCargo, mapNone, 1, cmpGreaterEqual, "None");
        CreateVictoryCondition(1, 0, ComCarg, "Evacuate 10000 units of Common Metals to the starship.");

        // Phoenix Module
        PhnxMod = CreateCountTrigger(1, 0, -1, mapPhoenixModule, mapNone, 1, cmpGreaterEqual, "None");
        CreateVictoryCondition(1, 0, PhnxMod, "Launch the Phoenix Module.");

        // Orbital Package
        OrbPack = CreateCountTrigger(1, 0, -1, mapOrbitalPackage, mapNone, 1, cmpGreaterEqual, "None");
        CreateVictoryCondition(1, 0, OrbPack, "Launch the Orbital Package.");

        // Stasis Systems
        StasSys = CreateCountTrigger(1, 0, -1, mapStasisSystems, mapNone, 1, cmpGreaterEqual, "None");
        CreateVictoryCondition(1, 0, StasSys, "Launch the Stasis Systems.");

        // Sensor Package
        SnsPack = CreateCountTrigger(1, 0, -1, mapSensorPackage, mapNone, 1, cmpGreaterEqual, "None");
        CreateVictoryCondition(1, 0, SnsPack, "Launch the Sensor Package.");

        // Habitat Ring
        HabRing = CreateCountTrigger(1, 0, -1, mapHabitatRing, mapNone, 1, cmpGreaterEqual, "None");
        CreateVictoryCondition(1, 0, HabRing, "Launch the Habitat Ring.");

        // Command Module
        CommMod = CreateCountTrigger(1, 0, -1, mapCommandModule, mapNone, 1, cmpGreaterEqual, "None");
        CreateVictoryCondition(1, 0, CommMod, "Launch the Command Module.");

        // Fueling Systems
        FuelSys = CreateCountTrigger(1, 0, -1, mapFuelingSystems, mapNone, 1, cmpGreaterEqual, "None");
        CreateVictoryCondition(1, 0, FuelSys, "Launch the Fueling Systems.");

        // Fusion Drive
        FusDriv = CreateCountTrigger(1, 0, -1, mapFusionDriveModule, mapNone, 1, cmpGreaterEqual, "None");
        CreateVictoryCondition(1, 0, FusDriv, "Launch the Fusion Drive Module.");

        // Ion Drive
        IonDriv = CreateCountTrigger(1, 0, -1, mapIonDriveModule, mapNone, 1, cmpGreaterEqual, "None");
        CreateVictoryCondition(1, 0, IonDriv, "Launch the Ion Drive Module.");

        // Skydock
        Skydock = CreateCountTrigger(1, 0, -1, mapSkydock, mapNone, 1, cmpGreaterEqual, "None");
        CreateVictoryCondition(1, 0, Skydock, "Place the Skydock in orbit.");

		Trigger NoCCs[5], NoConVecs[5];
		int ai = -1;
		for (int i = 0; i < TethysGame::NoPlayers(); i++)
		{
			if (!Player[i].IsHuman() && ai < 0)
		    {
		        ai = i;
			}
		}
	
		switch (ai)
		{
			case 5:
				NoCCs[4] = CreateCountTrigger(1, 0, 4, mapCommandCenter, mapNone, 0, cmpLowerEqual, "None");
				NoConVecs[4] = CreateCountTrigger(1, 0, 4, mapConVec, mapCommandCenter, 0, cmpLowerEqual, "None");
			case 4:
				NoCCs[3] = CreateCountTrigger(1, 0, 3, mapCommandCenter, mapNone, 0, cmpLowerEqual, "None");
				NoConVecs[3] = CreateCountTrigger(1, 0, 3, mapConVec, mapCommandCenter, 0, cmpLowerEqual, "None");
			case 3:
				NoCCs[2] = CreateCountTrigger(1, 0, 2, mapCommandCenter, mapNone, 0, cmpLowerEqual, "None");
				NoConVecs[2] = CreateCountTrigger(1, 0, 2, mapConVec, mapCommandCenter, 0, cmpLowerEqual, "None");
			case 2:
				NoCCs[1] = CreateCountTrigger(1, 0, 1, mapCommandCenter, mapNone, 0, cmpLowerEqual, "None");
				NoConVecs[1] = CreateCountTrigger(1, 0, 1, mapConVec, mapCommandCenter, 0, cmpLowerEqual, "None");
			case 1:
				NoCCs[0] = CreateCountTrigger(1, 0, 0, mapCommandCenter, mapNone, 0, cmpLowerEqual, "None");
				NoConVecs[0] = CreateCountTrigger(1, 0, 0, mapConVec, mapCommandCenter, 0, cmpLowerEqual, "None");
				break;
			default:
				TethysGame::AddMessage(-1, -1, "Error determining # of players", -1, -1);
				break;
		}

		switch (ai)
		{
			case 5:
				Defeat = CreateSetTrigger(1, 0, 10, 10, "None", NoCCs[0], NoCCs[1], NoCCs[2], NoCCs[3], NoCCs[4],
					                                            NoConVecs[0], NoConVecs[1], NoConVecs[2], NoConVecs[3], NoConVecs[4]);
				break;
			case 4:
				Defeat = CreateSetTrigger(1, 0, 8, 8, "None", NoCCs[0], NoCCs[1], NoCCs[2], NoCCs[3],
					                                          NoConVecs[0], NoConVecs[1], NoConVecs[2], NoConVecs[3]);
				break;
			case 3:
				Defeat = CreateSetTrigger(1, 0, 6, 6, "None", NoCCs[0], NoCCs[1], NoCCs[2],
					                                          NoConVecs[0], NoConVecs[1], NoConVecs[2]);
				break;
			case 2:
				Defeat = CreateSetTrigger(1, 0, 4, 4, "None", NoCCs[0], NoCCs[1],
					                                          NoConVecs[0], NoConVecs[1]);
				break;
			case 1:
				Defeat = CreateSetTrigger(1, 0, 2, 2, "None", NoCCs[0],
					                                          NoConVecs[0]);
				break;
		}

		CreateFailureCondition(1, 0, Defeat, "No human players with CCs or ConVecs loaded with CC kits.");
    }
}

// Set all S1 and S2 tiles to lava-possible (copied from the old wiki)
void SetAllLava()
{
  int x,y,tile;
  LOCATION mapsize(512,512);                         // create a point wayoff the map

  mapsize.Clip();                                      // clip it, so we have the mapsize

  for (y=0; y<mapsize.y; ++y)                          // run through all rows
  {
    for (x=0; x<mapsize.x; ++x)                    // check every tile on each row
    {
      //tile=GameMap::GetTile(LOCATION(x,y));            // get the tile on that position
      //if ( ((tile>=0x1C7) && (tile<=0x1F8))            // is this one a lavarock...
      //  || ((tile>=0x22F) && (tile<=0x25E)) )          // ...or an edge ?
	  tile = GameMap::GetCellType(LOCATION(x,y));
	  if (tile == cellSlowPassible1 ||
		  tile == cellSlowPassible2)
      {
        GameMap::SetLavaPossible(LOCATION(x,y),TRUE);  // -> then set it to LavaPossible
      }

    }
  }

} // end SetAllLava

// Set Initial Resources For All Players
void SetupInitRes()
{
    short i;
    for (i = 0; i < TethysGame::NoPlayers(); i++)
    {
        switch (Player[i].Difficulty() )
        {
            case 0:
                Player[i].SetKids(22);
                Player[i].SetWorkers(32);
                Player[i].SetScientists(21);
                Player[i].SetFoodStored(3600);
                Player[i].MarkResearchComplete(2702);   // Boptronics
                Player[i].MarkResearchComplete(2705);   // Social Sciences
                Player[i].MarkResearchComplete(3304);   // Offspring Enhancement
				Player[i].MarkResearchComplete(3401);   // Cybernetic Teleoperation
                Player[i].MarkResearchComplete(3407);   // Large-Scale Optical Resonators
                Player[i].MarkResearchComplete(3408);   // Focused Microwave Projection
                break;

            case 1:
                Player[i].SetKids(18);
                Player[i].SetWorkers(28);
                Player[i].SetScientists(19);
                Player[i].SetFoodStored(2400);
                Player[i].MarkResearchComplete(2701);   // Astronomy
				Player[i].MarkResearchComplete(3304);   // Offspring Enhancement
                Player[i].MarkResearchComplete(3407);   // Large-Scale Optical Resonators
                Player[i].MarkResearchComplete(3408);   // Focused Microwave Projection
                break;

            case 2:
                Player[i].SetKids(14);
                Player[i].SetWorkers(24);
                Player[i].SetScientists(15);
                Player[i].SetFoodStored(2000);
                Player[i].MarkResearchComplete(3407);   // Large-Scale Optical Resonators
                Player[i].MarkResearchComplete(3408);   // Focused Microwave Projection
                break;
        }

        if (DEBUG_MODE == true)
        {
            Player[i].SetTechLevel(12);
            if (i == 0)
            {
                TethysGame::ForceMoraleGreat(-1);
            }
        }

    }

}   // end SetupInitRes

void SetupWreckage()
{
    // You can never get up here.
    TethysGame::CreateWreck(58+31, 247-1, (map_id)9706,  0);       // WTF

    if (DEBUG_MODE == true)
    {
        Unit Unit1;
        TethysGame::CreateUnit(Unit1, mapCommandCenter, LOCATION(121+31, 31-1), 0, mapNone, 0);
		TethysGame::CreateUnit(Unit1, mapAgridome, LOCATION(121+31, 28-1), 0, mapNone, 0);
		TethysGame::CreateUnit(Unit1, mapAgridome, LOCATION(121+31, 25-1), 0, mapNone, 0);
		TethysGame::CreateUnit(Unit1, mapAgridome, LOCATION(125+31, 28-1), 0, mapNone, 0);
		TethysGame::CreateUnit(Unit1, mapAgridome, LOCATION(125+31, 25-1), 0, mapNone, 0);
		TethysGame::CreateUnit(Unit1, mapNursery, LOCATION(118+31, 31-1), 0, mapNone, 0);
		TethysGame::CreateUnit(Unit1, mapUniversity, LOCATION(118+31, 28-1), 0, mapNone, 0);
		TethysGame::CreateUnit(Unit1, mapGeothermalPlant, LOCATION(126+31, 31-1), 0, mapNone, 0);
        TethysGame::CreateUnit(Unit1, mapSpaceport, LOCATION(121+31, 35-1), 0, mapNone, 0);
            Unit1.SetFactoryCargo(0, mapRLV, mapNone);
            Unit1.SetFactoryCargo(1, mapEDWARDSatellite, mapNone);
    }

}   // end SetupWreckage

// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Mining Beacon Setup
// -----------------------------------------------------------------------

void CreateRes(RandomRes & res) // Helper function
{
	if (res.requireSmelterSpace == 0)
	{
		res.requireSmelterSpace = 2;
		res.noSmelterOverlap = true;
		res.maxSmelterSpacing = 1;
	}
	//res.scoutsFindBeacons = true;
	res.allowLavaPossible = false;

	res.Create();
}

void SetupMines()
{
	RandomRes bottom = RandomRes(MAP_RECT(MapLocation(1,176),MapLocation(128,215)), 16, 5, 2, 1, 8);
	//bottom.SetToIncludedArea(MAP_RECT(MapLocation(1,206),MapLocation(69,255)),0); // Exclude the very bottom left corner close to the Blight
	//bottom.SetToIncludedArea(MAP_RECT(MapLocation(53,225),MapLocation(100,255)),0); // Exclude the bottom middle area close to the Blight
	bottom.SetWeights(0, 0, 999, 2, 1, 0);
	bottom.scoutsFindBeacons = false;
	CreateRes(bottom);
	
	RandomRes middleBot = RandomRes(MAP_RECT(MapLocation(1,92),MapLocation(128,141)), 10, 5, 3, 3, 8);
	middleBot.SetWeights(1, 4, 2, 2, 4, 3);
	middleBot.scoutsFindBeacons = !DEBUG_MODE;
	//middleBot.scoutsFindBeacons = false;
	CreateRes(middleBot);
	
	RandomRes middleTop = RandomRes(MAP_RECT(MapLocation(1,40),MapLocation(128,91)), 14, 9, 3, 3, 8);
	middleTop.SetWeights(3, 4, 2, 4, 5, 3);
	middleTop.scoutsFindBeacons = !DEBUG_MODE;
	//middleTop.scoutsFindBeacons = false;
	CreateRes(middleTop);
	
	RandomRes top = RandomRes(MAP_RECT(MapLocation(1,1),MapLocation(128,39)), 4, 3, 2, 1, 8);
	top.SetToIncludedArea(MAP_RECT(MapLocation(41,1),MapLocation(100,35)),0);
	top.SetToIncludedArea(MAP_RECT(MapLocation(101,1),MapLocation(128,21)),0);
	top.SetToIncludedArea(MAP_RECT(MapLocation(16,1),MapLocation(40,7)),0);
	top.SetToIncludedArea(MAP_RECT(MapLocation(25,6),MapLocation(47,29)),0);
	top.scoutsFindBeacons = !DEBUG_MODE;
	//top.scoutsFindBeacons = false;
	top.SetWeights(1, 1, 5, 1, 3, 4);
	CreateRes(top);

	// I want this one here, always.  It's kind of a trap but you actually can survive there for one night.
	// Think of it as a "challenge mode" run. (no longer releveant since the Blight start moved)
	//TethysGame::CreateBeacon(mapMiningBeacon,   43+31, 225-1, 0,  0,  0);

}   // end SetupMines


// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Base Setup - Randomizes starting locations, creates player bases
// -----------------------------------------------------------------------

// Randomize Player Starting Locations
void DoRandomBases()
{
    // Randomized Base code.
    int i[5] = { 0, 1, 2, 3, 4 };

    if (DEBUG_MODE == false)
    {
        RandomizeList(autosize(i) );
    }

    // Create bases - check to make sure owner is active first!
    if (Player[i[0]].IsHuman() || DEBUG_MODE == true )
    {
		SetupBase(i[0], 58, 240);
		//SetupBase(i[0], 76, 69);		// Debug start.
    }

    if (Player[i[1]].IsHuman() || DEBUG_MODE == true )
    {
		SetupBase(i[1], 66, 234);
		//SetupBase(i[1], 49, 49);		// Debug start.
    }

    if (Player[i[2]].IsHuman() || DEBUG_MODE == true )
    {
        SetupBase(i[2], 75, 230);
		//SetupBase(i[2], 86, 80);		// Debug start.
    }

    if (Player[i[3]].IsHuman() || DEBUG_MODE == true )
    {
        SetupBase(i[3], 82, 240);
		//SetupBase(i[3], 103, 94);		// Debug start.
    }

    if (Player[i[4]].IsHuman() || DEBUG_MODE == true )
    {
        SetupBase(i[4], 86, 246);
		//SetupBase(i[4], 118, 62);		// Debug start.
    }

}   // end DoRandomBases

void SetupBase(int i, int x, int y)
{
    Unit Unit1;
    Player[i].CenterViewOn(x+31, y-1);
	
    // Land Rush style start.  Resource setting determines initial units.
    if (Player[i].Difficulty() == 0)
    {
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(x+31, y-1), i, mapCommandCenter, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+1)+31, y-1), i, mapStructureFactory, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+2)+31, y-1), i, mapCommonOreSmelter, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION((x+3)+31, y-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
            Unit1.SetTruckCargo(truckCommonMetal, 1000);
        TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION((x+4)+31, y-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
            Unit1.SetTruckCargo(truckCommonMetal, 1000);
        TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION((x+5)+31, y-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
            Unit1.SetTruckCargo(truckCommonMetal, 1000);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(x+31, (y+1)-1), i, mapTokamak, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+1)+31, (y+1)-1), i, mapAgridome, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+2)+31, (y+1)-1), i, mapBasicLab, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapRoboMiner, LOCATION((x+3)+31, (y+1)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapRoboSurveyor, LOCATION((x+4)+31, (y+1)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+5)+31, (y+1)-1), i, mapCommandCenter, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(x+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
            Unit1.SetTruckCargo(truckFood, 1800);
        TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION((x+1)+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
            Unit1.SetTruckCargo(truckFood, 1800);
        TethysGame::CreateUnit(Unit1, mapEvacuationTransport, LOCATION((x+2)+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapEvacuationTransport, LOCATION((x+3)+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapEvacuationTransport, LOCATION((x+4)+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapEarthworker, LOCATION((x+5)+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
    }

    else if (Player[i].Difficulty() == 1)
    {
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(x+31, y-1), i, mapCommandCenter, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+1)+31, y-1), i, mapStructureFactory, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+2)+31, y-1), i, mapCommonOreSmelter, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION((x+3)+31, y-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
            Unit1.SetTruckCargo(truckCommonMetal, 1000);
        TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION((x+4)+31, y-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
            Unit1.SetTruckCargo(truckCommonMetal, 1000);
        TethysGame::CreateUnit(Unit1, mapEarthworker, LOCATION((x+5)+31, y-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(x+31, (y+1)-1), i, mapTokamak, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+1)+31, (y+1)-1), i, mapAgridome, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+2)+31, (y+1)-1), i, mapBasicLab, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapRoboMiner, LOCATION((x+3)+31, (y+1)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapRoboSurveyor, LOCATION((x+4)+31, (y+1)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(x+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
            Unit1.SetTruckCargo(truckFood, 1400);
        TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION((x+1)+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
            Unit1.SetTruckCargo(truckFood, 1400);
        TethysGame::CreateUnit(Unit1, mapEvacuationTransport, LOCATION((x+2)+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapEvacuationTransport, LOCATION((x+3)+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapEvacuationTransport, LOCATION((x+4)+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
    }

    else if (Player[i].Difficulty() == 2)
    {
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(x+31, y-1), i, mapCommandCenter, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+1)+31, y-1), i, mapStructureFactory, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+2)+31, y-1), i, mapCommonOreSmelter, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION((x+3)+31, y-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
            Unit1.SetTruckCargo(truckCommonMetal, 1000);
        TethysGame::CreateUnit(Unit1, mapEarthworker, LOCATION((x+4)+31, y-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(x+31, (y+1)-1), i, mapTokamak, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+1)+31, (y+1)-1), i, mapAgridome, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+2)+31, (y+1)-1), i, mapBasicLab, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapRoboMiner, LOCATION((x+3)+31, (y+1)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapRoboSurveyor, LOCATION((x+4)+31, (y+1)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(x+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
            Unit1.SetTruckCargo(truckFood, 2000);
        TethysGame::CreateUnit(Unit1, mapEvacuationTransport, LOCATION((x+1)+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapEvacuationTransport, LOCATION((x+2)+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        TethysGame::CreateUnit(Unit1, mapEvacuationTransport, LOCATION((x+3)+31, (y+2)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
    }

    // Initial Vehicles
    map_id ivWpn = mapMicrowave;
    if (Player[i].IsEden() )
    {
        ivWpn = mapLaser;
    }

    switch (TethysGame::InitialUnits() )
    {
        case 12:
          TethysGame::CreateUnit(Unit1, mapScout, LOCATION((x+5)+31, (y+4)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        case 11:
          TethysGame::CreateUnit(Unit1, mapRoboDozer, LOCATION((x+4)+31, (y+4)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        case 10:
          TethysGame::CreateUnit(Unit1, mapEarthworker, LOCATION((x+3)+31, (y+4)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        case 9:
          TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+2)+31, (y+4)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
			Unit1.SetCargo(mapGuardPost, ivWpn);
        case 8:
          TethysGame::CreateUnit(Unit1, mapConVec, LOCATION((x+1)+31, (y+4)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
			Unit1.SetCargo(mapGuardPost, ivWpn);
        case 7:
          TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(x+31, (y+4)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
			Unit1.SetCargo(mapGuardPost, ivWpn);
        case 6:
          TethysGame::CreateUnit(Unit1, mapRoboSurveyor, LOCATION((x+5)+31, (y+3)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        case 5:
          TethysGame::CreateUnit(Unit1, mapRoboMiner, LOCATION((x+4)+31, (y+3)-1), i, mapNone, 6);
            Unit1.DoSetLights(1);
        case 4:
          TethysGame::CreateUnit(Unit1, mapLynx, LOCATION((x+3)+31, (y+3)-1), i, ivWpn, 6);
            Unit1.DoSetLights(1);
        case 3:
          TethysGame::CreateUnit(Unit1, mapLynx, LOCATION((x+2)+31, (y+3)-1), i, ivWpn, 6);
            Unit1.DoSetLights(1);
        case 2:
          TethysGame::CreateUnit(Unit1, mapLynx, LOCATION((x+1)+31, (y+3)-1), i, ivWpn, 6);
            Unit1.DoSetLights(1);
        case 1:
          TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(x+31, (y+3)-1), i, ivWpn, 6);
            Unit1.DoSetLights(1);
        default:
          // No initial vehicles.
          break;
    }

}   // end SetupBase

// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
//  Disasters
// -----------------------------------------------------------------------

// Functions for setting up volcano animations
void SouthEastFlowAni ( int x, int y)
{
	GameMap::SetTile(LOCATION(x, y), 0x489);
	GameMap::SetTile(LOCATION(x, y+1), 0x4A0);
	GameMap::SetTile(LOCATION(x+1, y+1), 0x4AB);
	GameMap::SetTile(LOCATION(x+1, y), 0x494);
}
void SouthFlowAni( int x, int y)
{
	GameMap::SetTile(LOCATION(x, y), 0x474);
	GameMap::SetTile(LOCATION(x, y+1), 0x47E);
}

void SouthWestFlowAni ( int x, int y)
{
	GameMap::SetTile(LOCATION(x+1, y), 0x453);
	GameMap::SetTile(LOCATION(x, y), 0x447);
	GameMap::SetTile(LOCATION(x, y+1), 0x45E);
	GameMap::SetTile(LOCATION(x+1, y+1), 0x469);
}

// Functions for stopping volcano animations
void SouthEastFlowStopAni ( int x, int y)
{
	GameMap::SetTile(LOCATION(x, y), 0x490);
	GameMap::SetTile(LOCATION(x, y+1), 0x4A8);
	GameMap::SetTile(LOCATION(x+1, y+1), 0x4B2);
	GameMap::SetTile(LOCATION(x+1, y), 0x49C);
}
void SouthFlowStopAni( int x, int y)
{
	GameMap::SetTile(LOCATION(x, y), 0x47B);
	GameMap::SetTile(LOCATION(x, y+1), 0x486);
}

void SouthWestFlowStopAni ( int x, int y)
{
	GameMap::SetTile(LOCATION(x+1, y), 0x45A);
	GameMap::SetTile(LOCATION(x, y), 0x44F);
	GameMap::SetTile(LOCATION(x, y+1), 0x465);
	GameMap::SetTile(LOCATION(x+1, y+1), 0x470);
}

void SetupDisasters()
{
    QuakeEnabler  = CreateTimeTrigger( 1, 1,  8000, "EnableQuakes1");
    StormEnabler  = CreateTimeTrigger( 1, 1, 46200, "EnableStorms1");
    //VortexEnabler = CreateTimeTrigger( 1, 1, 19400, "EnableVortex1");
    MeteorEnabler = CreateTimeTrigger( 1, 1,  26100, "EnableMeteor1");
    //LavaAnimTimer = CreateTimeTrigger( 1, 1, 10, "PrepVolcanoes");
}

Export void EnableQuakes1()
{
    Quakes = CreateTimeTrigger( 1, 0, 4830, 5410, "Quakes1");
    QuakeEnabler.Destroy();
    QuakeEnabler = CreateTimeTrigger( 1, 1, 32800, "EnableQuakes2");
}

Export void EnableStorms1()
{
    Storms = CreateTimeTrigger( 1, 0, 5300, 6900, "Storms1");
    StormEnabler.Destroy();
    StormEnabler = CreateTimeTrigger( 1, 1, 35200, "EnableStorms2");
}

Export void EnableVortex1()
{
    Vortexes = CreateTimeTrigger( 1, 0, 6200, 8400, "Vortex1");
    VortexEnabler.Destroy();
    VortexEnabler = CreateTimeTrigger( 1, 1, 20300, "EnableVortex2");
}

Export void EnableMeteor1()
{
    Meteors = CreateTimeTrigger( 1, 0, 3900, 6700, "Meteor1");
    MeteorEnabler.Destroy();
    MeteorEnabler = CreateTimeTrigger( 1, 1, 30400, "EnableMeteor2");
}

Export void EnableQuakes2()
{
    Quakes.Destroy();
    Quakes = CreateTimeTrigger( 1, 0, 4900, 5100, "Quakes2");
    QuakeEnabler.Destroy();
	QuakeEnabler = CreateTimeTrigger( 1, 1, 41600, "EnableQuakes3");
}

Export void EnableStorms2()
{
    Storms.Destroy();
    Storms = CreateTimeTrigger( 1, 0, 5503, 7029, "Storms2");
    StormEnabler.Destroy();
}

Export void EnableVortex2()
{
    Vortexes.Destroy();
    Vortexes = CreateTimeTrigger( 1, 0, 5400, 7200, "Vortex2");
    VortexEnabler.Destroy();
}

Export void EnableMeteor2()
{
    Meteors.Destroy();
    Meteors = CreateTimeTrigger( 1, 0, 4100, 6400, "Meteor2");
    MeteorEnabler.Destroy();
}

Export void EnableQuakes3()
{
    Quakes.Destroy();
    Quakes = CreateTimeTrigger( 1, 0, 4900, 5100, "Quakes3");
    QuakeEnabler.Destroy();
}

Export void Quakes1()
{
	TethysGame::SetEarthquake(52 + TethysGame::GetRand(37)+31, 95 + TethysGame::GetRand(74)-1, 1);
}

Export void Storms1()
{
    TethysGame::SetLightning(TethysGame::GetRand(128)+31, 78+TethysGame::GetRand(50)-1, TethysGame::GetRand(8)+6, TethysGame::GetRand(128)+31, 78+TethysGame::GetRand(50)-1 );
}

Export void Vortex1()
{
    TethysGame::SetTornado(TethysGame::GetRand(256)+31, TethysGame::GetRand(256)-1, TethysGame::GetRand(20)+10, TethysGame::GetRand(256)+31, TethysGame::GetRand(256)-1, 0 );
}

Export void Meteor1()
{
    TethysGame::SetMeteor(3+TethysGame::GetRand(121)+31, 13+TethysGame::GetRand(226)-1, 1);
}

Export void Quakes2()
{
    TethysGame::SetEarthquake(52 + TethysGame::GetRand(37)+31, 42 + TethysGame::GetRand(96)-1, TethysGame::GetRand(2)+1);
}

Export void Storms2()
{
    TethysGame::SetLightning(TethysGame::GetRand(128)+31, 78+TethysGame::GetRand(50)-1, TethysGame::GetRand(15)+10, TethysGame::GetRand(128)+31, 78+TethysGame::GetRand(50)-1 );
}

Export void Vortex2()
{
    TethysGame::SetTornado(TethysGame::GetRand(256)+31, TethysGame::GetRand(256)-1, TethysGame::GetRand(35)+20, TethysGame::GetRand(256)+31, TethysGame::GetRand(256)-1, 0 );
}

Export void Meteor2()
{
    TethysGame::SetMeteor(3+TethysGame::GetRand(121)+31, 13+TethysGame::GetRand(121)-1, TethysGame::GetRand(3)+1);
}

Export void Quakes3()
{
    TethysGame::SetEarthquake(52 + TethysGame::GetRand(37)+31, 42 + TethysGame::GetRand(96)-1, TethysGame::GetRand(2)+1);
}

Export void PrepVolcanoes()
{
    SouthFlowAni(47+31, 88-1);
    Volcano = CreateTimeTrigger(1, 1, 2100, "EruptVolcano1");
    LavaAnimTimer.Destroy();
}

Export void EruptVolcano1()
{
    //GameMap::SetLavaPossible(LOCATION(47+31, 89-1), TRUE);
    GameMap::SetLavaPossible(LOCATION(47+31, 90-1), TRUE);
	GameMap::SetLavaPossible(LOCATION(47+31, 91-1), TRUE);

    TethysGame::SetEruption(47+31, 90-1, 110);
    TethysGame::SetLavaSpeed(110);
    LavaAnimTimer = CreateTimeTrigger(1, 1, 1000, "StopLavaAnim1");
    Volcano.Destroy();
}

Export void StopLavaAnim1()
{
    SouthFlowStopAni(47+31, 88-1);
    LavaAnimTimer.Destroy();
}


void SetupBlight()
{
    // Setup the initial Blight
    BlightSpawn = CreateTimeTrigger(1, 1, 2, "Blight");

    // Setup time triggers to change the Blight's spread speed
    BlightRate1 = CreateTimeTrigger( 1, 1,  5000, "BlightSpeed1");
    BlightRate2 = CreateTimeTrigger( 1, 1,  40000, "BlightSpeed2");
    BlightRate3 = CreateTimeTrigger( 1, 1,  55000, "BlightSpeed3");
    BlightRate4 = CreateTimeTrigger( 1, 1,  82000, "BlightSpeed4");
	BlightRate5 = CreateTimeTrigger( 1, 1,  109000, "BlightSpeed5");
	BlightRate6 = CreateTimeTrigger( 1, 1,  150000, "BlightSpeed6");

	// We can't let people getting complacent now, can we?
	CreateTimeTrigger( 1, 1, 178000, "SurpriseBlight2");

	// ???
	Owned = CreateResearchTrigger( 1, 1, 9706, -1, "WTF_Tech");
}

Export void Blight()
{
    // Setup Blight
	GameMap::SetVirusUL(LOCATION(64+31, 255-1), 1);
	GameMap::SetVirusUL(LOCATION(65+31, 255-1), 1);
	GameMap::SetVirusUL(LOCATION(66+31, 255-1), 1);
	GameMap::SetVirusUL(LOCATION(67+31, 255-1), 1);
	GameMap::SetVirusUL(LOCATION(68+31, 255-1), 1);
	GameMap::SetVirusUL(LOCATION(69+31, 255-1), 1);
	GameMap::SetVirusUL(LOCATION(70+31, 255-1), 1);
	GameMap::SetVirusUL(LOCATION(71+31, 255-1), 1);
	GameMap::SetVirusUL(LOCATION(72+31, 255-1), 1);

	GameMap::SetVirusUL(LOCATION(63+31, 256-1), 1);
	GameMap::SetVirusUL(LOCATION(64+31, 256-1), 1);
	GameMap::SetVirusUL(LOCATION(65+31, 256-1), 1);
	GameMap::SetVirusUL(LOCATION(66+31, 256-1), 1);
	GameMap::SetVirusUL(LOCATION(67+31, 256-1), 1);
	GameMap::SetVirusUL(LOCATION(68+31, 256-1), 1);
	GameMap::SetVirusUL(LOCATION(69+31, 256-1), 1);
	GameMap::SetVirusUL(LOCATION(70+31, 256-1), 1);
	GameMap::SetVirusUL(LOCATION(71+31, 256-1), 1);
	GameMap::SetVirusUL(LOCATION(72+31, 256-1), 1);
	GameMap::SetVirusUL(LOCATION(73+31, 256-1), 1);
	TethysGame::SetMicrobeSpreadSpeed(35);

	// Warning Message
	TethysGame::AddMessage((68+31)*32, 8160, "Microbe growth detected!", -1, 152);

    BlightSpawn.Destroy();

}   // end SetupBlight

// -----------------------------------------------------------------------


// -----------------------------------------------------------------------
// Blight Speed Functions
// -----------------------------------------------------------------------

Export void BlightSpeed1()
{
    TethysGame::SetMicrobeSpreadSpeed(16);
    BlightRate1.Destroy();
}

Export void BlightSpeed2()
{
    TethysGame::SetMicrobeSpreadSpeed(21);
    BlightRate2.Destroy();
}

Export void SurpriseBlight2()
{
	// Add some Blight up in here.
	GameMap::SetVirusUL(LOCATION(127+31, 101-1), 1);
	GameMap::SetVirusUL(LOCATION(127+31, 102-1), 1);
	GameMap::SetVirusUL(LOCATION(127+31, 103-1), 1);

	GameMap::SetVirusUL(LOCATION(128+31, 100-1), 1);
	GameMap::SetVirusUL(LOCATION(128+31, 101-1), 1);
	GameMap::SetVirusUL(LOCATION(128+31, 102-1), 1);
	GameMap::SetVirusUL(LOCATION(128+31, 103-1), 1);
	GameMap::SetVirusUL(LOCATION(128+31, 104-1), 1);

	GameMap::SetVirusUL(LOCATION(129+31,  99-1), 1);
	GameMap::SetVirusUL(LOCATION(129+31, 100-1), 1);
	GameMap::SetVirusUL(LOCATION(129+31, 101-1), 1);
	GameMap::SetVirusUL(LOCATION(129+31, 102-1), 1);
	GameMap::SetVirusUL(LOCATION(129+31, 103-1), 1);
	GameMap::SetVirusUL(LOCATION(129+31, 104-1), 1);
	GameMap::SetVirusUL(LOCATION(129+31, 105-1), 1);

	// Warning Message
	TethysGame::AddMessage((128+31)*32, (102-1)*32, "Microbe growth detected!", -1, 152);
}

Export void BlightSpeed3()
{
	TethysGame::SetMicrobeSpreadSpeed(19);    
    BlightRate3.Destroy();
}

Export void BlightSpeed4()
{
    TethysGame::SetMicrobeSpreadSpeed(35);
    BlightRate4.Destroy();
}

Export void BlightSpeed5()
{
    TethysGame::SetMicrobeSpreadSpeed(48);
    BlightRate5.Destroy();
}

Export void BlightSpeed6()
{
    TethysGame::SetMicrobeSpreadSpeed(54);
    BlightRate6.Destroy();
}

// -----------------------------------------------------------------------


// -----------------------------------------------------------------------
// Misc. Functions
// -----------------------------------------------------------------------

// Function to own some poor fool
Export void WTF_Tech()
{
    short i, x;

    for (i = 0; i < TethysGame::NoPlayers(); i++)
    {
        // Find out which player actually researched the tech
        if (Player[i].HasTechnology(9706) )
        {
            x = i;

            // Warn that player of impending DETH
            TethysGame::AddMessage( -1, -1, "You are n00b; your colony is doomed!", x, sndSavnt227);
        }
    }

    // Find all buildings owned by that player and own them
    Unit curUnit;
    PlayerBuildingEnum unitEnum(x, mapNone);
    while (unitEnum.GetNext(curUnit) )
    {
        //curUnit.DoDeath();
        LOCATION curLoc = curUnit.Location();
        short curX = curLoc.x;
        short curY = curLoc.y;

        TethysGame::SetTornado(curX, curY, 0, curX, curY, 1);
    }

    // Vehicles don't get Vortex'd, just killed off
    PlayerVehicleEnum vechEnum(x);
    while (vechEnum.GetNext(curUnit) )
    {
        curUnit.DoSelfDestruct();
    }

	TethysGame::SetMicrobeSpreadSpeed(2000);
    Owned.Destroy();
	CreateTimeTrigger(1, 0, 16, "WTF_Tech_Part2");
}

Export void WTF_Tech_Part2()
{
	TethysGame::SetMicrobeSpreadSpeed(2000);
	GameMap::SetInitialLightLevel(TethysGame::GetRand(128)+32);
}

// end of file
