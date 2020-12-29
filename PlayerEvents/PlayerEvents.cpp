#include <Outpost2DLL.h>
#include <OP2Helper.h>
#include <hfl.h>
#include "PlayerEvents.h"
#include "..\OP2Types\OP2Units.h"

Trigger playerRebuiltAgridome[7], shareMicrobeWallTech, shareLavaWallTech;
bool shareTechEventsCreated = false;

namespace
{
	int oldFoodStored[7];
}


void CreatePlayerEvents(int playerId)
{
	char trigName[3][0x64];

	if (!shareTechEventsCreated)
	{
		shareMicrobeWallTech = CreateResearchTrigger(1, 1, 8403, -1, "ShareMicrobeWallTech");
		shareLavaWallTech = CreateResearchTrigger(1, 1, 5702, -1, "ShareLavaWallTech");
		shareTechEventsCreated = true;
	}

	if (!isInited || playerId > 6 || playerId < 0)
		return;

	if (Player[playerId].IsHuman() == 0)
		return;

	oldFoodStored[playerId] = -1;

	scr_snprintf(trigName[0], sizeof(trigName[0]), "NoStructures_P%i", playerId);
	CreateBuildingCountTrigger(1, 0, playerId, 0, cmpLowerEqual, trigName[0]);

	scr_snprintf(trigName[1], sizeof(trigName[1]), "NoAgridomes_P%i", playerId);
	CreateCountTrigger(1, 0, playerId, mapAgridome, mapAny, 0, cmpLowerEqual, trigName[1]);

	scr_snprintf(trigName[2], sizeof(trigName[2]), "RebuiltAgridome_P%i", playerId);
	playerRebuiltAgridome[playerId] = CreateCountTrigger(0, 0, playerId, mapAgridome, mapAny, 1, cmpGreaterEqual, trigName[2]);
}


void ShareMicrobeWallTech()
{
	for (int i = 0; i < TethysGame::NoPlayers(); i ++)
	{
		// For everyone else, send a message indicating they're getting the tech and mark it off
		if (!Player[i].HasTechnology(8403))
		{
			TethysGame::AddMessage(-1, -1, "Receiving Microbe Wall upgrade...", i, sndLab_3);
			Player[i].MarkResearchComplete(8403);
		}
		// For the player who researched it, notify them they're being forced to share the tech
		else
			TethysGame::AddMessage(-1, -1, "Transmitting Microbe Wall upgrade...", i, sndLab_3);
	}

	shareMicrobeWallTech.Destroy();
}


void ShareLavaWallTech()
{
	for (int i = 0; i < TethysGame::NoPlayers(); i ++)
	{
		// For everyone else, send a message indicating they're getting the tech and mark it off
		if (!Player[i].HasTechnology(5702))
		{
			TethysGame::AddMessage(-1, -1, "Receiving Lava Wall upgrade...", i, sndLab_3);
			Player[i].MarkResearchComplete(5702);
		}
		// For the player who researched it, notify them they're being forced to share the tech
		else
			TethysGame::AddMessage(-1, -1, "Transmitting Lava Wall upgrade...", i, sndLab_3);
	}

	shareLavaWallTech.Destroy();
}




void NoStructures(int playerId)
{
	int totalPopulation = 0, totalEvacCapacity = 0;
	UnitEx unit;
	PlayerVehicleEnum evacEnum(playerId);

	if (!isInited || playerId > 6 || playerId < 0)
		return;

	if (Player[playerId].IsHuman() == 0)
		return;

	totalPopulation = Player[playerId].Workers() + Player[playerId].Scientists() + Player[playerId].Kids();
	if (totalPopulation <= 0)
		return;

	while (evacEnum.GetNext(unit))
	{
		if (unit.IsLive())
		{
			if (unit.GetType() == mapEvacuationTransport)
				totalEvacCapacity += unit.GetUnitInfo().GetNumStorageBays(playerId);
		}
	}

	if (totalEvacCapacity >= totalPopulation)
		return;

	ExtPlayer[playerId].Starve(totalPopulation - totalEvacCapacity, 0);
	ExtPlayer[playerId].RecalculateValues();
}


