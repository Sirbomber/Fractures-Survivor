// This file contains definitions for all AI functions related to managing the Plymouth AI outpost

#pragma once

#include "FraxSurvAi.h"

void FraxSurvAI::SetupOutpost2()
{
	odprintf("Begin SetupOutpost2\n");
	Unit Unit1,
		CS1,
		CS2,
		CM;

	TethysGame::CreateUnit(PlymOutpostCC, mapCommandCenter, LOCATION(118 + 31, 143 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapStructureFactory, LOCATION(118 + 31, 146 - 1), aiNum, mapNone, 0);
	PlymouthSF.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapVehicleFactory, LOCATION(123 + 31, 146 - 1), aiNum, mapNone, 0);
	PlymouthVF[0].TakeUnit(Unit1);
	TethysGame::CreateUnit(CS1, mapCommonOreSmelter, LOCATION(118 + 31, 150 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(CS2, mapCommonOreSmelter, LOCATION(123 + 31, 150 - 1), aiNum, mapNone, 0);

	TethysGame::CreateBeacon(mapMiningBeacon, 122 + 31, 154 - 1, 0, 1, -1);
	TethysGame::CreateUnit(CM, mapCommonOreMine, LOCATION(122 + 31, 154 - 1), aiNum, mapNone, 0);

	TethysGame::CreateUnit(Unit1, mapTokamak, LOCATION(115 + 31, 135 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapTokamak, LOCATION(123 + 31, 135 - 1), aiNum, mapNone, 0);

	TethysGame::CreateUnit(Unit1, mapAgridome, LOCATION(122 + 31, 143 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapReinforcedResidence, LOCATION(118 + 31, 140 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapReinforcedResidence, LOCATION(122 + 31, 140 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapNursery, LOCATION(115 + 31, 143 - 1), aiNum, mapNone, 0);
	TethysGame::CreateUnit(Unit1, mapUniversity, LOCATION(115 + 31, 140 - 1), aiNum, mapNone, 0);

	TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(118 + 31, 148 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
	PlymouthSF.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapConVec, LOCATION(119 + 31, 148 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
	PlymouthSF.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(117 + 31, 152 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
	PlymouthCommon[0].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(118 + 31, 152 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
	PlymouthCommon[0].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(123 + 31, 152 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
	PlymouthCommon[1].TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapCargoTruck, LOCATION(124 + 31, 152 - 1), aiNum, mapNone, 0);
	Unit1.DoSetLights(1);
	PlymouthCommon[1].TakeUnit(Unit1);

	for (int mics = 0; mics < 6 + numHumansEasy + (2 * numHumansHard) + (3 * numHumansHard); mics++)
	{
		TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(108 + 31, 150 - 1 + mics), aiNum, mapMicrowave, 0);
		Unit1.DoSetLights(1);
		PlymouthDefense.TakeUnit(Unit1);
	}
	for (int foams = 0; foams < 3 + numHumansNormal + numHumansHard; foams++)
	{
		TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(109 + 31, 150 - 1 + foams), aiNum, mapStickyfoam, 0);
		Unit1.DoSetLights(1);
		PlymouthDefense.TakeUnit(Unit1);
	}
	for (int emps = 0; emps < numHumansHard; emps++)
	{
		TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(110 + 31, 150 - 1 + emps), aiNum, mapEMP, 0);
		Unit1.DoSetLights(1);
		PlymouthDefense.TakeUnit(Unit1);
	}
	/*
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(108 + 31, 150 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(108 + 31, 151 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(108 + 31, 152 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(108 + 31, 153 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(108 + 31, 154 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(108 + 31, 155 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(108 + 31, 156 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(108 + 31, 157 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(109 + 31, 157 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(109 + 31, 156 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(109 + 31, 155 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(109 + 31, 154 - 1), aiNum, mapMicrowave, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(112 + 31, 156 - 1), aiNum, mapStickyfoam, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(112 + 31, 155 - 1), aiNum, mapStickyfoam, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(112 + 31, 154 - 1), aiNum, mapStickyfoam, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(112 + 31, 153 - 1), aiNum, mapStickyfoam, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(114 + 31, 156 - 1), aiNum, mapEMP, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(114 + 31, 155 - 1), aiNum, mapEMP, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(114 + 31, 154 - 1), aiNum, mapEMP, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	TethysGame::CreateUnit(Unit1, mapLynx, LOCATION(114 + 31, 153 - 1), aiNum, mapEMP, 0);
	Unit1.DoSetLights(1);
	PlymouthDefense.TakeUnit(Unit1);
	*/

	// Mining groups
	PlymouthCommon[0].Setup(CM, CS1, MAP_RECT(118 + 31, 150 - 1, 123 + 31, 155 - 1));
	PlymouthCommon[1].Setup(CM, CS2, MAP_RECT(120 + 31, 150 - 1, 125 + 31, 155 - 1));

	// Guard Posts
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(103 + 31, 174 - 1), aiNum, mapMicrowave, 0);
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(115 + 31, 174 - 1), aiNum, mapMicrowave, 0);
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(123 + 31, 175 - 1), aiNum, mapMicrowave, 0);
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(126 + 31, 127 - 1), aiNum, mapMicrowave, 0);
	TethysGame::CreateUnit(Unit1, mapGuardPost, LOCATION(99 + 31, 146 - 1), aiNum, mapMicrowave, 0);

}

void FraxSurvAI::PrepareUnitGroupsPlymouth()
{
	// VFs
	PlymouthVF[0] = CreateBuildingGroup(aiNum);
	PlymouthVF[1] = CreateBuildingGroup(aiNum);

	// Setup mining groups
	PlymouthCommon[0] = CreateMiningGroup(aiNum);
	PlymouthCommon[0].SetTargCount(mapCargoTruck, mapNone, 2);
	PlymouthVF[0].RecordVehReinforceGroup(PlymouthCommon[0], 8000);

	PlymouthCommon[1] = CreateMiningGroup(aiNum);
	PlymouthCommon[1].SetTargCount(mapCargoTruck, mapNone, 2);
	PlymouthVF[0].RecordVehReinforceGroup(PlymouthCommon[1], 8000);

	PlymouthSF = CreateBuildingGroup(aiNum);
	PlymouthSF.SetTargCount(mapConVec, mapNone, 2);
	PlymouthSF.SetRect(MAP_RECT(115 + 31, 147 - 1, 120 + 31, 149 - 1));
	PlymouthVF[0].RecordVehReinforceGroup(PlymouthCommon[0], 5000);

	PlymouthEtc = CreateBuildingGroup(aiNum);
	PlymouthEtc.SetTargCount(mapRoboMiner, mapNone, 2);
	PlymouthEtc.SetTargCount(mapEarthworker, mapNone, 2);
	PlymouthEtc.SetRect(MAP_RECT(125 + 31, 140 - 1, 128 + 31, 144 - 1));
	PlymouthVF[0].RecordVehReinforceGroup(PlymouthEtc, 3000);

	PlymouthDefense = CreateFightGroup(aiNum);
	PlymouthDefense.SetTargCount(mapLynx, mapMicrowave, 6 + numHumansEasy + (2 * numHumansHard) + (3 * numHumansHard));
	PlymouthDefense.SetTargCount(mapLynx, mapStickyfoam, 3 + numHumansNormal + numHumansHard);
	PlymouthDefense.SetTargCount(mapLynx, mapEMP, numHumansHard);
	PlymouthDefense.SetRect(MAP_RECT(107 + 31, 148 - 1, 115 + 31, 157 - 1));
	PlymouthDefense.AddGuardedRect(MAP_RECT(109 + 31, 130 - 1, 128 + 31, 142 - 1));
	PlymouthDefense.AddGuardedRect(MAP_RECT(103 + 31, 145 - 1, 128 + 31, 157 - 1));
	PlymouthVF[0].RecordVehReinforceGroup(PlymouthDefense, 7000);

	PlymouthOffense = CreateFightGroup(aiNum);
	PlymouthOffense.SetRect(MAP_RECT(111 + 31, 158 - 1, 128 + 31, 163 - 1));
	PlymouthOffense.SetTargCount(mapLynx, mapMicrowave, (int)(3 + (numHumansEasy) + (numHumansNormal * 1.5) + (numHumansHard * 2)));	// Min: 9 Max: 14
	PlymouthVF[1].RecordVehReinforceGroup(PlymouthOffense, 8000);

	/*
	// These exist as a reference and provide a rough estimate of when each attack wave should be launched.  DO NOT UNCOMMENT!
	TrigAttackPlym = CreateTimeTrigger(1, 1, 22400, 22600, "Plymouth_Attack1");
	CreateTimeTrigger(1, 1, 29800, 30100, "Plymouth_Attack2");
	CreateTimeTrigger(1, 1, 37000, 37100, "Plymouth_Attack3");
	CreateTimeTrigger(1, 1, 43500, 43600, "Plymouth_Attack4");
	CreateTimeTrigger(1, 1, 50600, 50700, "Plymouth_Attack5");
	CreateTimeTrigger(1, 1, 58200, 58300, "Plymouth_Attack6");
	CreateTimeTrigger(1, 1, 64700, 64900, "Plymouth_Attack7");
	CreateTimeTrigger(1, 1, 72000, 72100, "Plymouth_Attack8");
	CreateTimeTrigger(1, 1, 78400, 78600, "Plymouth_Attack9");
	CreateTimeTrigger(1, 1, 83200, 83600, "Plymouth_AttackFinal");
	*/
	odprintf("End SetupOutpost2\n");
}

void FraxSurvAI::RecordStructuresPlymouth()
{
	PlymouthSF.RecordBuilding(LOCATION(118 + 31, 150 - 1), mapCommonOreSmelter, mapNone, PlymouthCommon[0]);
	PlymouthSF.RecordBuilding(LOCATION(123 + 31, 150 - 1), mapCommonOreSmelter, mapNone, PlymouthCommon[1]);
	PlymouthSF.RecordBuilding(LOCATION(123 + 31, 146 - 1), mapVehicleFactory, mapNone, PlymouthVF[0]);
	PlymouthSF.RecordBuilding(LOCATION(113 + 31, 146 - 1), mapVehicleFactory, mapNone, PlymouthVF[1]);
	PlymouthSF.RecordBuilding(LOCATION(123 + 31, 135 - 1), mapTokamak, mapNone);
	PlymouthSF.RecordBuilding(LOCATION(115 + 31, 135 - 1), mapTokamak, mapNone);
	PlymouthSF.RecordBuilding(LOCATION(122 + 31, 143 - 1), mapAgridome, mapNone);
	PlymouthSF.RecordBuilding(LOCATION(115 + 31, 143 - 1), mapNursery, mapNone);
	PlymouthSF.RecordBuilding(LOCATION(115 + 31, 140 - 1), mapUniversity, mapNone);
	PlymouthSF.RecordBuilding(LOCATION(118 + 31, 140 - 1), mapReinforcedResidence, mapNone);
	PlymouthSF.RecordBuilding(LOCATION(122 + 31, 140 - 1), mapReinforcedResidence, mapNone);

}