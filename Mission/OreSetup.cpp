#include <Tethys/API/API.h>
#include "..\Patches\RandomResources.h"

#include "..\functions.h"	// temp

#define LocATION(A) (LOCATION(A.x, A.y))	// Convert a TethysAPI Location to built-in OP2 LOCATION

// Override RNG for determining ore sets.  Use for debugging.
//#define RandomOverrideS 101;
//#define RandomOverrideM 2;
//#define RandomOverrideU 2;

using namespace TethysAPI;

// ------------------------------------------------------------------------------------------------
void SouthernOre_Set1()
{
	// Western side
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(12, 180), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(19, 192), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(10, 200), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(39, 187), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);

	// Eastern side
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(107, 188), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(120, 198), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);

	// Middle
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(67, 186), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(79, 182), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(90, 195), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);

	// Rare
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(104, 180), MineType::RareOre, Tethys::OreYield::Bar1, Tethys::OreVariant::Low);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(35, 206), MineType::RareOre, Tethys::OreYield::Bar1, Tethys::OreVariant::Low);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(68, 211), MineType::RareOre, Tethys::OreYield::Bar1, Tethys::OreVariant::Low);

	// Fumaroles
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(102, 204), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(40, 179), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
}

void SouthernOre_Set2()
{
	// Western side
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(30, 179), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(9, 190), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(19, 208), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);

	// Eastern side
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(121, 187), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(109, 195), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(107, 184), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(123, 205), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);

	// Middle
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(67, 191), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(81, 200), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(49, 195), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);

	// Rare
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(99, 205), MineType::RareOre, Tethys::OreYield::Bar1, Tethys::OreVariant::Low);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(126, 181), MineType::RareOre, Tethys::OreYield::Bar1, Tethys::OreVariant::Low);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(9, 177), MineType::RareOre, Tethys::OreYield::Bar1, Tethys::OreVariant::Low);

	// Fumaroles
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(40, 199), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(116, 196), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
}

void SouthernOre_Set3()
{
	// Western side
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(6, 181), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(26, 186), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);

	// Eastern side
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(120, 186), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(107, 192), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(126, 194), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);

	// Middle
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(66, 190), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(83, 178), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(62, 182), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(89, 193), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(53, 196), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);

	// Rare
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(31, 173), MineType::RareOre, Tethys::OreYield::Bar1, Tethys::OreVariant::Low);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(57, 176), MineType::RareOre, Tethys::OreYield::Bar1, Tethys::OreVariant::Low);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(118, 193), MineType::RareOre, Tethys::OreYield::Bar1, Tethys::OreVariant::Low);

	// Fumaroles
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(91, 211), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(40, 179), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
}

void SouthernOre_Set4()	// "Rare" set 1 - loads of common, but no rare
{
	// Western side
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(13, 179), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(31, 183), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(14, 186), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(24, 194), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(8, 191), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);

	// Eastern side
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(109, 186), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(123, 191), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(126, 198), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(110, 197), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(120, 207), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);

	// Middle
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(48, 194), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(65, 183), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(86, 190), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(76, 196), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(63, 197), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(91, 208), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
}

void SouthernOre_Set5()	// "Rare" set 2 - limited common, but tons of rare
{
	// Western side
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(30, 182), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(14, 188), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);

	// Eastern side
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(121, 192), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);

	// Middle
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(65, 181), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(84, 190), MineType::CommonOre, Tethys::OreYield::Bar3, Tethys::OreVariant::High);

	// Rare
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(7, 180), MineType::RareOre, Tethys::OreYield::Bar2, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(21, 197), MineType::RareOre, Tethys::OreYield::Bar2, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(48, 195), MineType::RareOre, Tethys::OreYield::Bar2, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(67, 193), MineType::RareOre, Tethys::OreYield::Bar2, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(81, 177), MineType::RareOre, Tethys::OreYield::Bar2, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(79, 204), MineType::RareOre, Tethys::OreYield::Bar2, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(107, 186), MineType::RareOre, Tethys::OreYield::Bar2, Tethys::OreVariant::High);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(120, 205), MineType::RareOre, Tethys::OreYield::Bar2, Tethys::OreVariant::High);

	// Fumaroles
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(101, 216), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(47, 201), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
}

// ------------------------------------------------------------------------------------------------

