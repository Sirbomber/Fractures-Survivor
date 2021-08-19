// This file contains definitions for all AI functions related to initialization and setting up and managing the AI's main base (including EMP Missile management)

#pragma once

#include "FraxSurvAi.h"

// ----------------------------------------------------------------------------
// AI Initialization/Maintenance
// ----------------------------------------------------------------------------

void FraxSurvAI::SetupAI(int id)
{
	odprintf("Begin SetupAI\n");


	//char string[50];
	//scr_snprintf(string, 50, "Level handle: %p", GetModuleHandle("mf5_fxs.dll"));
	//odprintf(string);

	// Store AI player number
	aiNum = id;

	// Get human player difficulty level counts
	for (int i = 0; i < TethysGame::NoPlayers(); i++)
	{
		if (Player[i].IsHuman())
		{
			switch (Player[i].Difficulty())
			{
			case 0:
				numHumansEasy++;
				break;
			case 1:
				numHumansNormal++;
				break;
			case 2:
				numHumansHard++;
				break;
			default:
				break;
			}
		}
	}

#ifdef _DEBUG
	numHumansEasy = 0;
	numHumansNormal = 0;
	numHumansHard = 5;
#endif

#ifdef USECOMBATGROUPS
	// If there are more players on hard than on normal and easy, give the AI increased search space
	if (numHumansHard >= numHumansNormal + numHumansEasy)
	{
		CombatGroup::SetMaxSearchRange(24);
	}
	// If all players are on easy, decrease the search space and nerf the AI's ability to micromanage
	else if (numHumansHard == 0 && numHumansNormal == 0)
	{
		CombatGroup::SetMaxSearchRange(10);
		CombatGroup::EnableHandicap();
	}
#endif

	// The AI counts as a player on easy.
	numHumansEasy++;

	// Set resources
	Player[aiNum].SetOre(60000);
	Player[aiNum].SetRareOre(30000);
	Player[aiNum].SetFoodStored(90000);
	Player[aiNum].SetKids(70);
	Player[aiNum].SetWorkers(130);
	Player[aiNum].SetScientists(55);
	Player[aiNum].SetTechLevel(6);
	Player[aiNum].SetSolarSat(4);

	// Setup bases and unit groups
	PrepareUnitGroupsMain();
	SetupMainBase();
	RecordStructuresMain();
	nextAttackTimeMain = 19000 + TethysGame::GetRand(101);

	// Setup the Eden outpost
	PrepareUnitGroupsEden();
	SetupOutpost1();
	nextAttackTimeEden = 22000 + TethysGame::GetRand(101);

	// Setup the Plymouth outpost
	PrepareUnitGroupsPlymouth();
	SetupOutpost2();
	RecordStructuresPlymouth();
	nextAttackTimePlym = 22400 + TethysGame::GetRand(201);

	// Setup AI monitoring functions
	trigUpdateCycle = CreateTimeTrigger(1, 0, 100, "UpdateCycle");
	
	LastMissileTime[0] = SPACEPORT_DELAY;
	LastMissileTime[1] = SPACEPORT_DELAY + 7122;	// Slight delay so both missiles don't fire at the same time.

	odprintf("End SetupAI\n");

}

void FraxSurvAI::UpdateCycle()
{
#ifdef USECOMBATGROUPS
	// Call combat group update functions
	std::vector<CombatGroup>::iterator i = allCombatGroups.begin();
	while (i != allCombatGroups.end())
	{
		if (i->IsEmpty())
		{
			i = allCombatGroups.erase(i);
		}
		else
		{
			i->Update();
			i++;
		}
	}
#endif

	// Check if it's time to launch a new attack wave
	CheckAttack();

	// Repair monitor
	#ifndef NOAIREPAIRS
	Main_Repair();
	#endif

	Eden_Repair();

	// Check Spaceport/EMP Missile status
	if (TethysGame::Tick() > SPACEPORT_DELAY)
	{
		CheckSpaceportStatus();
	}

	// Check lab/research status
	LabStuff();

	// Check if the AI should try to kill everything
	CheckRevenge();
}

// ----------------------------------------------------------------------------
// Base Setup/Management
// ----------------------------------------------------------------------------

