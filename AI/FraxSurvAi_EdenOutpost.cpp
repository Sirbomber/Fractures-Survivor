// This file contains definitions for all AI functions related to managing the Eden AI outpost

#pragma once

#include "FraxSurvAi.h"

void FraxSurvAI::FraxSurvAI::SetupOutpost1()
{
	odprintf("Begin SetupOutpost1\n");
	Unit Unit1,
		cMine,
		cSmelter;

	Player[aiNum].GoEden();

	TethysGame::CreateUnit(Unit1, mapTokamak, LOCATION(18 + 31, 157 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(EdenCC, mapCommandCenter, LOCATION(12 + 31, 134 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapAgridome, LOCATION(12 + 31, 131 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapAgridome, LOCATION(16 + 31, 134 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapAgridome, LOCATION(16 + 31, 131 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapNursery, LOCATION(20 + 31, 134 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapUniversity, LOCATION(20 + 31, 131 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapResidence, LOCATION(9 + 31, 134 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapResidence, LOCATION(9 + 31, 131 - 1), aiNum, mapNone, 0);
	//TethysGame::CreateUnit(Unit1, mapStructureFactory, LOCATION(11+31, 137-1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapVehicleFactory, LOCATION(11 + 31, 137 - 1), aiNum, mapNone, 0);
	EdenVF[0].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapVehicleFactory, LOCATION(16 + 31, 137 - 1), aiNum, mapNone, 0);
	EdenVF[1].TakeUnit(Unit1);
	TethysGame::CreateUnit(cSmelter, mapCommonOreSmelter, LOCATION(11 + 31, 141 - 1), aiNum, mapNone, 0);

	TethysGame::CreateBeacon(mapMiningBeacon, 15 + 31, 144 - 1, 0, 1, -1);
	TethysGame::CreateUnit(cMine, mapCommonOreMine, LOCATION(15 + 31, 144 - 1), aiNum, mapNone, 0);

	// Civillian Vehicles
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(10 + 31, 143 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
	EdenCommon.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(11 + 31, 143 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
	EdenCommon.TakeUnit(Unit1);
	//TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(11+31, 139-1), aiNum, mapNone, 0);
	//    Unit1.DoSetLights(1);
	//TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(12+31, 139-1), aiNum, mapNone, 0);
	//    Unit1.DoSetLights(1);

	// Defenders
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(20 + 31, 142 - 1), aiNum, mapLaser, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(21 + 31, 142 - 1), aiNum, mapLaser, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(22 + 31, 142 - 1), aiNum, mapLaser, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(23 + 31, 142 - 1), aiNum, mapLaser, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(24 + 31, 142 - 1), aiNum, mapLaser, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(25 + 31, 142 - 1), aiNum, mapLaser, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(26 + 31, 142 - 1), aiNum, mapLaser, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(27 + 31, 142 - 1), aiNum, mapLaser, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(20 + 31, 143 - 1), aiNum, mapLaser, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(21 + 31, 143 - 1), aiNum, mapLaser, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(22 + 31, 143 - 1), aiNum, mapLaser, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(23 + 31, 143 - 1), aiNum, mapLaser, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(18 + 31, 140 - 1), aiNum, mapRailGun, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(19 + 31, 140 - 1), aiNum, mapRailGun, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(20 + 31, 140 - 1), aiNum, mapRailGun, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(21 + 31, 140 - 1), aiNum, mapRailGun, 0);
	Unit1.DoSetLights(1);
	EdenDefense.TakeUnit(Unit1);


	// Guard Posts
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(44 + 31, 170 - 1), aiNum, mapLaser, 0);
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(47 + 31, 170 - 1), aiNum, mapLaser, 0);
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(32 + 31, 137 - 1), aiNum, mapLaser, 0);
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(32 + 31, 141 - 1), aiNum, mapLaser, 0);
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(3 + 31, 125 - 1), aiNum, mapLaser, 0);

	TethysGame::CreateUnit(Unit1, mapTokamak, LOCATION(4 + 31, 148 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapTokamak, LOCATION(9 + 31, 150 - 1), aiNum, mapNone, 0);

	Player[aiNum].GoPlymouth();

	EdenCommon.Setup(cMine, cSmelter, MAP_RECT(11 + 31, 141 - 1, 16 + 31, 145 - 1));

	// Why not?
	CreateTubeOrWallLine(1 + 31, 152 - 1, 27 + 31, 152 - 1, mapMicrobeWall);
	CreateTubeOrWallLine(1 + 31, 153 - 1, 27 + 31, 153 - 1, mapMicrobeWall);

}

void FraxSurvAI::PrepareUnitGroupsEden()
{
	// Mining group
	EdenCommon = CreateMiningGroup(aiNum);
	EdenCommon.SetTargCount(mapCargoTruck, mapNone, 2);

	// Vehicle Factories
	EdenVF[0] = CreateBuildingGroup(aiNum);
	EdenVF[1] = CreateBuildingGroup(aiNum);

	// Repair Group
	EdenRepairs = CreateBuildingGroup(aiNum);
	EdenRepairs.SetTargCount(mapRepairVehicle, mapNone, 1);
	EdenRepairs.SetRect(MAP_RECT(2 + 31, 134 - 1, 5 + 31, 138 - 1));
	EdenVF[0].RecordVehReinforceGroup(EdenRepairs, 2000);

	// Defense group
	EdenDefense = CreateFightGroup(aiNum);
	EdenDefense.SetTargCount(mapLynx, mapLaser, 12);
	EdenDefense.SetTargCount(mapLynx, mapRailGun, 4);
	EdenDefense.SetRect(MAP_RECT(17 + 31, 139 - 1, 28 + 31, 147 - 1));
	EdenDefense.AddGuardedRect(MAP_RECT(1 + 31, 126 - 1, 30 + 31, 158 - 1));
	EdenVF[0].RecordVehReinforceGroup(EdenDefense, 7000);

	EdenOffense = CreateFightGroup(aiNum);
	EdenOffense.SetTargCount(mapLynx, mapLaser, (2 * (TethysGame::NoPlayers())) + 1);
	EdenOffense.SetRect(MAP_RECT(17 + 31, 139 - 1, 28 + 31, 147 - 1));
	EdenOffense.AddGuardedRect(MAP_RECT(1 + 31, 126 - 1, 30 + 31, 158 - 1));
	EdenVF[1].RecordVehReinforceGroup(EdenOffense, 7000);

	/*
	// These exist as a reference and provide a rough estimate of when each attack wave should be launched.  DO NOT UNCOMMENT!
	TrigAttackEden = CreateTimeTrigger(1, 1, 22000, 22100, "Eden_Attack1");
	CreateTimeTrigger(1, 1, 29400, 29500, "Eden_Attack2");
	CreateTimeTrigger(1, 1, 36800, 36900, "Eden_Attack3");
	CreateTimeTrigger(1, 1, 43400, 43600, "Eden_Attack4");
	CreateTimeTrigger(1, 1, 50200, 50300, "Eden_Attack5");
	CreateTimeTrigger(1, 1, 57900, 58100, "Eden_Attack6");
	CreateTimeTrigger(1, 1, 64400, 64500, "Eden_Attack7");
	CreateTimeTrigger(1, 1, 71600, 71700, "Eden_Attack8");
	CreateTimeTrigger(1, 1, 78000, 78100, "Eden_Attack9");
	CreateTimeTrigger(1, 1, 84300, 84400, "Eden_Attack10");
	CreateTimeTrigger(1, 1, 83200, 83600, "Eden_AttackFinal");
	*/
	odprintf("End SetupOutpost1\n");
}

void FraxSurvAI::Eden_Repair()
{
	if (!EdenRepairs.IsInitialized())
	{
		return;
	}

	UnitEx RpV, Next;

	// Get the Repair Vehicle
	GroupEnumerator Repair(EdenRepairs);
	Repair.GetNext(RpV);

	if (RpV.IsLive() && !RpV.IsBusy())
	{
		// Find damaged structures.
		InRectEnumerator EdenBase(MAP_RECT(4 + 31, 128 - 1, 33 + 31, 151 - 1));
		while (EdenBase.GetNext(Next))
		{
			if (Next.OwnerID() == aiNum &&
				Next.IsLive() &&
				Next.IsBuilding() &&
				Next.GetDamage() > 80 &&
				Next.GetLastCommand() != ctMoDevelop)
			{
				RpV.DoRepair(Next);
			}
		}
	}

}
