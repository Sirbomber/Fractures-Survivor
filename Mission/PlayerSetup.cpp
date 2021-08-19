#include <Tethys/API/API.h>

#include "..\functions.h"	// temp

using namespace Tethys;
using namespace Tethys::TethysAPI;

void SpawnPlayer(int i, int x, int y)
{
	Unit Unit1;
	Player[i].CenterViewOn(GameMap::At(x, y));

	// Land Rush style start.  Resource setting determines initial units.
	if (Player[i].GetDifficulty() == DifficultyLevel::Easy)
	{
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x, y), i, MapID::CommandCenter, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 1, y), i, MapID::StructureFactory, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 2, y), i, MapID::CommonOreSmelter, UnitDirection::North, true);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x + 3, y), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::CommonMetal, 1500);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x + 4, y), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::CommonMetal, 1500);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x + 5, y), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::CommonMetal, 1500);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x, y + 1), i, MapID::Tokamak, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 1, y + 1), i, MapID::Agridome, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 2, y + 1), i, MapID::StandardLab, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::RoboMiner, GameMap::At(x + 3, y + 1), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::RoboSurveyor, GameMap::At(x + 4, y + 1), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 5, y + 1), i, MapID::CommandCenter, UnitDirection::North, true);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x, y + 2), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::Food, 1000);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x + 1, y + 2), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::Food, 1000);
		//TethysGame::CreateUnit(MapID::EvacuationTransport, GameMap::At(x + 2, y + 2), i, MapID::None, UnitDirection::North, true);
		//TethysGame::CreateUnit(MapID::EvacuationTransport, GameMap::At(x + 3, y + 2), i, MapID::None, UnitDirection::North, true);
		//TethysGame::CreateUnit(MapID::EvacuationTransport, GameMap::At(x + 4, y + 2), i, MapID::None, UnitDirection::North, true);
		//TethysGame::CreateUnit(MapID::EvacuationTransport, GameMap::At(x + 5, y + 2), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 2, y + 2), i, MapID::Nursery, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 3, y + 2), i, MapID::University, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 4, y + 2), i, MapID::VehicleFactory, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::Earthworker, GameMap::At(x + 5, y + 2), i, MapID::None, UnitDirection::North, true);
		
		Player[i].SetKids(25);
		Player[i].SetWorkers(36);
		Player[i].SetScientists(24);
		Player[i].SetFoodStored(4400);
		Player[i].MarkResearchComplete(2701);	// Astronomy
		Player[i].MarkResearchComplete(2702);   // Boptronics
		Player[i].MarkResearchComplete(2703);	// Chemistry
		Player[i].MarkResearchComplete(2704);	// Planetary Sciences
		Player[i].MarkResearchComplete(2705);   // Social Sciences
		Player[i].MarkResearchComplete(2707);	// Biology
		Player[i].MarkResearchComplete(2708);	// Earth Scientific Database
		Player[i].MarkResearchComplete(3201);	// Seismology
		Player[i].MarkResearchComplete(3304);   // Offspring Enhancement
		Player[i].MarkResearchComplete(3305);   // Research Training Programs
		Player[i].MarkResearchComplete(3401);   // Cybernetic Teleoperation
		Player[i].MarkResearchComplete(3402);   // High-Temperature Superconductivity
		Player[i].MarkResearchComplete(3407);   // Large-Scale Optical Resonators
		Player[i].MarkResearchComplete(3408);   // Focused Microwave Projection
		Player[i].MarkResearchComplete(3851);   // Mobile Weapons Platform
	}

	else if (Player[i].GetDifficulty() == DifficultyLevel::Normal)
	{
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x, y), i, MapID::CommandCenter, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x+1, y), i, MapID::StructureFactory, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x+2, y), i, MapID::CommonOreSmelter, UnitDirection::North, true);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x+3, y), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::CommonMetal, 1000);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x + 4, y), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::CommonMetal, 1000);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x + 5, y), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::CommonMetal, 1000);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x, y + 1), i, MapID::Tokamak, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 1, y + 1), i, MapID::Agridome, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 2, y + 1), i, MapID::BasicLab, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::RoboMiner, GameMap::At(x + 3, y + 1), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::RoboSurveyor, GameMap::At(x + 4, y + 1), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 5, y + 1), i, MapID::CommandCenter, UnitDirection::North, true);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x, y + 2), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::Food, 1800);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x + 1, y + 2), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::Food, 1800);
		TethysGame::CreateUnit(MapID::EvacuationTransport, GameMap::At(x + 2, y + 2), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::EvacuationTransport, GameMap::At(x + 3, y + 2), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::EvacuationTransport, GameMap::At(x + 4, y + 2), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::Earthworker, GameMap::At(x + 5, y + 2), i, MapID::None, UnitDirection::North, true);

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
	}

	else if (Player[i].GetDifficulty() == DifficultyLevel::Hard)
	{
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x, y), i, MapID::CommandCenter, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 1, y), i, MapID::StructureFactory, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 2, y), i, MapID::CommonOreSmelter, UnitDirection::North, true);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x + 3, y), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::CommonMetal, 1000);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x + 4, y), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::CommonMetal, 1000);
		TethysGame::CreateUnit(MapID::Earthworker, GameMap::At(x + 5, y), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x, y + 1), i, MapID::Tokamak, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 1, y + 1), i, MapID::Agridome, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 2, y + 1), i, MapID::BasicLab, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::RoboMiner, GameMap::At(x + 3, y + 1), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::RoboSurveyor, GameMap::At(x + 4, y + 1), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 5, y + 1), i, MapID::CommandCenter, UnitDirection::North, true);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x, y + 2), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::Food, 1400);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x + 1, y + 2), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::Food, 1400);
		TethysGame::CreateUnit(MapID::EvacuationTransport, GameMap::At(x + 2, y + 2), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::EvacuationTransport, GameMap::At(x + 3, y + 2), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::EvacuationTransport, GameMap::At(x + 4, y + 2), i, MapID::None, UnitDirection::North, true);

		Player[i].SetKids(18);
		Player[i].SetWorkers(28);
		Player[i].SetScientists(19);
		Player[i].SetFoodStored(2400);
		//Player[i].MarkResearchComplete(2701);   // Astronomy
		Player[i].MarkResearchComplete(3304);   // Offspring Enhancement
		Player[i].MarkResearchComplete(3407);   // Large-Scale Optical Resonators
		Player[i].MarkResearchComplete(3408);   // Focused Microwave Projection
	}

	/* v1.0 Hard
	else if (Player[i].GetDifficulty() == DifficultyLevel::Hard)
	{
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x, y), i, MapID::CommandCenter, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 1, y), i, MapID::StructureFactory, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 2, y), i, MapID::CommonOreSmelter, UnitDirection::North, true);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x + 3, y), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::CommonMetal, 1000);
		TethysGame::CreateUnit(MapID::Earthworker, GameMap::At(x + 4, y), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 1, y + 1), i, MapID::Agridome, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 2, y + 1), i, MapID::BasicLab, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::RoboMiner, GameMap::At(x + 3, y + 1), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::RoboSurveyor, GameMap::At(x + 4, y + 1), i, MapID::None, UnitDirection::North, true);
		Unit1 = TethysGame::CreateUnit(MapID::CargoTruck, GameMap::At(x, y + 2), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(CargoType::Food, 2000);
		TethysGame::CreateUnit(MapID::EvacuationTransport, GameMap::At(x + 1, y + 2), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::EvacuationTransport, GameMap::At(x + 2, y + 2), i, MapID::None, UnitDirection::North, true);
		TethysGame::CreateUnit(MapID::EvacuationTransport, GameMap::At(x + 3, y + 2), i, MapID::None, UnitDirection::North, true);

		Player[i].SetKids(14);
		Player[i].SetWorkers(24);
		Player[i].SetScientists(15);
		Player[i].SetFoodStored(2000);
		Player[i].MarkResearchComplete(3407);   // Large-Scale Optical Resonators
		Player[i].MarkResearchComplete(3408);   // Focused Microwave Projection
	}
	*/

	// Initial Vehicles
	MapID ivWpn = MapID::Microwave;
	if (Player[i].IsEden())
	{
		ivWpn = MapID::Laser;
	}

	switch (TethysGame::InitialUnits())
	{
	case 12:
		TethysGame::CreateUnit(MapID::Scout, GameMap::At(x + 5, y + 4), i, MapID::None, UnitDirection::North, true);
	case 11:
		TethysGame::CreateUnit(MapID::RoboDozer, GameMap::At(x + 4, y + 4), i, MapID::None, UnitDirection::North, true);
	case 10:
		TethysGame::CreateUnit(MapID::Earthworker, GameMap::At(x + 3, y + 4), i, MapID::None, UnitDirection::North, true);
	case 9:
		Unit1 = TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 2, y + 4), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(MapID::GuardPost, ivWpn);
	case 8:
		Unit1 = TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x + 1, y + 4), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(MapID::GuardPost, ivWpn);
	case 7:
		Unit1 = TethysGame::CreateUnit(MapID::ConVec, GameMap::At(x, y + 4), i, MapID::None, UnitDirection::North, true);
		Unit1.SetCargo(MapID::GuardPost, ivWpn);
	case 6:
		TethysGame::CreateUnit(MapID::RoboSurveyor, GameMap::At(x + 5, y + 3), i, MapID::None, UnitDirection::North, true);
	case 5:
		TethysGame::CreateUnit(MapID::RoboMiner, GameMap::At(x + 4, y + 3), i, MapID::None, UnitDirection::North, true);
	case 4:
		TethysGame::CreateUnit(MapID::Lynx, GameMap::At(x + 3, y + 3), i, ivWpn, UnitDirection::North, true);
	case 3:
		TethysGame::CreateUnit(MapID::Lynx, GameMap::At(x + 2, y + 3), i, ivWpn, UnitDirection::North, true);
	case 2:
		TethysGame::CreateUnit(MapID::Lynx, GameMap::At(x + 1, y + 3), i, ivWpn, UnitDirection::North, true);
	case 1:
		TethysGame::CreateUnit(MapID::Lynx, GameMap::At(x, y + 3), i, ivWpn, UnitDirection::North, true);
	default:
		// No initial vehicles.
		break;
	}

#ifdef _DEBUG
	Player[i].SetTechLevel(13);
	if (i == 0)
	{
		TethysGame::ForceMorale(MoraleLevel::Excellent);
	}
#endif

}

