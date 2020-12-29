#include <Outpost2DLL.h>
#include <OP2Helper.h>
#include <math.h>
#include <hfl.h>
#include "RandomResources.h"
#include "OP2Types\OP2Types.h"
#include "HookHelper.h"
#include "UnitHookHelper.h"
#include "ExtLocation.h"
#include "functions.h"

struct BeaconList beacons = BeaconList();
bool beaconVtblHooked = false;


RandomRes::RandomRes(struct MAP_RECT area, short numCommon, short numRare, short numFumaroles, short numMagma, char minDistance)
{
	if (isInited)
	{
		this->areaWidth = ((OP2Map*)mapObj)->tileWidth;
		this->areaHeight = ((OP2Map*)mapObj)->tileHeight;
		this->numElementsIncludedTiles = ((OP2Map*)mapObj)->numTiles;
		this->sizeIncludedTiles = sizeof(ResourceTileData) * this->numElementsIncludedTiles;
		this->includedTiles = new ResourceTileData[this->numElementsIncludedTiles];
		if (!this->includedTiles)
		{
			this->inited = false;
			return;
		}
	}
	else
	{
		LOCATION topLeft = LOCATION(-64, -64), bottomRight = LOCATION(512, 512);
		topLeft.Clip();
		bottomRight.Clip();

		this->areaWidth = (bottomRight.x - topLeft.x);
		this->areaHeight = (bottomRight.y - topLeft.y);
		this->numElementsIncludedTiles = this->areaWidth * this->areaHeight;
		this->sizeIncludedTiles = sizeof(ResourceTileData) * this->numElementsIncludedTiles;
		this->includedTiles = new ResourceTileData[this->numElementsIncludedTiles];
		if (!this->includedTiles)
		{
			this->inited = false;
			return;
		}
	}
	memset(this->includedTiles, 0, this->sizeIncludedTiles);

	this->SetToIncludedArea(area, 1);

	this->maxResources = numCommon + numRare + numFumaroles + numMagma;
	this->maxCommon = numCommon;
	this->maxRare = numRare;
	this->maxFumaroles = numFumaroles;
	this->maxMagma = numMagma;
	this->distance = minDistance;

	this->SetMax();
	this->SetMin();
	this->SetWeights();

	for (int i = 0; i < numof(this->resourcesCreated); i ++)
		this->resourcesCreated[i] = 0;
	for (int i = 0; i < numof(this->typesCreated); i ++)
		this->typesCreated[i] = 0;

	this->requireSmelterSpace = 0;
	this->noSmelterOverlap = false;
	this->maxSmelterSpacing = 0;

	this->fumarolesOnGrayRock = true;
	this->fumarolesOnLavaFlow = true;
	this->magmaOnLavaFlow = false;

	this->allowOrangeRock = true;
	this->allowGrayRock = true;
	this->allowBlackRock = true;
	this->allowLavaFlow = true;
	this->allowLavaPossible = true;

	this->scoutsFindBeacons = false;

	this->inited = true;
}
RandomRes::RandomRes()
{
	this->inited = false;
}


RandomRes::~RandomRes()
{
	if (this->includedTiles)
		delete [] this->includedTiles;
}


bool RandomRes::IsInited()
{
	return this->inited;
}


void RandomRes::SetToIncludedArea(MAP_RECT rect, char value, bool smelter)
{
	if (rect.y1 < 1) // Can't be testing tiles off the top edge of the map or OP2 will crash
		rect.y1 = 1;
	if (rect.y2 < 1)
		rect.y2 = 1;

	/*if (rect.x2 >= 507) // Can't properly test tiles on the side edge of world maps
	{
		rect.x2 = 507;
		if (rect.x1 <= 4)
			rect.x1 = 4;
	}*/

	for (int x = rect.x1; x <= rect.x2; x ++)
	{
		for (int y = rect.y1; y <= rect.y2; y ++)
			this->SetToIncludedArea(LOCATION(x, y), value, smelter);
	}
}
void RandomRes::SetToIncludedArea(LOCATION location, char value, bool smelter)
{
	if (!this->includedTiles)
		return;

	int offset = this->GetTileOffset(location);

	if (offset > this->numElementsIncludedTiles || offset < 0)
		return; // Out of bounds

	if (smelter)
		this->includedTiles[offset].smelterOrMine = value;
	else
		this->includedTiles[offset].canPlace = value;
}