void NoAgridomes(int playerId)
{
	UnitEx truck;
	PlayerVehicleEnum truckEnum(playerId);
	int totalFoodCargo = 0, foodConsumed = 0;

	if (!isInited || playerId > 6 || playerId < 0)
		return;

	if (Player[playerId].IsHuman() == 0)
		return;

	if (oldFoodStored[playerId] == -1)
	{
		playerRebuiltAgridome[playerId].Enable();
		oldFoodStored[playerId] = Player[playerId].FoodStored();
	}

	foodConsumed = oldFoodStored[playerId] - Player[playerId].FoodStored();

	while (truckEnum.GetNext(truck))
	{
		if (truck.IsLive())
		{
			if (truck.GetType() == mapCargoTruck && truck.GetCargoType() == truckFood)
			{
				if (truck.GetCargoAmount() < foodConsumed)
				{
					foodConsumed -= truck.GetCargoAmount();
					truck.SetTruckCargo(truckEmpty, 0);
				}
				else if (foodConsumed > 0)
				{
					if ((truck.GetCargoAmount() - foodConsumed) > 0)
						truck.SetTruckCargo(truckFood, truck.GetCargoAmount() - foodConsumed);
					else
						truck.SetTruckCargo(truckEmpty, 0);
					foodConsumed = 0;
				}
				totalFoodCargo += truck.GetCargoAmount();
			}
		}
	}
	if (Player[playerId].FoodStored() != totalFoodCargo)
		Player[playerId].SetFoodStored(totalFoodCargo);

	oldFoodStored[playerId] = Player[playerId].FoodStored();
}

void RebuiltAgridome(int playerId)
{
	UnitEx agridome, truck;
	PlayerBuildingEnum agriEnum(playerId, mapAgridome);
	bool constructedAgridome = false;
	PlayerVehicleEnum truckEnum(playerId);
	int totalFoodCargo = 0;

	if (!isInited || playerId > 6 || playerId < 0)
		return;

	if (Player[playerId].IsHuman() == 0)
		return;

	while (agriEnum.GetNext(agridome))
	{
		if (agridome.IsLive())
		{
			if (agridome.GetLastCommand() == ctMoDevelop
				|| agridome.GetLastCommand() == ctMoUnDevelop
				|| agridome.GetLastCommand() == ctMoDeath)
				continue;

			constructedAgridome = true;
		}
	}

	if (!constructedAgridome)
		return;

	while (truckEnum.GetNext(truck))
	{
		if (truck.IsLive())
		{
			if (truck.GetType() == mapCargoTruck && truck.GetCargoType() == truckFood)
			{
				totalFoodCargo += truck.GetCargoAmount();
				truck.SetTruckCargo(truckEmpty, 0);
			}
		}
	}
	if (totalFoodCargo > 0)
		TethysGame::AddMessage(-1, -1, "All food cargo has been unloaded.", playerId, sndAg_sel);

	oldFoodStored[playerId] = -1;
	playerRebuiltAgridome[playerId].Disable();
}


void NoStructures_P0() { NoStructures(0); }
void NoStructures_P1() { NoStructures(1); }
void NoStructures_P2() { NoStructures(2); }
void NoStructures_P3() { NoStructures(3); }
void NoStructures_P4() { NoStructures(4); }
void NoStructures_P5() { NoStructures(5); }
void NoStructures_P6() { NoStructures(6); }


void NoAgridomes_P0() { NoAgridomes(0); }
void NoAgridomes_P1() { NoAgridomes(1); }
void NoAgridomes_P2() { NoAgridomes(2); }
void NoAgridomes_P3() { NoAgridomes(3); }
void NoAgridomes_P4() { NoAgridomes(4); }
void NoAgridomes_P5() { NoAgridomes(5); }
void NoAgridomes_P6() { NoAgridomes(6); }

void RebuiltAgridome_P0() { RebuiltAgridome(0); }
void RebuiltAgridome_P1() { RebuiltAgridome(1); }
void RebuiltAgridome_P2() { RebuiltAgridome(2); }
void RebuiltAgridome_P3() { RebuiltAgridome(3); }
void RebuiltAgridome_P4() { RebuiltAgridome(4); }
void RebuiltAgridome_P5() { RebuiltAgridome(5); }
void RebuiltAgridome_P6() { RebuiltAgridome(6); }