void MiddleOre_Set1()
{
	RandomRes r = RandomRes(MAP_RECT(LOCATION(1, 1), LOCATION(1, 1)), 0, 0, 0, 0, 0);

	// Common
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(19, 111)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(15, 89)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(88, 111)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(60, 125)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(73, 131)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(78, 143)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(30, 138)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(55, 143)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(121, 131)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(124, 167)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::Mid, true);

	// Rare
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(37, 104)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(97, 124)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(62, 110)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(100, 135)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(3, 139)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(121, 144)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(73, 121)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(106, 159)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);

	// Fumaroles
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(42, 120), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(23, 130), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(118, 123), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(73, 116), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(104, 140), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(61, 151), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);

	// Magma vents
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(54, 154), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(56, 101), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
}

void MiddleOre_Set2()
{
	RandomRes r = RandomRes(MAP_RECT(LOCATION(1, 1), LOCATION(1, 1)), 0, 0, 0, 0, 0);

	// Common
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(19, 107)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(38, 106)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(58, 152)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(87, 138)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(90, 120)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(78, 112)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(63, 134)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);

	// Rare
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(24, 122)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(17, 94)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(61, 112)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(104, 126)), (int)MineType::RareOre, (int)Tethys::OreYield::Random, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(76, 125)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(85, 129)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(111, 150)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Random, true);

	// Fumaroles
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(26, 111), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(88, 107), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(40, 122), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(126, 131), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(25, 91), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(62, 138), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);

	// Magma vents
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(8, 113), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(109, 119), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(51, 142), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
}

void MiddleOre_Set3()
{
	RandomRes r = RandomRes(MAP_RECT(LOCATION(1, 1), LOCATION(1, 1)), 0, 0, 0, 0, 0);

	// Common
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(86, 114)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(61, 130)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(39, 120)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(43, 102)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(10, 118)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(91, 138)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(82, 130)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(106, 153)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(25, 136)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::Low, true);

	// Rare
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(78, 146)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(50, 140)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(72, 120)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(101, 128)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(8, 98)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(119, 137)), (int)MineType::RareOre, (int)Tethys::OreYield::Random, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(20, 127)), (int)MineType::RareOre, (int)Tethys::OreYield::Random, (int)Tethys::OreVariant::Random, true);

	// Fumaroles
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(93, 155), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(117, 128), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(12, 81), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);

	// Magma vents
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(107, 117), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(19, 111), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
}

// ------------------------------------------------------------------------------------------------

void UpperOre_Set1()
{
	RandomRes r = RandomRes(MAP_RECT(LOCATION(1, 1), LOCATION(1, 1)), 0, 0, 0, 0, 0);

	// Common
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(120, 53)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(106, 55)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(115, 70)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(100, 86)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(79, 85)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(37, 50)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(46, 55)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(15, 20)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(69, 52)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::High, true);

	// Rare
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(119, 61)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(123, 89)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(67, 76)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(31, 59)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(49, 72)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(15, 34)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);
	

	// Fumaroles
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(121, 101), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(125, 41), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(16, 41), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(29, 72), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(86, 46), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);

	// Magma vents
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(102, 99), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(76, 59), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(62, 60), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
}

void UpperOre_Set2()
{
	RandomRes r = RandomRes(MAP_RECT(LOCATION(1, 1), LOCATION(1, 1)), 0, 0, 0, 0, 0);

	// Common
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(16, 14)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(23, 34)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(12, 38)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(53, 69)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(88, 68)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(92, 90)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(107, 64)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(111, 33)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::High, true);

	// Rare
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(8, 5)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(46, 42)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(13, 63)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(63, 49)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(119, 72)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(122, 27)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(73, 81)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(119, 49)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);

	// Fumaroles
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(42, 26), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(60, 79), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(120, 84), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(97, 69), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);

	// Magma vents
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(20, 74), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(100, 98), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(87, 94), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
}

void UpperOre_Set3()
{
	RandomRes r = RandomRes(MAP_RECT(LOCATION(1, 1), LOCATION(1, 1)), 0, 0, 0, 0, 0);

	// Common
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(125, 40)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(123, 57)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(94, 82)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(81, 74)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(54, 73)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(36, 65)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(40, 53)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(16, 22)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(11, 42)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(12, 69)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(116, 102)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);

	// Rare
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(118, 41)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(105, 55)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(120, 74)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(8, 14)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(47, 40)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(20, 57)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(90, 64)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::High, true);

	// Fumaroles
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(42, 26), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(120, 85), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(63, 84), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(23, 3), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(78, 52), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(60, 61), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);

	// Magma vents
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(20, 74), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(99, 97), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(108, 95), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
}