void RandomRes::SetWeights(short common1, short common2, short common3, short rare1, short rare2, short rare3)
{
	this->commonWeight[0] = common1;
	this->commonWeight[1] = common2;
	this->commonWeight[2] = common3;
	this->rareWeight[0] = rare1;
	this->rareWeight[1] = rare2;
	this->rareWeight[2] = rare3;
}


void RandomRes::SetMin(short common1, short common2, short common3, short rare1, short rare2, short rare3, short fumaroles, short magma)
{
	this->commonRange[0][0] = (common1 <= this->commonRange[0][1]) ? common1 : this->commonRange[0][1];
	this->commonRange[1][0] = (common2 <= this->commonRange[1][1]) ? common2 : this->commonRange[1][1];
	this->commonRange[2][0] = (common2 <= this->commonRange[2][1]) ? common3 : this->commonRange[2][1];

	this->rareRange[0][0] = (rare1 <= this->rareRange[0][1]) ? rare1 : this->rareRange[0][1];
	this->rareRange[1][0] = (rare2 <= this->rareRange[1][1]) ? rare2 : this->rareRange[1][1];
	this->rareRange[2][0] = (rare2 <= this->rareRange[2][1]) ? rare3 : this->rareRange[2][1];

	this->fumaroleRange[0] = (fumaroles <= this->fumaroleRange[1]) ? fumaroles : this->fumaroleRange[1];
	this->magmaRange[0] = (magma <= this->magmaRange[1]) ? magma : this->magmaRange[1];
}


void RandomRes::SetMax(short common1, short common2, short common3, short rare1, short rare2, short rare3, short fumaroles, short magma)
{
	this->commonRange[0][1] = (common1 >= 0) ? common1 : this->maxCommon;
	this->commonRange[1][1] = (common2 >= 0) ? common2 : this->maxCommon;
	this->commonRange[2][1] = (common2 >= 0) ? common3 : this->maxCommon;

	this->rareRange[0][1] = (rare1 >= 0) ? rare1 : this->maxRare;
	this->rareRange[1][1] = (rare2 >= 0) ? rare2 : this->maxRare;
	this->rareRange[2][1] = (rare2 >= 0) ? rare3 : this->maxRare;

	this->fumaroleRange[1] = (fumaroles >= 0) ? fumaroles : this->maxFumaroles;
	this->magmaRange[1] = (magma >= 0) ? magma : this->maxMagma;

	this->CapAt(-1);
}


void RandomRes::CapAt(short max)
{
	int totalMax = this->maxCommon + this->maxRare + this->maxFumaroles + this->maxMagma;

	if (totalMax < max || max < 0)
		this->maxResources = totalMax;
	else
		this->maxResources = max;
}


bool RandomRes::AreaBlocked(MAP_RECT area, bool allowUnit, bool allowWallOrBuilding, MAP_RECT excludedRect)
{
	MapTile tile;
	LOCATION curLocation, excludeTestLocation;
	int blockedCellTypes[] = {cellImpassible2, cellImpassible1, cellNorthCliffs, cellCliffsHighSide, cellCliffsLowSide,
		cellVentsAndFumaroles, cellNormalWall, cellMicrobeWall, cellLavaWall};

	for (int x = area.x1; x <= area.x2; x ++) {
		for (int y = area.y1; y <= area.y2; y ++) {
			if (y < 0 || y > ((OP2Map*)mapObj)->tileHeight)
				return true;

			curLocation = LocationEx(x, y).Wrapped();

			if (this->GetTileOffset(curLocation) >= this->numElementsIncludedTiles)
				return true;

			if (excludedRect.x2 != -1 || excludedRect.y2 != -1)
			{
				for (int x2 = excludedRect.x1; x2 <= excludedRect.x2; x2 ++)
				{
					for (int y2 = excludedRect.y1; y2 <= excludedRect.y2; y2 ++)
					{
						excludeTestLocation = LocationEx(x2, y2).Wrapped();
						if (curLocation.x == excludeTestLocation.x && curLocation.y == excludeTestLocation.y)
							return true;
					}
				}
			}

			tile = GameMapEx::GetTileEx(curLocation);
			if (!allowLavaPossible && (tile.lavaPossible || tile.lava))
				return true;

			if (tile.unitIndex && !allowUnit)
				return true;

			if (tile.wallOrBuilding && !allowWallOrBuilding)
				return true;

			if (isInited)
			{
				int (__fastcall *IsDockLocation)(int x, int y) = (int (__fastcall*)(int,int))OP2Addr(0x438B70);
				if (IsDockLocation(SplitLoc(curLocation)))
					return true;
			}

			if (this->includedTiles[this->GetTileOffset(curLocation)].smelterOrMine)
				return true;

			for (int i = 0; i < numof(blockedCellTypes); i ++) {
				if (tile.cellType == blockedCellTypes[i])
					return true;
			}
		}
	}
	
	return false;
}


