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

// Global Variables
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

// Used to reference the triggers that mark the tutorial objectives complete.
Trigger Tutorial_AllPlayersHaveCCs,
        Tutorial_CheckDone,
		Tutorial_DummyObjective;

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

// Dummy objectives to give new players a basic rundown of Survivor's special rules.
// Also sets up defeat conditions.
void SetupTutorial()
{
	if (DEBUG_MODE == false)
	{
		Trigger Defeat;

		// Mark the tutorial objectives as complete once all players have operational CCs, or if 100 marks have passed.
		// Fail the mission when there are no human-owned Command Centers (deployed or in ConVecs).
		Trigger FirstCC[5], NoCCs[5], NoConVecs[5];
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
			FirstCC[4] = CreateOperationalTrigger(1, 1, 4, mapCommandCenter, 1, cmpGreaterEqual, "None");
			NoCCs[4] = CreateCountTrigger(1, 0, 4, mapCommandCenter, mapNone, 0, cmpLowerEqual, "None");
			NoConVecs[4] = CreateCountTrigger(1, 0, 4, mapConVec, mapCommandCenter, 0, cmpLowerEqual, "None");
		case 4:
			FirstCC[3] = CreateOperationalTrigger(1, 1, 3, mapCommandCenter, 1, cmpGreaterEqual, "None");
			NoCCs[3] = CreateCountTrigger(1, 0, 3, mapCommandCenter, mapNone, 0, cmpLowerEqual, "None");
			NoConVecs[3] = CreateCountTrigger(1, 0, 3, mapConVec, mapCommandCenter, 0, cmpLowerEqual, "None");
		case 3:
			FirstCC[2] = CreateOperationalTrigger(1, 1, 2, mapCommandCenter, 1, cmpGreaterEqual, "None");
			NoCCs[2] = CreateCountTrigger(1, 0, 2, mapCommandCenter, mapNone, 0, cmpLowerEqual, "None");
			NoConVecs[2] = CreateCountTrigger(1, 0, 2, mapConVec, mapCommandCenter, 0, cmpLowerEqual, "None");
		case 2:
			FirstCC[1] = CreateOperationalTrigger(1, 1, 1, mapCommandCenter, 1, cmpGreaterEqual, "None");
			NoCCs[1] = CreateCountTrigger(1, 0, 1, mapCommandCenter, mapNone, 0, cmpLowerEqual, "None");
			NoConVecs[1] = CreateCountTrigger(1, 0, 1, mapConVec, mapCommandCenter, 0, cmpLowerEqual, "None");
		case 1:
			FirstCC[0] = CreateOperationalTrigger(1, 1, 0, mapCommandCenter, 1, cmpGreaterEqual, "None");
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
			Tutorial_AllPlayersHaveCCs = CreateSetTrigger(1, 0, 5, 5, "None", FirstCC[0], FirstCC[1], FirstCC[2], FirstCC[3], FirstCC[4]);
			Defeat = CreateSetTrigger(1, 0, 10, 10, "None", NoCCs[0], NoCCs[1], NoCCs[2], NoCCs[3], NoCCs[4],
															NoConVecs[0], NoConVecs[1], NoConVecs[2], NoConVecs[3], NoConVecs[4]);
			break;
		case 4:
			Tutorial_AllPlayersHaveCCs = CreateSetTrigger(1, 0, 4, 4, "None", FirstCC[0], FirstCC[1], FirstCC[2], FirstCC[3]);
			Defeat = CreateSetTrigger(1, 0, 8, 8, "None", NoCCs[0], NoCCs[1], NoCCs[2], NoCCs[3],
														  NoConVecs[0], NoConVecs[1], NoConVecs[2], NoConVecs[3]);
			break;
		case 3:
			Tutorial_AllPlayersHaveCCs = CreateSetTrigger(1, 0, 3, 3, "None", FirstCC[0], FirstCC[1], FirstCC[2]);
			Defeat = CreateSetTrigger(1, 0, 6, 6, "None", NoCCs[0], NoCCs[1], NoCCs[2],
														  NoConVecs[0], NoConVecs[1], NoConVecs[2]);
			break;
		case 2:
			Tutorial_AllPlayersHaveCCs = CreateSetTrigger(1, 0, 2, 2, "None", FirstCC[0], FirstCC[1]);
			Defeat = CreateSetTrigger(1, 0, 4, 4, "None", NoCCs[0], NoCCs[1],
														  NoConVecs[0], NoConVecs[1]);
			break;
		case 1:
			Tutorial_AllPlayersHaveCCs = FirstCC[0];
			Defeat = CreateSetTrigger(1, 0, 2, 2, "None", NoCCs[0],
														  NoConVecs[0]);
			break;
		}

		Tutorial_CheckDone = CreateTimeTrigger(1, 0, 16, "SetupRealObjectives");
		Tutorial_DummyObjective = CreateTimeTrigger(0, 1, 1, "None");
		CreateVictoryCondition(1, 0, Tutorial_DummyObjective, "Use Scouts to reveal mining beacons.");
		if (Player[TethysGame::LocalPlayer()].Difficulty() != 0)
		{
			CreateVictoryCondition(1, 0, Tutorial_DummyObjective, "One Evacuation Transport holds 25 colonists.");
			CreateVictoryCondition(1, 0, Tutorial_DummyObjective, "Future evacuations will require enough Evacuation Transports and food loaded in Cargo Trucks for your population.");
		}
		CreateVictoryCondition(1, 0, Tutorial_DummyObjective, "Escape from the Blight and establish a new temporary colony.");

		CreateFailureCondition(1, 0, Defeat, "No human players with CCs or ConVecs loaded with CC kits.");
	}
}

Export void SetupRealObjectives()
{
	if (TethysGame::Tick() >= 10000 || Tutorial_AllPlayersHaveCCs.HasFired(TethysGame::LocalPlayer()))
	{
		TethysGame::AddMessage(-1, -1, "New objective.", -1, sndSavant30);
		SetupVictory();
		Tutorial_DummyObjective.Enable();
		Tutorial_AllPlayersHaveCCs.Destroy();
		Tutorial_CheckDone.Destroy();
	}
}

// Setup actual victory conditions
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
                EvacMod;

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

void SetupWreckage()
{
    // You can never get up here.
    TethysGame::CreateWreck(58+31, 247-1, (map_id)9706,  0);

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
		SpawnPlayer(i[0], 58, 240);
    }

    if (Player[i[1]].IsHuman() || DEBUG_MODE == true )
    {
		SpawnPlayer(i[1], 66, 234);
    }

    if (Player[i[2]].IsHuman() || DEBUG_MODE == true )
    {
		SpawnPlayer(i[2], 75, 230);
    }

    if (Player[i[3]].IsHuman() || DEBUG_MODE == true )
    {
		SpawnPlayer(i[3], 82, 240);
    }

    if (Player[i[4]].IsHuman() || DEBUG_MODE == true )
    {
		SpawnPlayer(i[4], 86, 246);
    }

}   // end DoRandomBases

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

// Easter egg for somehow retrieving the inaccessible wreckage.
Export void v2()
{
	short i, x;

	for (i = 0; i < TethysGame::NoPlayers(); i++)
	{
		// Find out which player actually researched the tech
		if (Player[i].HasTechnology(9706))
		{
			x = i;

			// Send them a message
			TethysGame::AddMessage(-1, -1, "I'm really proud of the work I did on this mission!", x, sndLab_1);
		}
	}
}
