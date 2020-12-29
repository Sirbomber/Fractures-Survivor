#ifndef RESOURCESCLASS_H
#define RESOURCESCLASS_H

#include <limits.h>
#include <Outpost2DLL.h>
#include "OP2Types\OP2Units.h"


#pragma pack(push,1)
struct ResourceTileData
{
	char canPlace		:4; // bool
	char smelterOrMine	:4; // 0 = none, 1 = smelter or mine, -1 = temp. smelter data
};

struct BeaconList {
	BeaconList()
	{
		this->numBeacons = 0;
		memset(this->beaconsDiscovered, 0, sizeof(this->beaconsDiscovered));
		memset(this->beacon, 0, sizeof(this->beacon));
	}
	short numBeacons;
	short beaconsDiscovered[7];
	struct BeaconData {
		map_id beaconType;
		LOCATION location;
		char commonRareType;
		char barYield;
		char barVariant;
		bool isVisible;
	} beacon[2047];
};
#pragma pack(pop)

extern struct BeaconList beacons;

class RandomRes
{
public:
	RandomRes(struct MAP_RECT area, short numCommon, short numRare, short numFumaroles, short numMagma, char minDistance = 5);
	RandomRes();
	~RandomRes();

	bool IsInited();

	void SetToIncludedArea(MAP_RECT rect, char value, bool smelter = false);
	void SetToIncludedArea(LOCATION location, char value, bool smelter = false);

	void SetWeights(short common1 = 1, short common2 = 1, short common3 = 1, short rare1 = 1, short rare2 = 1, short rare3 = 1);
	void SetMin(short common1 = 0, short common2 = 0, short common3 = 0, short rare1 = 0, short rare2 = 0, short rare3 = 0, short fumaroles = 0, short magma = 0);
	void SetMax(short common1 = -1, short common2 = -1, short common3 = -1, short rare1 = -1, short rare2 = -1, short rare3 = -1, short fumaroles = -1, short magma = -1);
	void CapAt(short max);

	static void AddBeacon(map_id beaconType, LOCATION location, int commonRareType, int barYield, int barVariant, bool hidden);
	void Create();

	static double GetDistanceToBeacon(struct LOCATION location, short beaconId);
	static void DiscoverBeacon(short beaconId, int playerId, bool silent = false);
	static void DiscoverAllBeacons(int playerId);
	static void LocateBeaconProc(int playerId);

	static bool SetBeaconHook();
	static int __thiscall Beacon_IsUnitVisible(OP2Unit *classPtr, void *viewport);
	static void __thiscall Beacon_Survey(OP2Unit *classPtr, int playerNum);
	static int __thiscall Beacon_BuildMine(OP2Unit *classPtr, OP2Unit *mineUnit);
	static void Minimap_DrawUnitsOnMinimap_Trampoline();

private:
	int GetTileOffset(LOCATION location);

	bool IsInIncludedArea(LOCATION location);

	bool AreaBlocked(MAP_RECT area, bool allowUnit = true, bool allowWallOrBuilding = false, MAP_RECT excludedRect = MAP_RECT(-1, -1, -1, -1));
	bool CanBuildSmelterAt(LOCATION beaconLocation, bool allowOnMine);
	LOCATION DoSmelterTest(MAP_RECT testRect, MAP_RECT baseSmelterRect, MAP_RECT mineExcludeRect);

	bool TestDistance(struct LOCATION loc);

	char GetTerrainType(struct LOCATION loc); // returns: 1 = gray rock, 2 = black rock, 3 = lava flow, 0 = other (orange/etc.)
	char NextRes();
	char GetRandomRes();
	short GetTotalCreated();

	static bool IsBeaconVisible(int beaconIndex, int playerId);
	static bool IsBeaconVisibleSelfOnly(int beaconIndex, int playerId);
	static void SetBeaconIsVisible(int beaconIndex, int playerId, bool isVisible);

public:
	ResourceTileData *includedTiles;
	int numElementsIncludedTiles;
	int sizeIncludedTiles;
	int areaWidth;
	int areaHeight;

	short maxCommon;
	short maxRare;
	short maxFumaroles;
	short maxMagma;
	char distance; // distance between each resource

	int requireSmelterSpace; // this value is equal to the min. number of potential smelters that room must be made for near beacons; -1 causes beacons to ensure no smelters can be placed near them
	bool noSmelterOverlap; // if true and the above value > 0, the calculated potential smelter locations for one beacon can't be overlapped by the smelter locations for any other beacon after it
	int maxSmelterSpacing; // maximum allowed distance from potential smelter locations to their beacon

	bool fumarolesOnGrayRock;
	bool fumarolesOnLavaFlow;

	bool magmaOnLavaFlow;

	bool allowOrangeRock;
	bool allowGrayRock;
	bool allowBlackRock;
	bool allowLavaFlow;
	bool allowLavaPossible;

	bool scoutsFindBeacons; // beacons are invisible until a scout sees them within its sight range

private:
	bool inited;

	short maxResources;

	short commonRange[3][2]; // dimension 0 = yield (lowest to highest), dimension 1 = min/max
	short commonWeight[3];
	short rareRange[3][2];
	short rareWeight[3];
	short fumaroleRange[2];
	short magmaRange[2];

	short resourcesCreated[8]; // common 1/2/3, rare 1/2/3, fumarole, magma
	short typesCreated[4]; // common, rare, fumarole, magma

	const static DWORD maxIterations = 1000; // how many times the main Create() loop can iterate before quitting
	const static DWORD maxInnerIterations = 1000; // how many times the loops inside the main Create() loop can iterate before they quit
};

#endif