bool RandomRes::CanBuildSmelterAt(LOCATION beaconLocation, bool allowOnMine)
{
	MAP_RECT mineRect, baseSmelterRect, testRect, excludeRect;

	LOCATION returnedValue = LOCATION(-1, -1);

	int numSmeltersPossible;
	bool returnValue;

	mineRect = MAP_RECT(beaconLocation.x - 2, beaconLocation.y - 1, beaconLocation.x + 1, beaconLocation.y + 1);
	baseSmelterRect = MAP_RECT(1, 1, 5, 4);

	testRect = mineRect;
	testRect.Inflate(baseSmelterRect.Width() + this->maxSmelterSpacing, baseSmelterRect.Height() + this->maxSmelterSpacing);
	testRect.x2 -= (baseSmelterRect.Width());
	if (allowOnMine)
		testRect.x2 ++;
	testRect.y2 -= (baseSmelterRect.Height());

	if (!allowOnMine)
		excludeRect = MAP_RECT(beaconLocation.x - 1, beaconLocation.y, beaconLocation.x, beaconLocation.y);
	else
		excludeRect = MAP_RECT(-1, -1, -1, -1);

	for (int i = 0; i <= (int)(!allowOnMine); i ++)
	{
		returnValue = false;
		numSmeltersPossible = 0;

		testRect.Offset(i, 0);
		if (!allowOnMine)
		{
			excludeRect.Offset(i, 0);
			this->SetToIncludedArea(excludeRect, -1, true);
		}

		returnedValue = this->DoSmelterTest(testRect, baseSmelterRect, excludeRect);
		if (!(returnedValue.x == -1 && returnedValue.y == -1))
		{
			if (this->requireSmelterSpace > 1)
			{
				while (numSmeltersPossible < this->requireSmelterSpace)
				{
					if (numSmeltersPossible > 0)
						returnedValue = this->DoSmelterTest(testRect, baseSmelterRect, excludeRect);
					if (returnedValue.x == -1 && returnedValue.y == -1)
						break;
					else
					{
						this->SetToIncludedArea(MAP_RECT(returnedValue.x + 1, returnedValue.y + 1, returnedValue.x + baseSmelterRect.Width() - 1, returnedValue.y + baseSmelterRect.Height() - 1), -1, true);
						numSmeltersPossible ++;
					}
				}
				for (int x = testRect.x1; x <= testRect.x2 + baseSmelterRect.Width(); x ++)
				{
					for (int y = testRect.y1; y <= testRect.y2 + baseSmelterRect.Height(); y ++)
					{
						if (includedTiles[this->GetTileOffset(LOCATION(x, y))].smelterOrMine == -1)
							includedTiles[this->GetTileOffset(LOCATION(x, y))].smelterOrMine = (numSmeltersPossible == this->requireSmelterSpace && this->noSmelterOverlap) ? 1 : 0;
					}
				}
				if (numSmeltersPossible == this->requireSmelterSpace)
					returnValue = true;
				else
					returnValue = false;
			}
			else
			{
				if (this->noSmelterOverlap)
					this->SetToIncludedArea(MAP_RECT(returnedValue.x + 1, returnedValue.y + 1, returnedValue.x + baseSmelterRect.Width() - 1, returnedValue.y + baseSmelterRect.Height() - 1), 1, true);
				returnValue = true;
			}
		}

		testRect.Offset(-i, 0);
		if (!allowOnMine)
			excludeRect.Offset(-i, 0);

		if (returnValue == true)
			break;
	}

	return returnValue;
}
LOCATION RandomRes::DoSmelterTest(MAP_RECT testRect, MAP_RECT baseSmelterRect, MAP_RECT mineExcludeRect)
{
	bool allowOnWallOrBuilding = false;
	if (mineExcludeRect.x2 == -1 && mineExcludeRect.y2 == -1)
		allowOnWallOrBuilding = true;

	for (int x = testRect.x1; x <= testRect.x2; x ++)
	{
		for (int y = testRect.y1; y <= testRect.y2; y ++)
		{
			if (!this->AreaBlocked(MAP_RECT(x, y, x + baseSmelterRect.Width(), y + baseSmelterRect.Height()), true, allowOnWallOrBuilding, mineExcludeRect))
				return LOCATION(x, y);
		}
	}

	return LOCATION(-1, -1);
}