void UpperOre_Set4()
{
	RandomRes r = RandomRes(MAP_RECT(LOCATION(1, 1), LOCATION(1, 1)), 0, 0, 0, 0, 0);

	// Common
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(125, 68)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(108, 61)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(101, 43)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(122, 101)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(106, 83)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(88, 96)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(88, 68)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(75, 56)), (int)MineType::CommonOre, (int)Tethys::OreYield::Random, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(60, 58)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(17, 39)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(32, 32)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(39, 47)), (int)MineType::CommonOre, (int)Tethys::OreYield::Random, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(51, 52)), (int)MineType::CommonOre, (int)Tethys::OreYield::Random, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(51, 60)), (int)MineType::CommonOre, (int)Tethys::OreYield::Random, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(44, 70)), (int)MineType::CommonOre, (int)Tethys::OreYield::Random, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(48, 77)), (int)MineType::CommonOre, (int)Tethys::OreYield::Random, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(63, 80)), (int)MineType::CommonOre, (int)Tethys::OreYield::Random, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(122, 25)), (int)MineType::CommonOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Random, true);

	// Rare
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(125, 53)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(3, 18)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(10, 30)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::Mid, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(43, 39)), (int)MineType::RareOre, (int)Tethys::OreYield::Random, (int)Tethys::OreVariant::Random, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(17, 69)), (int)MineType::RareOre, (int)Tethys::OreYield::Random, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(92, 76)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar3, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(76, 76)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar1, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(55, 44)), (int)MineType::RareOre, (int)Tethys::OreYield::Bar2, (int)Tethys::OreVariant::Low, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(17, 69)), (int)MineType::RareOre, (int)Tethys::OreYield::Random, (int)Tethys::OreVariant::High, true);
	r.AddBeacon((map_id)Tethys::MapID::MiningBeacon, LocATION(TethysAPI::GameMap::At(17, 69)), (int)MineType::RareOre, (int)Tethys::OreYield::Random, (int)Tethys::OreVariant::High, true);

	// Fumaroles
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(119, 74), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(11, 10), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(28, 58), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(61, 37), MineType::Fumarole, Tethys::OreYield::Random, Tethys::OreVariant::Random);

	// Magma vents
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(74, 46), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(81, 50), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
	Tethys::TethysAPI::TethysGame::CreateMine(TethysAPI::GameMap::At(97, 96), MineType::MagmaVent, Tethys::OreYield::Random, Tethys::OreVariant::Random);
}

// ------------------------------------------------------------------------------------------------

void PlaceOre()
{
	int r = TethysAPI::TethysGame::GetRand(102);

#ifdef RandomOverrideS
	r = RandomOverrideS;
#endif

	// Southern/starting area ore
	if (r < 34)
	{
		SouthernOre_Set1();
	}
	else if (r < 67)
	{
		SouthernOre_Set2();
	}
	else if (r < 100)
	{
		SouthernOre_Set3();
	}
	else if (r == 100)
	{
		SouthernOre_Set4();
	}
	else if (r == 101)
	{
		SouthernOre_Set5();
	}
	else
	{
		//TethysAPI::TethysGame::AddMessage("Learn how GetRand works!", SoundID::Savnt227, -1);
	}

	// Everything south of the lava chokepoint and north of the starting area (~y 92 - 141)
	r = TethysAPI::TethysGame::GetRand(3);
#ifdef RandomOverrideM
	r = RandomOverrideM;
#endif
	switch (r)
	{
	case 0:
		MiddleOre_Set1();
		break;
	case 1:
		MiddleOre_Set2();
		break;
	case 2:
		MiddleOre_Set3();
		break;
	default:
		break;
	}

	// End zone -- Note that ore setup #4 is exteremely generous, and overrides the random ore selection if all players are playing on easy.
	bool useSet4 = true;
	for (int i = 0; i < TethysAPI::TethysGame::NumPlayers(); i++)
	{
		if (TethysAPI::Player[i].IsAI())
		{
			continue;
		}
		else if (TethysAPI::Player[i].GetDifficulty() != Tethys::DifficultyLevel::Easy)
		{
			useSet4 = false;
			break;
		}
	}
	if (useSet4)
	{
		r = 3;
	}
	else
	{
		r = TethysAPI::TethysGame::GetRand(3);
	}
	
#ifdef RandomOverrideU
	r = RandomOverrideU;
#endif
	switch (r)
	{
	case 0:
		UpperOre_Set1();
		break;
	case 1:
		UpperOre_Set2();
		break;
	case 2:
		UpperOre_Set3();
		break;
	case 3:
		UpperOre_Set4();
		break;
	default:
		break;
	}
}

// ------------------------------------------------------------------------------------------------