void FraxSurvAI::SetupMainBase()
{
	odprintf("Begin SetupMainBase\n");

	// Setup the AI's main base, mining beacons, and stuff like that.
	UnitEx Unit1,
		cSmelter[6],
		cMine[2],
		rSmelter[3],
		rMine;

	TethysGame::CreateUnit(MainCC, mapCommandCenter, LOCATION(70 + 31, 20 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapStructureFactory, LOCATION(71 + 31, 27 - 1), aiNum, mapNone, 0);
	//#ifndef NOAIFACTORIES
	MainSF.TakeUnit(Unit1);
	//#endif
	TethysGame::CreateUnit(cSmelter[0], mapCommonOreSmelter, LOCATION(76 + 31, 23 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(cSmelter[1], mapCommonOreSmelter, LOCATION(76 + 31, 27 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(cSmelter[2], mapCommonOreSmelter, LOCATION(81 + 31, 23 - 1), aiNum, mapNone, 0);
	TethysGame::CreateBeacon(mapMiningBeacon, 81 + 31, 27 - 1, 0, 0, 0);
	TethysGame::CreateUnit(cMine[0], mapCommonOreMine, LOCATION(81 + 31, 27 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapVehicleFactory, LOCATION(71 + 31, 23 - 1), aiNum, mapNone, 0);
#ifndef NOAIFACTORIES
	MainVF[0].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(StdLab, mapStandardLab, LOCATION(70 + 31, 17 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapNursery, LOCATION(74 + 31, 20 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapUniversity, LOCATION(74 + 31, 17 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapAgridome, LOCATION(77 + 31, 20 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapAgridome, LOCATION(81 + 31, 20 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapAgridome, LOCATION(77 + 31, 17 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapAgridome, LOCATION(81 + 31, 17 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapMedicalCenter, LOCATION(67 + 31, 20 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapMedicalCenter, LOCATION(67 + 31, 17 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapVehicleFactory, LOCATION(66 + 31, 23 - 1), aiNum, mapNone, 0);
#ifndef NOAIFACTORIES
	MainVF[1].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapVehicleFactory, LOCATION(61 + 31, 23 - 1), aiNum, mapNone, 0);
#ifndef NOAIFACTORIES
	MainVF[2].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapMedicalCenter, LOCATION(64 + 31, 20 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapMedicalCenter, LOCATION(64 + 31, 17 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapRobotCommand, LOCATION(61 + 31, 20 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapMHDGenerator, LOCATION(79 + 31, 3 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapMHDGenerator, LOCATION(76 + 31, 3 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapMHDGenerator, LOCATION(73 + 31, 3 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapMHDGenerator, LOCATION(70 + 31, 3 - 1), aiNum, mapNone, 0);
#ifndef NOAIREPAIRS
	TethysGame::CreateUnit(Unit1, mapArachnidFactory, LOCATION(61 + 31, 17 - 1), aiNum, mapNone, 0);
	MainRepairs.TakeUnit(Unit1);
#endif
#ifdef NOAIREPAIRS
	TethysGame::CreateUnit(Unit1, mapDIRT, LOCATION(60 + 31, 17 - 1), aiNum, mapNone, 0);
#endif
	TethysGame::CreateUnit(Unit1, mapDIRT, LOCATION(70 + 31, 14 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapDIRT, LOCATION(66 + 31, 27 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapReinforcedResidence, LOCATION(74 + 31, 14 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapReinforcedResidence, LOCATION(78 + 31, 14 - 1), aiNum, mapNone, 0);
	TethysGame::CreateBeacon(mapMiningBeacon, 90 + 31, 7 - 1, 1, 0, 0);
	TethysGame::CreateUnit(rMine, mapCommonOreMine, LOCATION(90 + 31, 7 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(rSmelter[0], mapRareOreSmelter, LOCATION(90 + 31, 3 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(rSmelter[1], mapRareOreSmelter, LOCATION(95 + 31, 3 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(rSmelter[2], mapRareOreSmelter, LOCATION(95 + 31, 7 - 1), aiNum, mapNone, 0);
	TethysGame::CreateBeacon(mapMiningBeacon, 108 + 31, 11 - 1, 0, 1, -1);
	TethysGame::CreateUnit(cMine[1], mapCommonOreMine, LOCATION(108 + 31, 11 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(cSmelter[3], mapCommonOreSmelter, LOCATION(104 + 31, 7 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(cSmelter[4], mapCommonOreSmelter, LOCATION(109 + 31, 7 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(cSmelter[5], mapCommonOreSmelter, LOCATION(104 + 31, 11 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Spaceport[0], mapSpaceport, LOCATION(48 + 31, 4 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Spaceport[1], mapSpaceport, LOCATION(48 + 31, 9 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(AdvLab, mapAdvancedLab, LOCATION(97 + 31, 16 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapVehicleFactory, LOCATION(56 + 31, 23 - 1), aiNum, mapNone, 0);
#ifndef NOAIFACTORIES
	MainVF[3].TakeUnit(Unit1);
#endif

	// Civilian Units
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(76 + 31, 25 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainCommon[0].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(77 + 31, 25 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainCommon[0].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(81 + 31, 25 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainCommon[1].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(82 + 31, 25 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainCommon[1].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(76 + 31, 29 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainCommon[2].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(77 + 31, 29 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainCommon[2].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(69 + 31, 25 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainSF.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(70 + 31, 25 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainSF.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(71 + 31, 25 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainSF.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(72 + 31, 25 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainSF.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapRoboSurveyor, LOCATION(62 + 31, 26 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainEtc.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapRoboMiner, LOCATION(63 + 31, 27 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainEtc.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapEarthworker, LOCATION(58 + 31, 26 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainEtc.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapEarthworker, LOCATION(59 + 31, 27 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainEtc.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(89 + 31, 5 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainRare[0].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(90 + 31, 5 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainRare[0].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(95 + 31, 5 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainRare[1].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(96 + 31, 5 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainRare[1].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(94 + 31, 9 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainRare[2].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(95 + 31, 9 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainRare[2].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(103 + 31, 9 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainCommon[3].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(104 + 31, 9 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainCommon[3].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(109 + 31, 9 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainCommon[4].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(110 + 31, 9 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainCommon[4].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(104 + 31, 13 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainCommon[5].TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(105 + 31, 13 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
#ifndef NOAICIVUNITS
	MainCommon[5].TakeUnit(Unit1);
#endif

	// Combat Units
	map_id vType;
	if (numHumansNormal + numHumansHard > 0)
	{
		vType = mapTiger;
	}
	else
	{
		vType = mapPanther;
	}
	
#ifdef USECOMBATGROUPS
	CombatGroup ChokeDefense;
	ChokeDefense.CreateCombatGroup(aiNum);
	ChokeDefense.SetDestination(MAP_RECT(x, y, x, y));
#endif
	

	TethysGame::CreateUnit(Unit1, vType, LOCATION(2 + 31, 69 - 1), aiNum, mapEMP, 0);
	Unit1.DoSetLights(1);
#ifndef NOAIMILUNITS
	ChokeDefense.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(4 + 31, 69 - 1), aiNum, mapEMP, 0);
	Unit1.DoSetLights(1);
#ifndef NOAIMILUNITS
	ChokeDefense.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(6 + 31, 69 - 1), aiNum, mapEMP, 0);
	Unit1.DoSetLights(1);
#ifndef NOAIMILUNITS
	ChokeDefense.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, vType, LOCATION(2 + 31, 70 - 1), aiNum, mapRPG, 0);
	Unit1.DoSetLights(1);
#ifndef NOAIMILUNITS
	ChokeDefense.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, vType, LOCATION(3 + 31, 70 - 1), aiNum, mapRPG, 0);
	Unit1.DoSetLights(1);
#ifndef NOAIMILUNITS
	ChokeDefense.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(4 + 31, 70 - 1), aiNum, mapRPG, 0);
	Unit1.DoSetLights(1);
#ifndef NOAIMILUNITS
	ChokeDefense.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(5 + 31, 70 - 1), aiNum, mapRPG, 0);
	Unit1.DoSetLights(1);
#ifndef NOAIMILUNITS
	ChokeDefense.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(6 + 31, 70 - 1), aiNum, mapRPG, 0);
	Unit1.DoSetLights(1);
#ifndef NOAIMILUNITS
	ChokeDefense.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(7 + 31, 70 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
#ifndef NOAIMILUNITS
	ChokeDefense.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(8 + 31, 70 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
#ifndef NOAIMILUNITS
	ChokeDefense.TakeUnit(Unit1);
#endif
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(7 + 31, 69 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
#ifndef NOAIMILUNITS
	ChokeDefense.TakeUnit(Unit1);
#endif

	// Guard Posts
	if (numHumansNormal + numHumansHard > 0)
	{
		vType = mapESG;
	}
	else
	{
		vType = mapEMP;
	}
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(8 + 31, 72 - 1), aiNum, vType, 0);
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(6 + 31, 72 - 1), aiNum, mapRPG, 0);
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(4 + 31, 72 - 1), aiNum, mapRPG, 0);
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(127 + 31, 4 - 1), aiNum, mapESG, 0);
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(42 + 31, 10 - 1), aiNum, mapESG, 0);
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(42 + 31, 4 - 1), aiNum, mapESG, 0);

	// Walls and Tubes
	CreateTubeOrWallLine(84 + 31, 23 - 1, 97 + 31, 23 - 1, mapTube);
	CreateTubeOrWallLine(97 + 31, 22 - 1, 97 + 31, 19 - 1, mapTube);
	CreateTubeOrWallLine(100 + 31, 16 - 1, 103 + 31, 16 - 1, mapTube);
	CreateTubeOrWallLine(104 + 31, 14 - 1, 104 + 31, 16 - 1, mapTube);
	CreateTubeOrWallLine(98 + 31, 7 - 1, 101 + 31, 7 - 1, mapTube);
	CreateTubeOrWallLine(52 + 31, 9 - 1, 61 + 31, 9 - 1, mapTube);
	CreateTubeOrWallLine(61 + 31, 10 - 1, 61 + 31, 15 - 1, mapTube);
	CreateTubeOrWallLine(114 + 31, 7 - 1, 114 + 31, 1 - 1, mapWall);
	CreateTubeOrWallLine(103 + 31, 41 - 1, 109 + 31, 41 - 1, mapWall);
	CreateTubeOrWallLine(28 + 31, 11 - 1, 28 + 31, 17 - 1, mapWall);
	CreateTubeOrWallLine(29 + 31, 11 - 1, 35 + 31, 11 - 1, mapWall);
	CreateTubeOrWallLine(40 + 31, 1 - 1, 40 + 31, 5 - 1, mapWall);
	CreateTubeOrWallLine(40 + 31, 8 - 1, 40 + 31, 11 - 1, mapWall);

	// Initialize mining groups
	MainCommon[0].Setup(cMine[0], cSmelter[0], MAP_RECT(76 + 31, 23 - 1, 82 + 31, 28 - 1));
	MainCommon[1].Setup(cMine[0], cSmelter[1], MAP_RECT(76 + 31, 25 - 1, 82 + 31, 28 - 1));
	MainCommon[2].Setup(cMine[0], cSmelter[2], MAP_RECT(78 + 31, 23 - 1, 82 + 31, 28 - 1));

	MainRare[0].Setup(rMine, rSmelter[0], MAP_RECT(87 + 31, 3 - 1, 91 + 31, 8 - 1));
	MainRare[1].Setup(rMine, rSmelter[1], MAP_RECT(89 + 31, 3 - 1, 95 + 31, 8 - 1));
	MainRare[2].Setup(rMine, rSmelter[2], MAP_RECT(88 + 31, 6 - 1, 95 + 31, 9 - 1));

	MainCommon[3].Setup(cMine[1], cSmelter[3], MAP_RECT(104 + 31, 7 - 1, 110 + 31, 13 - 1));
	MainCommon[4].Setup(cMine[1], cSmelter[4], MAP_RECT(104 + 31, 10 - 1, 110 + 31, 13 - 1));
	MainCommon[5].Setup(cMine[1], cSmelter[5], MAP_RECT(106 + 31, 7 - 1, 110 + 31, 13 - 1));

	odprintf("End SetupAI\n");
} // end SetupMainBase

void FraxSurvAI::PrepareUnitGroupsMain()
{
	odprintf("Begin PrepareUnitGroupsMain\n");
	// Setup mining groups
	MainCommon[0] = CreateMiningGroup(aiNum);
	MainCommon[0].SetTargCount(mapCargoTruck, mapNone, 2);

	MainCommon[1] = CreateMiningGroup(aiNum);
	MainCommon[1].SetTargCount(mapCargoTruck, mapNone, 2);

	MainCommon[2] = CreateMiningGroup(aiNum);
	MainCommon[2].SetTargCount(mapCargoTruck, mapNone, 2);

	MainCommon[3] = CreateMiningGroup(aiNum);
	MainCommon[3].SetTargCount(mapCargoTruck, mapNone, 2);

	MainCommon[4] = CreateMiningGroup(aiNum);
	MainCommon[4].SetTargCount(mapCargoTruck, mapNone, 2);

	MainCommon[5] = CreateMiningGroup(aiNum);
	MainCommon[5].SetTargCount(mapCargoTruck, mapNone, 2);

	MainRare[0] = CreateMiningGroup(aiNum);
	MainRare[0].SetTargCount(mapCargoTruck, mapNone, 2);

	MainRare[1] = CreateMiningGroup(aiNum);
	MainRare[1].SetTargCount(mapCargoTruck, mapNone, 2);

	MainRare[2] = CreateMiningGroup(aiNum);
	MainRare[2].SetTargCount(mapCargoTruck, mapNone, 2);

	MainSF = CreateBuildingGroup(aiNum);
	MainSF.SetTargCount(mapConVec, mapNone, 4);
	MainSF.SetRect(MAP_RECT(68 + 31, 28 - 1, 73 + 31, 30 - 1));

	MainEtc = CreateBuildingGroup(aiNum);
	MainEtc.SetTargCount(mapRoboMiner, mapNone, 2);
	MainEtc.SetTargCount(mapEarthworker, mapNone, 2);
	MainEtc.SetRect(MAP_RECT(59 + 31, 25 - 1, 64 + 31, 27 - 1));

	MainVF[0] = CreateBuildingGroup(aiNum);
	MainVF[0].RecordVehReinforceGroup(MainCommon[0], 9999);
	MainVF[0].RecordVehReinforceGroup(MainCommon[3], 9998);
	MainVF[0].RecordVehReinforceGroup(MainRare[0], 9997);

	MainVF[1] = CreateBuildingGroup(aiNum);
	MainVF[1].RecordVehReinforceGroup(MainCommon[1], 9999);
	MainVF[1].RecordVehReinforceGroup(MainCommon[4], 9998);
	MainVF[1].RecordVehReinforceGroup(MainRare[1], 9997);

	MainVF[2] = CreateBuildingGroup(aiNum);
	MainVF[2].RecordVehReinforceGroup(MainCommon[2], 9999);
	MainVF[2].RecordVehReinforceGroup(MainCommon[5], 9998);
	MainVF[2].RecordVehReinforceGroup(MainRare[2], 9997);

	MainVF[3] = CreateBuildingGroup(aiNum);
	MainVF[3].RecordVehReinforceGroup(MainSF, 9999);
	MainVF[3].RecordVehReinforceGroup(MainEtc, 9998);

	MainVF[4] = CreateBuildingGroup(aiNum);
	MainVF[5] = CreateBuildingGroup(aiNum);

#ifndef NOAIREPAIRS
	MainRepairs = CreateBuildingGroup(aiNum);
	MainRepairs.SetTargCount(mapSpider, mapNone, 4);
	MainRepairs.SetRect(MAP_RECT(64 + 31, 11 - 1, 68 + 31, 14 - 1));
	MainRepairs.RecordVehReinforceGroup(MainRepairs, 1000);
#endif

	MainDefense[0] = CreateFightGroup(aiNum);
	MainDefense[0].SetTargCount(mapLynx, mapRPG, 16);
	MainDefense[0].SetTargCount(mapLynx, mapEMP, 12);
	MainDefense[0].SetRect(MAP_RECT(33 + 31, 1 - 1, 43 + 31, 10 - 1));
	MainDefense[0].AddGuardedRect(MAP_RECT(16 + 31, 1 - 1, 32 + 31, 10 - 1));
	MainDefense[0].AddGuardedRect(MAP_RECT(28 + 31, 1 - 1, 73 + 31, 28 - 1));
	MainDefense[0].AddGuardedRect(MAP_RECT(58 + 31, 1 - 1, 92 + 31, 37 - 1));
	MainVF[0].RecordVehReinforceGroup(MainDefense[0], 7000);

	MainDefense[1] = CreateFightGroup(aiNum);
	MainDefense[1].SetTargCount(mapTiger, mapRPG, 12);
	MainDefense[1].SetTargCount(mapTiger, mapEMP, 8);
	MainDefense[1].SetTargCount(mapTiger, mapESG, 6);
	MainDefense[1].SetRect(MAP_RECT(78 + 31, 30 - 1, 88 + 31, 34 - 1));
	MainDefense[1].AddGuardedRect(MAP_RECT(58 + 31, 1 - 1, 92 + 31, 37 - 1));
	MainDefense[1].AddGuardedRect(MAP_RECT(94 + 31, 31 - 1, 96 + 31, 39 - 1));
	MainDefense[1].AddGuardedRect(MAP_RECT(97 + 31, 33 - 1, 100 + 31, 36 - 1));
	MainVF[1].RecordVehReinforceGroup(MainDefense[1], 7000);

	MainDefense[2] = CreateFightGroup(aiNum);
	MainDefense[2].SetTargCount(mapLynx, mapESG, 12);
	MainDefense[2].SetTargCount(mapLynx, mapRPG, 10);
	MainDefense[2].SetTargCount(mapLynx, mapEMP, 10);
	MainDefense[2].SetRect(MAP_RECT(84 + 31, 14 - 1, 92 + 31, 26 - 1));
	MainDefense[2].AddGuardedRect(MAP_RECT(80 + 31, 1 - 1, 101 + 31, 25 - 1));
	MainDefense[2].AddGuardedRect(MAP_RECT(102 + 31, 1 - 1, 128 + 31, 26 - 1));
	MainDefense[2].AddGuardedRect(MAP_RECT(58 + 31, 1 - 1, 92 + 31, 37 - 1));
	MainVF[2].RecordVehReinforceGroup(MainDefense[2], 7000);

	ChokeDefense = CreateFightGroup(aiNum);
	ChokeDefense.SetRect(MAP_RECT(0 + 31, 66 - 1, 6 + 31, 71 - 1));
	ChokeDefense.AddGuardedRect(MAP_RECT(0 + 31, 66 - 1, 10 + 31, 77 - 1));
	//ChokeDefense.SetDeleteWhenEmpty(1);

	RevengeGroup[0] = CreateFightGroup(aiNum);
	RevengeGroup[1] = CreateFightGroup(aiNum);

	MainOffense[0] = CreateFightGroup(aiNum);
	MainOffense[0].SetTargCount(mapLynx, mapMicrowave, 7 + numHumansHard);
	MainOffense[0].SetRect(MAP_RECT(58 + 31, 38 - 1, 69 + 31, 44 - 1));
	MainVF[3].RecordVehReinforceGroup(MainOffense[0], 4000);

	MainOffense[1] = CreateFightGroup(aiNum);
	MainOffense[1].SetRect(MAP_RECT(76 + 31, 47 - 1, 87 + 31, 52 - 1));

	// Send attack waves
	Attacking[0] = CreateFightGroup(aiNum);
	//Attacking[0].SetAttackType(mapCommandCenter);
	//Attacking[0].DoAttackEnemy();

	Attacking[1] = CreateFightGroup(aiNum);
	Attacking[1].SetAttackType(mapCommandCenter);
	Attacking[1].DoAttackEnemy();

	//Attacking[2] = CreateFightGroup(aiNum);
	//Attacking[2].SetAttackType(mapCommandCenter);
	//Attacking[2].DoAttackEnemy();

	// Start attacks.
	/*
	// These are now setup by each attack wave.  DO NOT UNCOMMENT!  Left here for timing reference.
	TrigAttackMain = CreateTimeTrigger(1, 1, 19000, 19100, "Main_AttackTeaser");
	TrigAttackMain[1] = CreateTimeTrigger(1, 1, 27200, 27300, "Main_Attack1");
	TrigAttackMain[2] = CreateTimeTrigger(1, 1, 34600, 34800, "Main_Attack2");
	TrigAttackMain[3] = CreateTimeTrigger(1, 1, 41100, 41300, "Main_Attack3");
	TrigAttackMain[4] = CreateTimeTrigger(1, 1, 48500, 48700, "Main_Attack5");
	TrigAttackMain[5] = CreateTimeTrigger(1, 1, 56000, 56100, "Main_Attack6");			// This usually catches an evacuation.
	TrigAttackMain[6] = CreateTimeTrigger(1, 1, 62200, 62300, "Main_Attack7");
	TrigAttackMain[7] = CreateTimeTrigger(1, 1, 69600, 69700, "Main_Attack8");
	TrigAttackMain[8] = CreateTimeTrigger(1, 1, 76000, 76200, "Main_Attack9");

	// The outposts are dead.  Time to step up our game!
	TrigAttackMain[9]  = CreateTimeTrigger(1, 1,  83600,  83800, "Main_Attack10");
	TrigAttackMain[10] = CreateTimeTrigger(1, 1,  90900,  91000, "Main_Attack11");
	TrigAttackMain[11] = CreateTimeTrigger(1, 1,  97400,  97500, "Main_Attack12");
	TrigAttackMain[12] = CreateTimeTrigger(1, 1, 104000, 104401, "Main_Attack13");
	TrigAttackMain[13] = CreateTimeTrigger(1, 1, 118000, 111900, "Main_Attack14");
	*/

	// Setup AI patrol groups
#ifndef NOAIMILUNITS
	PatrolGroups[0] = CreateFightGroup(aiNum);
	PatrolGroups[1] = CreateFightGroup(aiNum);

	// Now setup the actual patrols and units
	Unit Unit1;
	PatrolRoute Route1;
	LOCATION Points1[] =
	{
		LOCATION(54 + 31, 171 - 1),
		LOCATION(95 + 31, 161 - 1),
		LOCATION(-1,-1)
	};

	Route1.waypoints = Points1;
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(54 + 31, 171 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[0].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(54 + 31, 172 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[0].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(54 + 31, 173 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[0].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(54 + 31, 174 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[0].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(55 + 31, 171 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[0].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(55 + 31, 172 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[0].TakeUnit(Unit1);
	if (numHumansHard + numHumansHard > 0)
	{
		TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(55 + 31, 173 - 1), aiNum, mapMicrowave, 0);
		Unit1.DoSetLights(1);
		PatrolGroups[0].TakeUnit(Unit1);
		TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(55 + 31, 174 - 1), aiNum, mapMicrowave, 0);
		Unit1.DoSetLights(1);
		PatrolGroups[0].TakeUnit(Unit1);
	}

	PatrolGroups[0].SetPatrolMode(Route1);

	// Patrol group #2
	PatrolRoute Route2;
	LOCATION Points2[] =
	{
		LOCATION(49 + 31, 120 - 1),
		LOCATION(107 + 31, 125 - 1),
		LOCATION(-1,    -1)
	};
	Route2.waypoints = Points2;

	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(47 + 31, 120 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[1].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(47 + 31, 119 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[1].TakeUnit(Unit1);

	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(49 + 31, 120 - 1), aiNum, mapStickyfoam, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[1].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(50 + 31, 120 - 1), aiNum, mapStickyfoam, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[1].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(51 + 31, 120 - 1), aiNum, mapStickyfoam, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[1].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(52 + 31, 120 - 1), aiNum, mapStickyfoam, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[1].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(50 + 31, 119 - 1), aiNum, mapRPG, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[1].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(51 + 31, 119 - 1), aiNum, mapRPG, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[1].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(52 + 31, 119 - 1), aiNum, mapRPG, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[1].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(53 + 31, 119 - 1), aiNum, mapRPG, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[1].TakeUnit(Unit1);

	Player[aiNum].GoEden();
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(51 + 31, 117 - 1), aiNum, mapThorsHammer, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[1].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(52 + 31, 117 - 1), aiNum, mapThorsHammer, 0);
	Unit1.DoSetLights(1);
	PatrolGroups[1].TakeUnit(Unit1);
	if (numHumansHard > 1)
	{
		TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(54 + 31, 118 - 1), aiNum, mapAcidCloud, 0);
		Unit1.DoSetLights(1);
		PatrolGroups[1].TakeUnit(Unit1);
		TethysGame::CreateUnit(Unit1, mapPanther, LOCATION(55 + 31, 118 - 1), aiNum, mapAcidCloud, 0);
		Unit1.DoSetLights(1);
		PatrolGroups[1].TakeUnit(Unit1);
	}

	Player[aiNum].GoPlymouth();

	PatrolGroups[1].SetPatrolMode(Route2);
#endif

	odprintf("End PrepareUnitGroupsMain\n");
}

void FraxSurvAI::RecordStructuresMain()
{
#ifndef NOAIFACTORIES
	odprintf("Begin RecordStructuresMain\n");
	// High Priority Structures
	MainSF.RecordBuilding(LOCATION(70 + 31, 20 - 1), mapCommandCenter, mapNone);
	MainSF.RecordBuilding(LOCATION(71 + 31, 27 - 1), mapStructureFactory, mapNone);
	MainSF.RecordBuilding(LOCATION(76 + 31, 23 - 1), mapCommonOreSmelter, mapNone);
	MainSF.RecordBuilding(LOCATION(76 + 31, 27 - 1), mapCommonOreSmelter, mapNone);
	MainSF.RecordBuilding(LOCATION(81 + 31, 23 - 1), mapCommonOreSmelter, mapNone);
	MainSF.RecordBuilding(LOCATION(97 + 31, 16 - 1), mapAdvancedLab, mapNone);
	MainSF.RecordBuilding(LOCATION(95 + 31, 3 - 1), mapRareOreSmelter, mapNone);
	MainSF.RecordBuilding(LOCATION(79 + 31, 3 - 1), mapMHDGenerator, mapNone);
	MainSF.RecordBuilding(LOCATION(76 + 31, 3 - 1), mapMHDGenerator, mapNone);
	MainSF.RecordBuilding(LOCATION(71 + 31, 23 - 1), mapVehicleFactory, mapNone);
	MainSF.RecordBuilding(LOCATION(66 + 31, 23 - 1), mapVehicleFactory, mapNone);
	MainSF.RecordBuilding(LOCATION(61 + 31, 23 - 1), mapVehicleFactory, mapNone);
	MainSF.RecordBuilding(LOCATION(56 + 31, 23 - 1), mapVehicleFactory, mapNone);
	MainSF.RecordBuilding(LOCATION(95 + 31, 3 - 1), mapRareOreSmelter, mapNone);
	MainSF.RecordBuilding(LOCATION(95 + 31, 7 - 1), mapRareOreSmelter, mapNone);
	MainSF.RecordBuilding(LOCATION(104 + 31, 7 - 1), mapCommonOreSmelter, mapNone);
	MainSF.RecordBuilding(LOCATION(109 + 31, 7 - 1), mapCommonOreSmelter, mapNone);
	MainSF.RecordBuilding(LOCATION(104 + 31, 11 - 1), mapCommonOreSmelter, mapNone);
	MainSF.RecordBuilding(LOCATION(73 + 31, 3 - 1), mapMHDGenerator, mapNone);
	MainSF.RecordBuilding(LOCATION(70 + 31, 3 - 1), mapMHDGenerator, mapNone);
	MainSF.RecordBuilding(LOCATION(104 + 31, 3 - 1), mapVehicleFactory, mapNone, MainVF[4]);
	MainSF.RecordBuilding(LOCATION(109 + 31, 3 - 1), mapVehicleFactory, mapNone, MainVF[5]);
	MainSF.RecordBuilding(LOCATION(61 + 31, 20 - 1), mapRobotCommand, mapNone);
#ifndef NOAIREPAIRS
	MainSF.RecordBuilding(LOCATION(61 + 31, 17 - 1), mapArachnidFactory, mapNone);
#endif
	MainSF.RecordBuilding(LOCATION(48 + 31, 4 - 1), mapSpaceport, mapNone);
	MainSF.RecordBuilding(LOCATION(48 + 31, 9 - 1), mapSpaceport, mapNone);
	MainSF.RecordBuilding(LOCATION(70 + 31, 6 - 1), mapSolarPowerArray, mapNone);
	MainSF.RecordBuilding(LOCATION(78 + 31, 6 - 1), mapSolarPowerArray, mapNone);

	// Low Priority Structures
#ifdef NOAIREPAIRS
	MainSF.RecordBuilding(LOCATION(60 + 31, 17 - 1), mapDIRT, mapNone);
#endif
	MainSF.RecordBuilding(LOCATION(70 + 31, 17 - 1), mapStandardLab, mapNone);
	MainSF.RecordBuilding(LOCATION(77 + 31, 20 - 1), mapAgridome, mapNone);
	MainSF.RecordBuilding(LOCATION(81 + 31, 20 - 1), mapAgridome, mapNone);
	MainSF.RecordBuilding(LOCATION(77 + 31, 17 - 1), mapAgridome, mapNone);
	MainSF.RecordBuilding(LOCATION(81 + 31, 17 - 1), mapAgridome, mapNone);
	MainSF.RecordBuilding(LOCATION(70 + 31, 14 - 1), mapDIRT, mapNone);
	MainSF.RecordBuilding(LOCATION(66 + 31, 27 - 1), mapDIRT, mapNone);
	MainSF.RecordBuilding(LOCATION(74 + 31, 20 - 1), mapNursery, mapNone);
	MainSF.RecordBuilding(LOCATION(74 + 31, 17 - 1), mapUniversity, mapNone);
	MainSF.RecordBuilding(LOCATION(67 + 31, 20 - 1), mapMedicalCenter, mapNone);
	MainSF.RecordBuilding(LOCATION(67 + 31, 17 - 1), mapMedicalCenter, mapNone);
	MainSF.RecordBuilding(LOCATION(64 + 31, 20 - 1), mapMedicalCenter, mapNone);
	MainSF.RecordBuilding(LOCATION(64 + 31, 17 - 1), mapMedicalCenter, mapNone);
	MainSF.RecordBuilding(LOCATION(74 + 31, 14 - 1), mapReinforcedResidence, mapNone);
	MainSF.RecordBuilding(LOCATION(78 + 31, 14 - 1), mapReinforcedResidence, mapNone);

	// Handled by secondary team
	MainEtc.RecordBuilding(LOCATION(81 + 31, 27 - 1), mapCommonOreMine, mapNone);
	MainEtc.RecordBuilding(LOCATION(90 + 31, 7 - 1), mapCommonOreMine, mapNone);
	MainEtc.RecordBuilding(LOCATION(108 + 31, 11 - 1), mapRareOreMine, mapNone);

	for (int x = 41 + 31; x < 119 + 31; x++)
	{
		for (int y = 1 - 1; y < 30 - 1; y++)
		{
			if (GameMap::GetTile(LOCATION(x, y)) >= 417 && GameMap::GetTile(LOCATION(x, y)) <= 432)
			{
				MainEtc.RecordTube(LOCATION(x, y));
			}
		}
	}

	for (int y = 4 - 1; y > 9 - 1; y++)
	{
		MainEtc.RecordTube(LOCATION(52 + 31, y));
	}

	for (int x = 62 + 31; x < 69 - 1; x++)
	{
		MainEtc.RecordTube(LOCATION(x, 14 - 1));
	}

	odprintf("End RecordStructuresMain\n");
#endif
}

// ----------------------------------------------------------------------------
// Spaceport/EMP Missile Management
// ----------------------------------------------------------------------------

void FraxSurvAI::CheckSpaceportStatus()
{
	if (MainCC.IsLive())
	{
		// Check if spaceport is alive and can build/launch a missile.
		if (Spaceport[0].IsLive())
		{
			if (!Spaceport[0].IsBusy() &&
				Spaceport[0].GetLastCommand() != ctMoDevelop &&
				Spaceport[0].GetObjectOnPad() != mapEMPMissile)
			{
				// Build the missile.
				Spaceport[0].DoDevelop(mapEMPMissile);
			}
			else if (!Spaceport[0].IsBusy() &&
				     Spaceport[0].GetObjectOnPad() == mapEMPMissile &&
				TethysGame::Tick() > (LastMissileTime[0]))
			{
				LaunchEmpMissile1();
			}
		}

		// Spaceport is dead.  Check if it has been rebuilt.
		else
		{
			CheckRebuiltSpaceport1();
		}


		// Check if spaceport is alive and can build/launch a missile.
		if (Spaceport[1].IsLive())
		{
			if (!Spaceport[1].IsBusy() &&
				Spaceport[1].GetLastCommand() != ctMoDevelop &&
				Spaceport[1].GetObjectOnPad() != mapEMPMissile)
			{
				// Build the missile.
				Spaceport[1].DoDevelop(mapEMPMissile);
			}
			else if (!Spaceport[1].IsBusy() &&
				Spaceport[1].GetObjectOnPad() == mapEMPMissile &&
				TethysGame::Tick() > (LastMissileTime[1]))
			{
				LaunchEmpMissile2();
			}
		}

		// Spaceport is dead.  Check if it has been rebuilt.
		else
		{
			CheckRebuiltSpaceport2();
		}
	}
}

void FraxSurvAI::CheckRebuiltSpaceport1()
{
	UnitEx i;
	PlayerBuildingEnum GetSP(aiNum, mapSpaceport);
	while (GetSP.GetNext(i))
	{
		if (i.Location().y == 4 - 1)
		{
			Spaceport[0] = i;
			return;
		}
	}
}

void FraxSurvAI::CheckRebuiltSpaceport2()
{
	UnitEx i;
	PlayerBuildingEnum GetSP(aiNum, mapSpaceport);
	while (GetSP.GetNext(i))
	{
		if (i.Location().y == 9 - 1)
		{
			Spaceport[1] = i;
			return;
		}
	}
}

void FraxSurvAI::LaunchEmpMissile1()
{
	odprintf("Begin LaunchEmpMissile1\n");
	// Pick a semi-random enemy target.  Start by picking a random victim.
	int victim = TethysGame::GetRand(aiNum),
		oldValue = -1,
		newValue;
	Unit target, i;

	PlayerUnitEnum targetEnum(victim);
	odprintf("LaunchEmpMissile1: Trying to find target.\n");
	while (targetEnum.GetNext(i))
	{

		//char string[50];
		//scr_snprintf(string, 50, "Unit Type: %d", i.GetType());
		//TethysGame::AddMessage(-1, -1, string, -1, sndRes1_sel);

		if (i.GetType() == mapConVec ||
			i.GetType() == mapCargoTruck ||
			i.GetType() == mapLynx ||
			i.GetType() == mapPanther ||
			i.GetType() == mapTiger)
		{
			// Generate random target value
			newValue = TethysGame::GetRand(100);
			if (newValue > oldValue)
			{
				oldValue = newValue;
				target = i;
			}
		}

	}

	// Make sure we actually got a target
	odprintf("LaunchEmpMissile1: Target maybe found.  Attempting to launch.\n");
	if (target.IsLive() && Spaceport[0].IsLive())
	{
		Spaceport[0].DoLaunch(target.Location().x * 32, target.Location().y * 32, 1);
		LastMissileTime[0] = TethysGame::Tick() + 2900 + TethysGame::GetRand(1400);
		odprintf("LaunchEmpMissile1: Launched.\n");
	}
	odprintf("End LaunchEmpMissile1\n");
}

void FraxSurvAI::LaunchEmpMissile2()
{
	odprintf("Begin LaunchEmpMissile2\n");
	// Try to pick a target intelligently.
	// TO-DO

		// Pick a semi-random enemy target.  Start by picking a random victim.
	int victim = TethysGame::GetRand(aiNum),
		oldValue = -1,
		newValue;
	Unit target, i;

	PlayerUnitEnum targetEnum(victim);
	while (targetEnum.GetNext(i))
	{

		//char string[50];
		//scr_snprintf(string, 50, "Unit Type: %d", i.GetType());
		//TethysGame::AddMessage(-1, -1, string, -1, sndRes1_sel);

		if (i.GetType() == mapConVec ||
			i.GetType() == mapCargoTruck ||
			i.GetType() == mapLynx ||
			i.GetType() == mapPanther ||
			i.GetType() == mapTiger)
		{
			// Generate random target value
			newValue = TethysGame::GetRand(100);
			if (newValue > oldValue)
			{
				oldValue = newValue;
				target = i;
			}
		}

	}

	// Make sure we actually got a target
	if (target.IsLive() && Spaceport[1].IsLive())
	{
		Spaceport[1].DoLaunch(target.Location().x * 32, target.Location().y * 32, 1);
		LastMissileTime[1] = TethysGame::Tick() + 2900 + TethysGame::GetRand(1400);
	}
	odprintf("End LaunchEmpMissile2\n");
}

// ----------------------------------------------------------------------------
// "...to the last I grapple with thee; from hell’s heart I stab at thee; for hate’s sake I spit my last breath at thee."
// ----------------------------------------------------------------------------

void FraxSurvAI::CheckRevenge()
{
	if (!MainCC.IsLive())
	{
		odprintf("AI Revenge: Revenge mode activated.\n");

		// Give the AI some final "help"
#ifdef USECOMBATGROUPS
		CombatGroup revengeGroup;
		revengeGroup.CreateCombatGroup(aiNum);
#endif

		UnitEx Unit1;
		for (int x = 68; x < 81; x++)
		{
			TethysGame::CreateUnit(Unit1, mapTiger, LOCATION(x + 31, 0), aiNum, mapESG, 2);
			Unit1.DoSetLights(1);
			RevengeGroup[0].TakeUnit(Unit1);
			TethysGame::CreateUnit(Unit1, mapTiger, LOCATION(x + 31, 1), aiNum, mapESG, 2);
			Unit1.DoSetLights(1);
			RevengeGroup[1].TakeUnit(Unit1);
		}

		//MainDefense[0].SetDeleteWhenEmpty(1);
		MainDefense[0].ClearGuarderdRects();
		MainDefense[0].SetAttackType(mapCommandCenter);
		MainDefense[0].DoAttackEnemy();

		//MainDefense[1].SetDeleteWhenEmpty(1);
		MainDefense[1].ClearGuarderdRects();
		MainDefense[1].SetAttackType(mapCommandCenter);
		MainDefense[1].DoAttackEnemy();

		//MainDefense[2].SetDeleteWhenEmpty(1);
		MainDefense[2].ClearGuarderdRects();
		MainDefense[2].SetAttackType(mapCommandCenter);
		MainDefense[2].DoAttackEnemy();

		//MainOffense[0].SetDeleteWhenEmpty(1);
		MainOffense[0].ClearGuarderdRects();
		MainOffense[0].SetAttackType(mapCommandCenter);
		MainOffense[0].DoAttackEnemy();

		//MainOffense[1].SetDeleteWhenEmpty(1);
		MainOffense[1].ClearGuarderdRects();
		MainOffense[1].SetAttackType(mapCommandCenter);
		MainOffense[1].DoAttackEnemy();

		RevengeGroup[0].SetAttackType(mapCommandCenter);
		RevengeGroup[0].DoAttackEnemy();

		RevengeGroup[1].SetAttackType(mapCommandCenter);
		RevengeGroup[1].DoAttackEnemy();

#ifdef USECOMBATGROUPS
		allCombatGroups.push_back(revengeGroup);
#endif

		// Now, cleanup the AI.
		//trigUpdateCycle.Destroy();
		//odprintf(" >AI Revenge: Update cycle destroyed.\n");

		Unit ToRemove;

		if (MainSF.IsInitialized())
		{
			SafeDestroyGroup(MainSF);
		}
		odprintf(" >AI Revenge: SF group destroyed.\n");

		for (int i = 0; i < AI_MAIN_VF_GROUPS; i++)
		{
			if (MainVF[i].IsInitialized())
			{
				SafeDestroyGroup(MainVF[i]);
			}
		}
		odprintf(" >AI Revenge: VF groups destroyed.\n");

		for (int i = 0; i < AI_MAIN_COMMON_GROUPS; i++)
		{
			if (MainCommon[i].IsInitialized())
			{
				SafeDestroyGroup(MainCommon[i]);
			}
		}
		odprintf(" >AI Revenge: Common ore groups destroyed.\n");

		for (int i = 0; i < AI_MAIN_RARE_GROUPS; i++)
		{
			if (MainRare[i].IsInitialized())
			{
				SafeDestroyGroup(MainRare[i]);
			}
		}
		odprintf(" >AI Revenge: Rare ore groups destroyed.\n");

		GroupEnumerator DeleteAll(Player[aiNum].GetDefaultGroup());
		while (DeleteAll.GetNext(ToRemove))
		{
			Player[aiNum].GetDefaultGroup().RemoveUnit(ToRemove);
		}

		odprintf("AI Revenge: Done.\n");
	}
}

#ifndef NOAIREPAIRS
Export void Main_Repair()
{
	//	odprintf("Base alloc addr %p | Next alloc addr %p | Buffer end %p", (unsigned int)(*waypointBaseAllocAddr), (unsigned int)(*waypointNextAllocAddr), (unsigned int)(*waypointBaseAllocAddr) + 0xFFC00);

	if (!MainRepairs.IsInitialized())
	{
		return;
	}

	// Get all units in the AI's main base
	UnitEx Next,
		Spider;
	InRectEnumerator MainBase(MAP_RECT(41 + 31, 1 - 1, 128 + 31, 35 - 1));
	GroupEnumerator Repair(MainRepairs);

	// Find damaged structures.
	while (MainBase.GetNext(Next))
	{
		if (Next.OwnerID() == aiNum &&
			Next.IsLive() &&
			Next.IsBuilding() &&
			Next.GetDamage() > 80 &&
			Next.GetLastCommand() != ctMoDevelop)
		{
			// Get all Spiders
			while (Repair.GetNext(Spider))
			{
				if (Spider.IsLive() && !Spider.IsBusy())
				{
					Spider.DoRepair(Next);
					break;
				}
			}
		}
	}
}
#endif