bool RandomRes::TestDistance(struct LOCATION loc)
{
	Unit unit;
	PlayerUnitEnum beaconEnum(6);
	while (beaconEnum.GetNext(unit))
	{
		if (unit.GetType() == mapMiningBeacon || unit.GetType() == mapMagmaVent || unit.GetType() == mapFumarole || unit.GetType() == mapWreckage)
		{
			if (sqrt(pow((double)(loc.x - unit.Location().x), 2) + pow((double)(loc.y - unit.Location().y), 2)) < distance)
				return true;
		}
	}
	if (beacons.numBeacons > 0 && !beaconVtblHooked)
	{
		for (int i = 0; i < beacons.numBeacons; i ++)
		{
			if (sqrt(pow((double)(loc.x - beacons.beacon[i].location.x), 2) + pow((double)(loc.y - beacons.beacon[i].location.y), 2)) < distance)
				return true;
		}
	}
	return false;
}


char RandomRes::GetTerrainType(struct LOCATION loc)
{
	int grayRockTiles[3][2] = {
		{1, 269},
		{379, 432},
		{951, 998}
	};

	int blackRockTiles[4][2] = {
		{439, 470},
		{505, 558},
		{607, 652},
		{886, 920}
	};

	int lavaFlowTiles[3][2] = {
		{471, 504},
		{559, 606},
		{653, 797}
	};

	int Tile = GameMap::GetTile(loc);
	if ((Tile >= grayRockTiles[0][0] && Tile <= grayRockTiles[0][1]) ||
		(Tile >= grayRockTiles[1][0] && Tile <= grayRockTiles[1][1]) ||
		(Tile >= grayRockTiles[2][0] && Tile <= grayRockTiles[2][1]))
	{
		return 1;
	}
	else if ((Tile >= blackRockTiles[0][0] && Tile <= blackRockTiles[0][1]) ||
		(Tile >= blackRockTiles[1][0] && Tile <= blackRockTiles[1][1]) ||
		(Tile >= blackRockTiles[2][0] && Tile <= blackRockTiles[2][1]) ||
		(Tile >= blackRockTiles[3][0] && Tile <= blackRockTiles[3][1]))
	{
		return 2;
	}
	else if ((Tile >= lavaFlowTiles[0][0] && Tile <= lavaFlowTiles[0][1]) ||
		(Tile >= lavaFlowTiles[1][0] && Tile <= lavaFlowTiles[1][1]) ||
		(Tile >= lavaFlowTiles[2][0] && Tile <= lavaFlowTiles[2][1]))
	{
		return 3;
	}
	return 0;
}


char RandomRes::NextRes()
{
	int minResources[] = {this->commonRange[0][0], this->commonRange[1][0], this->commonRange[2][0],
		this->rareRange[0][0], this->rareRange[1][0], this->rareRange[2][0],
		this->fumaroleRange[0], this->magmaRange[0]};

	for (int i = 0; i < 8; i ++)
	{
		if (this->resourcesCreated[i] < minResources[i])
			return (char)i;
	}
	return this->GetRandomRes();
}


char RandomRes::GetRandomRes()
{
	if (this->GetTotalCreated() >= this->maxResources)
		return -1;

	int res = -1;
	DWORD iterations = 0;
	int testType, testRes, testWeight;

	int maxResources[] = {this->commonRange[0][1], this->commonRange[1][1], this->commonRange[2][1],
		this->rareRange[0][1], this->rareRange[1][1], this->rareRange[2][1],
		this->fumaroleRange[1], this->magmaRange[1]};
	int maxTypes[] = {this->maxCommon, this->maxRare, this->maxFumaroles, this->maxMagma};

	int commonWeightTotal = this->commonWeight[0] + this->commonWeight[1] + this->commonWeight[2];
	int rareWeightTotal = this->rareWeight[0] + this->rareWeight[1] + this->rareWeight[2];

	while (res == -1)
	{
		iterations ++;
		if (iterations > maxInnerIterations)
		{
			break;
		}

		testType = TethysGame::GetRand(4);
		if (this->typesCreated[testType] < maxTypes[testType])
		{
			switch (testType)
			{
				case 0:
					testWeight = TethysGame::GetRand(commonWeightTotal + 1);
					for (int i = 0; i < 3; i ++)
					{
						testWeight -= this->commonWeight[i];
						if (testWeight <= 0)
						{
							testRes = i;
							break;
						}
					}
					break;
				case 1:
					testWeight = TethysGame::GetRand(rareWeightTotal + 1);
					for (int i = 0; i < 3; i ++)
					{
						testWeight -= this->rareWeight[i];
						if (testWeight <= 0)
						{
							testRes = i + 3;
							break;
						}
					}
					break;
				case 2:
				case 3:
					testRes = testType + 4;
					break;
				default:
					testRes = -1;
					break;
			}
			if (testRes != -1)
			{
				if (this->resourcesCreated[testRes] < maxResources[testRes])
					res = testRes;
			}
		}
	}
	return (char)res;
}


short RandomRes::GetTotalCreated()
{
	int total = 0;
	for (int i = 0; i < 8; i ++)
		total += this->resourcesCreated[i];
	return total;
}


int RandomRes::GetTileOffset(LOCATION location)
{
	if (isInited)
	{
		OP2Map *map = (OP2Map*)mapObj;
		return ((map->tileXMask & location.x) & 31) + (((((map->tileXMask & location.x) >> 5) << map->logTileHeight) + location.y) << 5);
	}
	else
		return (location.y * this->areaHeight) + location.x;
}


bool RandomRes::IsInIncludedArea(LOCATION location)
{
	if (!this->includedTiles)
		return false;

	int offset = this->GetTileOffset(location);

	if (offset > this->numElementsIncludedTiles || offset < 0)
		return false; // Out of bounds

	if (this->includedTiles[offset].canPlace == 1)
		return true;

	return false;
}


void RandomRes::AddBeacon(map_id beaconType, LOCATION location, int commonRareType, int barYield, int barVariant, bool hidden)
{
	if (beaconVtblHooked)
	{
		if (TethysGame::CreateBeacon(beaconType, location.x, location.y, commonRareType, barYield, barVariant))
		{
			if (beaconType == mapMiningBeacon && hidden)
			{
				beacons.numBeacons ++;
				SetBeaconIsVisible(((OP2Map*)mapObj)->lastUsedUnitIndex, -1, false);
			}
		}
	}
	else
	{
		if (beaconType == mapMiningBeacon && hidden == true)
		{
			beacons.numBeacons ++;
			beacons.beacon[beacons.numBeacons - 1].beaconType = beaconType;
			beacons.beacon[beacons.numBeacons - 1].location = location;
			beacons.beacon[beacons.numBeacons - 1].commonRareType = commonRareType;
			beacons.beacon[beacons.numBeacons - 1].barYield = barYield;
			beacons.beacon[beacons.numBeacons - 1].barVariant = barVariant;
			beacons.beacon[beacons.numBeacons - 1].isVisible = false;
		}
		else
			TethysGame::CreateBeacon(beaconType, location.x, location.y, commonRareType, barYield, barVariant);
	}
}


void RandomRes::Create()
{
	if (!this->inited)
		return;

	short created = this->GetTotalCreated();
	DWORD iterations = 0, iterations2 = 0;
	char res, terrain;
	LOCATION loc;
	MAP_RECT testRect;
	bool valid = false, canPlaceSmelter = false;

	MAP_RECT includedRect = MAP_RECT(this->areaWidth, this->areaHeight, 0, 0);
	for (int x = 0; x < this->areaWidth; x ++)
	{
		for (int y = 0; y < this->areaHeight; y ++)
		{
			if (this->IsInIncludedArea(LOCATION(x, y)))
			{
				if (x < includedRect.x1)
					includedRect.x1 = x;
				if (x > includedRect.x2)
					includedRect.x2 = x;
				if (y < includedRect.y1)
					includedRect.y1 = y;
				if (y > includedRect.y2)
					includedRect.y2 = y;

				if (!valid)
					valid = true;
			}
		}
	}
	//odprintf("start: %i,%i end: %i,%i", includedRect.x1, includedRect.y1, includedRect.x2, includedRect.y2);
	if (!valid)
		return;

	while (created < this->maxResources)
	{
		iterations ++;
		if (iterations > maxIterations)
			break;

		while (true)
		{
			iterations2 ++;
			if (iterations2 > maxInnerIterations)
			{
				loc = LocationEx(0, 0);
				break;
			}

			loc = LocationEx(TethysGame::GetRand(includedRect.x2 + 1 - includedRect.x1) + includedRect.x1, TethysGame::GetRand(includedRect.y2 + 1 - includedRect.y1) + includedRect.y1).Wrapped();
			if (this->IsInIncludedArea(loc))
				break;
		}

		if (this->TestDistance(loc) == true)
			continue;

		terrain = this->GetTerrainType(loc);
		switch (terrain)
		{
			case 0:
				if (!this->allowOrangeRock)
					continue;
				break;
			case 1:
				if (!this->allowGrayRock)
					continue;
				break;
			case 2:
				if (!this->allowBlackRock)
					continue;
				break;
			case 3:
				if (!this->allowLavaFlow)
					continue;
				break;
		}

		res = this->NextRes();

		terrain = this->GetTerrainType(loc);
		if (res == 6)
		{
			if (!((terrain == 1 && this->fumarolesOnGrayRock == true) || terrain == 2 || (terrain == 3 && this->fumarolesOnLavaFlow == true))
				|| GameMap::GetCellType(loc) == cellTube0)
				continue;
		}
		else if (res == 7)
		{
			if (!(terrain == 2 || (terrain == 3 && this->magmaOnLavaFlow == true))
				|| GameMap::GetCellType(loc) == cellTube0)
				continue;
		}

		testRect = MAP_RECT(loc.x - 2, loc.y - 1, loc.x + 1, loc.y + 1);
		if (this->AreaBlocked(testRect))
		{
			testRect.Offset(1, 0);
			if (this->AreaBlocked(testRect))
				continue;
		}

		if (res >= 0 && res != 6)
		{
			if (this->requireSmelterSpace != 0)
			{
				canPlaceSmelter = this->CanBuildSmelterAt(loc, (this->requireSmelterSpace >= 1) ? false : true);
				if ((!canPlaceSmelter && this->requireSmelterSpace >= 1) || (canPlaceSmelter && this->requireSmelterSpace < 0))
					continue;
			}
		}

		switch (res)
		{
			case 0:
			case 1:
			case 2:
				this->typesCreated[0] ++;
				AddBeacon(mapMiningBeacon, loc, 0, 2 - res, -1, scoutsFindBeacons);
				break;
			case 3:
			case 4:
			case 5:
				this->typesCreated[1] ++;
				AddBeacon(mapMiningBeacon, loc, 1, 2 - (res - 3), -1, scoutsFindBeacons);
				break;
			case 6:
				this->typesCreated[2] ++;
				AddBeacon(mapFumarole, loc, -1, -1, -1, false);
				break;
			case 7:
				this->typesCreated[3] ++;
				AddBeacon(mapMagmaVent, loc, -1, -1, -1, false);
				break;
			default:
				continue;
				break;
		}
		this->resourcesCreated[res] ++;
		created ++;
	}
}


double RandomRes::GetDistanceToBeacon(struct LOCATION location, short beaconId)
{
	if (beaconId > beacons.numBeacons)
		return -1;
	return sqrt(pow((double)(location.x - beacons.beacon[beaconId].location.x),2) + pow((double)(location.y - beacons.beacon[beaconId].location.y),2));
}


void RandomRes::DiscoverBeacon(short beaconId, int playerId, bool silent)
{
	bool wasVisible = false;
	char msgBuf[0x64];

	if (beaconVtblHooked)
	{
		if (!silent)
			wasVisible = IsBeaconVisible(beaconId, playerId);
		SetBeaconIsVisible(beaconId, playerId, true);

		if (playerId != 6 && !wasVisible && !silent)
		{
			UnitEx beacon;
			beacon.unitID = beaconId;

			if ((*unitArray)[beaconId].Beacon.type == 0)
				scr_snprintf(msgBuf, sizeof(msgBuf), "Common mining beacon located at %i,%i", beacon.Location().x - MapLocation().x, beacon.Location().y - MapLocation().y);
			else
				scr_snprintf(msgBuf, sizeof(msgBuf), "Rare mining beacon located at %i,%i", beacon.Location().x - MapLocation().x, beacon.Location().y - MapLocation().y);
			for (int i = 0; i < TethysGame::NoPlayers(); i ++)
			{
				if (ExtPlayer[playerId].IsAlliedTo(i))
					TethysGame::AddMessage(beacon, msgBuf, i, (sndMine_1 + (*unitArray)[beaconId].Beacon.type));
			}
		}

		beacons.beaconsDiscovered[playerId] ++;
	}
	else
	{
		BeaconList::BeaconData beacon = beacons.beacon[beaconId];
		LocationEx location = LocationEx(beacon.location);

		TethysGame::CreateBeacon(beacon.beaconType, location.x, location.y,	beacon.commonRareType, beacon.barYield, beacon.barVariant);

		if (beacon.commonRareType == 0)
			scr_snprintf(msgBuf, sizeof(msgBuf), "Common mining beacon located at %i,%i", location.x - MapLocation().x, location.y - MapLocation().y);
		else
			scr_snprintf(msgBuf, sizeof(msgBuf), "Rare mining beacon located at %i,%i", location.x - MapLocation().x, location.y - MapLocation().y);
		for (int i = 0; i < TethysGame::NoPlayers(); i ++)
		{
			if (ExtPlayer[playerId].IsAlliedTo(i))
				TethysGame::AddMessage(SplitLoc(location.ToPixels()), msgBuf, i, (sndMine_1 + beacon.commonRareType));
		}

		beacons.beacon[beaconId].isVisible = true;
		beacons.beaconsDiscovered[0] ++;
	}
}


void RandomRes::DiscoverAllBeacons(int playerId)
{
	if (beaconVtblHooked)
	{
		if (beacons.beaconsDiscovered[playerId] < beacons.numBeacons)
		{
			UnitEx curBeacon;
			PlayerUnitEnum beaconEnum(6);
			while (beaconEnum.GetNext(curBeacon))
			{
				if (curBeacon.GetType() == mapMiningBeacon)
				{
					if (!IsBeaconVisibleSelfOnly(curBeacon.unitID, playerId))
						DiscoverBeacon(curBeacon.unitID, playerId, true);
				}
			}
			TethysGame::AddMessage(-1, -1, "All mining beacons discovered", playerId, sndMine_2);
		}
	}
	else
	{
		if (beacons.beaconsDiscovered[0] < beacons.numBeacons)
		{
			for (int i = 0; i < beacons.numBeacons; i ++)
			{
				if (beacons.beacon[i].isVisible == false)
					DiscoverBeacon(i, 6);
			}
			TethysGame::AddMessage(-1, -1, "All mining beacons discovered", -1, sndMine_2);
		}
	}
}


void RandomRes::LocateBeaconProc(int playerId)
{
	double distance;
	int rareOreTech;

	if ((beaconVtblHooked && beacons.beaconsDiscovered[playerId] < beacons.numBeacons)
		|| (!beaconVtblHooked && beacons.beaconsDiscovered[0] < beacons.numBeacons))
	{
		UnitEx curScout, curBeacon;
		PlayerVehicleEnum scoutEnum(playerId);
		while (scoutEnum.GetNext(curScout))
		{
			if (!(curScout.GetType() == mapScout || curScout.GetType() == mapRoboSurveyor) || curScout.GetCurAction() != moMove)
				continue;

			if (beaconVtblHooked)
			{
				PlayerUnitEnum beaconEnum(6);
				while (beaconEnum.GetNext(curBeacon))
				{
					if (curBeacon.GetType() == mapMiningBeacon)
					{
						if (!IsBeaconVisibleSelfOnly(curBeacon.unitID, playerId))
						{
							rareOreTech = ((OP2Research*)researchObj)->techInfos[((OP2UnitInfo*)unitInfoArray[mapRareOreMine])->techId]->techID;
							if ((*unitArray)[curBeacon.unitID].Beacon.type == 1 && Player[playerId].HasTechnology(rareOreTech) == 0)
								continue;

							distance = sqrt(pow((double)(curBeacon.Location().x - curScout.Location().x),2) + pow((double)(curBeacon.Location().y - curScout.Location().y),2));

							if (distance < (((OP2UnitInfo*)unitInfoArray[curScout.GetType()])->playerData[playerId].sightRange + 1))
								DiscoverBeacon(curBeacon.unitID, playerId);
						}
					}
				}
			}
			else
			{
				for (int j = 0; j < beacons.numBeacons; j++)
				{
					if (beacons.beacon[j].isVisible == false)
					{
						rareOreTech = ((OP2Research*)researchObj)->techInfos[((OP2UnitInfo*)unitInfoArray[mapRareOreMine])->techId]->techID;
						if (beacons.beacon[j].commonRareType == 1 && Player[playerId].HasTechnology(rareOreTech) == 0)
							continue;

						distance = GetDistanceToBeacon(curScout.Location(), j);
						if (distance == -1)
							continue;

						if (distance < (((OP2UnitInfo*)unitInfoArray[curScout.GetType()])->playerData[playerId].sightRange + 1))
							DiscoverBeacon(j, playerId);
					}
				}
			}
		}
	}
}


bool RandomRes::SetBeaconHook()
{
	if (beaconVtblHooked)
		return true;

	OP2PatchManager::PatchReferencedVirtualFunction(unitVtbl[mapMiningBeacon]->IsUnitVisible, RandomRes::Beacon_IsUnitVisible, false);
	OP2PatchManager::PatchReferencedVirtualFunction(unitVtbl[mapMiningBeacon]->Survey, RandomRes::Beacon_Survey, false);
	OP2PatchManager::PatchReferencedVirtualFunction(unitVtbl[mapMiningBeacon]->BuildMine, RandomRes::Beacon_BuildMine, false);
	OP2PatchManager::PatchFunction((void*)0x48D131, &RandomRes::Minimap_DrawUnitsOnMinimap_Trampoline, false);

	beaconVtblHooked = true;
	return true;
}


int __thiscall RandomRes::Beacon_IsUnitVisible(OP2Unit *classPtr, void *viewport)
{
	if (!beaconVtblHooked || !isInited)
		return 0;

	if (IsBeaconVisible(classPtr->unitIndex, TethysGame::LocalPlayer()) || ((OP2Game*)gameObj)->produceAll)
		return oldUnitVtbl[mapMiningBeacon].IsUnitVisible(classPtr, viewport);

	return 0;
}


void __thiscall RandomRes::Beacon_Survey(OP2Unit *classPtr, int playerNum)
{
	if (!beaconVtblHooked || !isInited)
		return;

	if (IsBeaconVisible(classPtr->unitIndex, playerNum))
	{
		if (!IsBeaconVisibleSelfOnly(classPtr->unitIndex, playerNum))
		{
			SetBeaconIsVisible(classPtr->unitIndex, playerNum, true);
			beacons.beaconsDiscovered[playerNum] ++;
		}
		return oldUnitVtbl[mapMiningBeacon].Survey(classPtr, playerNum);
	}

	return;
}


int __thiscall RandomRes::Beacon_BuildMine(OP2Unit *classPtr, OP2Unit *mineUnit)
{
	if (!beaconVtblHooked)
		return 0;

	if (!IsBeaconVisibleSelfOnly(classPtr->unitIndex, mineUnit->ownerNum))
	{
		SetBeaconIsVisible(classPtr->unitIndex, mineUnit->ownerNum, true);
		beacons.beaconsDiscovered[mineUnit->ownerNum] ++;
	}

	return oldUnitVtbl[mapMiningBeacon].BuildMine(classPtr, mineUnit);
}


bool RandomRes::IsBeaconVisible(int beaconIndex, int playerId)
{
	return (playerArray[playerId].playerBitmask & ~(*unitArray)[beaconIndex].rotation) != 0;
}

bool RandomRes::IsBeaconVisibleSelfOnly(int beaconIndex, int playerId)
{
	return ((1 << playerId) & ~(*unitArray)[beaconIndex].rotation) != 0;
}


void RandomRes::SetBeaconIsVisible(int beaconIndex, int playerId, bool isVisible)
{
	// Requires HFL in order to access the unit array
	if (!isInited)
		return;

	if (playerId >= 0 && playerId < 6)
	{
		if (isVisible)
			(*unitArray)[beaconIndex].rotation &= ~(1 << playerId);
		else
			(*unitArray)[beaconIndex].rotation |= (1 << playerId);
	}
	else
		(*unitArray)[beaconIndex].rotation = (isVisible) ? 0 : -1;
}


// Nastiness for hooking the minimap unit drawing function: BEGIN.


void __declspec(naked) RandomRes::Minimap_DrawUnitsOnMinimap_Trampoline()
{
	static OP2Unit *thisBeacon;
	__asm
	{
		mov thisBeacon, edi

		pushad
	}
	static DWORD jumpToAddr;

	if (IsBeaconVisible(thisBeacon->unitIndex, TethysGame::LocalPlayer()) || ((OP2Game*)gameObj)->produceAll)
		jumpToAddr = OP2Addr(0x48D137);
	else
		jumpToAddr = OP2Addr(0x48D1CD);

	__asm
	{
		popad

		// Reissue overwritten instructions
		call dword ptr [eax + 0x98]

		// Jump back to original function
		jmp jumpToAddr
	}
}