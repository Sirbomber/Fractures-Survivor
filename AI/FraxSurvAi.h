#pragma once

#include <math.h>
#include <Outpost2DLL.h>
#include <OP2Helper.h>
#include <hfl.h>
#include "OP2Types.h"
#include "FraxSurvWaypoints.h"

//#define NOAIFACTORIES
//#define NOAIMINING
//#define NOAICIVUNITS
//#define NOAIMILUNITS
#define NOAIREPAIRS
// Note: AI repairs are disabled due to an OP2 bug/crash that involves reinforcing from a rebuilt Arachnid Factory

// I thought about storing smelters/etc in lists, but that would cause problems with saved games, so hardcoded arrays it is.
// You're welcome, anyone looking to port this into a colony game.
const int AI_MAIN_COMMON_GROUPS = 6;
const int AI_MAIN_RARE_GROUPS = 3;
const int AI_MAIN_VF_GROUPS = 6;

const int AI_EDEN_COMMON_GROUPS = 1;
const int AI_EDEN_RARE_GROUPS = 0;
const int AI_EDEN_VF_GROUPS = 2;

const int AI_PLYM_COMMON_GROUPS = 3;
const int AI_PLYM_RARE_GROUPS = 0;
const int AI_PLYM_VF_GROUPS = 2;

// Attack wave delay constants
const int MAIN_DELAY_MIN = 7100,
		  MAIN_DELAY_MAX = 7600;
const int SPACEPORT_DELAY = 55000;
const int NUM_WAYPOINTS = 20;

class FraxSurvAI
{
public:
	FraxSurvAI() {};
	~FraxSurvAI() {};

	void SetupAI(int _aiNum);			// Initialization
	void UpdateCycle();					// Check AI status

	int PlayerNum() { return aiNum; }

private:
	// Base setup and management
	void SetupMainBase();				// Create units/etc for main AI base
	void SetupOutpost1();				// Create units/etc for Eden outpost
	void SetupOutpost2();				// Create units/etc for Plymouth outpost

	void PrepareUnitGroupsMain();
	void RecordStructuresMain();
	void PrepareUnitGroupsEden();
	void PrepareUnitGroupsPlymouth();
	void RecordStructuresPlymouth();

	void Eden_Repair();

	// Utility methods
	long GetDistance(LOCATION pt1, LOCATION pt2);
	void FindNearestCommandCenter();
	void LabStuff();
	void SafeDestroyGroup(ScGroup &group);

	// Spaceport management
	void CheckSpaceportStatus();
	void CheckRebuiltSpaceport1();
	void CheckRebuiltSpaceport2();
	void LaunchEmpMissile1();
	void LaunchEmpMissile2();

	// Combat management
	void CheckAttack();

	void Main_AttackTeaser();
	void Main_Attack1();
	void Main_Attack2();
	void Main_Attack3();
	void Main_Attack4();
	void Main_Attack5();
	void Main_Attack6();
	void Main_Attack7();
	void Main_Attack8();
	void Main_Attack9();
	void Main_Attack10();
	void Main_Attack11();
	void Main_Attack12();
	void Main_Attack13();
	void Main_Attack14();
	void Main_AttackLoop();

	void Eden_Attack1();
	void Eden_Attack2();
	void Eden_Attack3();
	void Eden_Attack4();
	void Eden_Attack5();
	void Eden_Attack6();
	void Eden_Attack7();
	void Eden_Attack8();
	void Eden_Attack9();
	void Eden_Attack10();
	void Eden_AttackFinal();
	void Eden_Shutdown();

	void Plymouth_Attack1();
	void Plymouth_Attack2();
	void Plymouth_Attack3();
	void Plymouth_Attack4();
	void Plymouth_Attack5();
	void Plymouth_Attack6();
	void Plymouth_Attack7();
	void Plymouth_Attack8();
	void Plymouth_Attack9();
	void Plymouth_AttackFinal();
	void Plymouth_Shutdown();

	void AttackersAttacked();
	void PG1_Attacked();
	void PG2_Attacked();

	void CheckRevenge();

	// Data
	int aiNum = -1;						// AI player's internal player number
	int numHumansEasy = 0;				// Used to adjust AI attack strength based on player difficulty settings
	int numHumansNormal = 0;			// Used to adjust AI attack strength based on player difficulty settings
	int numHumansHard = 0;				// Used to adjust AI attack strength based on player difficulty settings
	Trigger trigUpdateCycle;

	// AI Unit Groups
	MiningGroup MainCommon[AI_MAIN_COMMON_GROUPS],
		MainRare[AI_MAIN_RARE_GROUPS],
		EdenCommon,
		PlymouthCommon[AI_PLYM_COMMON_GROUPS];

	BuildingGroup MainSF,
		MainEtc,
		MainVF[AI_MAIN_VF_GROUPS],
		MainRepairs,
		EdenVF[AI_EDEN_VF_GROUPS],
		EdenRepairs,
		PlymouthSF,
		PlymouthEtc,
		PlymouthVF[AI_PLYM_VF_GROUPS];

	FightGroup MainDefense[3],
		EdenDefense,
		PlymouthDefense,
		PatrolGroups[4],
		ChokeDefense,
		MainOffense[3],
		EdenOffense,
		PlymouthOffense,
		Attacking[2];

	// Track these so we can make EMP Missiles.
	UnitEx Spaceport[2];
	int LastMissileTime[2];

	// Research, because why not?
	UnitEx StdLab,
		AdvLab;
	int LastResearchTime = 0;

	int curStdTech = 0,
		curAdvTech = 0;

	// Track these just to clean things up.
	Unit MainCC,
		EdenCC,
		PlymOutpostCC;

	// Attack waves used to be handled by triggers.  This is a sloppy replacement that allows everything to be handled by the primary UpdateCycle method.
	int curAttackWaveMain = 0,
		curAttackWaveEden = 0,
		curAttackWavePlym = 0,
		nextAttackTimeMain = 0,
		nextAttackTimeEden = 0,
		nextAttackTimePlym = 0;

	// "Has been attacked" detectors for patrol groups
	bool Patrol1Attacked = false,
		Patrol2Attacked = false,
		AttackGroupAttacked = false;	// instead of blind attack orders, the main Plymouth force should follow waypoints to the enemy base.

	LOCATION *attackRoute = NULL;

	wayPnt aiWaypoints[20] =
	{ {LOCATION(66 + 31,  54 - 1), 0},					// Point 0: Start point
	{LOCATION(35 + 31,  36 - 1), &aiWaypoints[0]},		// Point 1: Northern chokepoint to northwest corner
	{LOCATION(37 + 31,  60 - 1), &aiWaypoints[0]},		// Point 2: Midpoint between start and north of lava passage checkpoint (unused)
	{LOCATION(4 + 31,  63 - 1), &aiWaypoints[0]},		// Point 3: North of lava checkpoint (used to point to 2)
	{LOCATION(7 + 31,  87 - 1), &aiWaypoints[3]},		// Point 4: South of lava checkpoint (unused)
	{LOCATION(16 + 31,  90 - 1), &aiWaypoints[3]},		// Point 5: Pathing helper (used to point to 4)
	{LOCATION(7 + 31, 111 - 1), &aiWaypoints[5]},		// Point 6: Black rock plataeu north of Eden
	{LOCATION(21 + 31, 136 - 1), &aiWaypoints[6]},		// Point 7: At Eden
	{LOCATION(42 + 31, 140 - 1), &aiWaypoints[7]},		// Point 8: Just east of Eden
	{LOCATION(44 + 31, 166 - 1), &aiWaypoints[8]},		// Point 9: South of Eden (unused)
	{LOCATION(13 + 31, 195 - 1), &aiWaypoints[7]},		// Point 10: Southwest of Eden (used to point to 9)
	{LOCATION(48 + 31, 195 - 1), &aiWaypoints[7]},		// Point 11: Southeast of Eden (used to point to 9)
	{LOCATION(74 + 31, 143 - 1), &aiWaypoints[8]},		// Point 12: Middle south
	{LOCATION(96 + 31, 140 - 1), &aiWaypoints[12]},		// Point 13: West of Plymouth (unused)
	{LOCATION(107 + 31, 153 - 1), &aiWaypoints[12]},		// Point 14: Plymouth (used to point to 13)
	{LOCATION(118 + 31, 180 - 1), &aiWaypoints[14]},		// Point 15: South of Plymouth (unused)
	{LOCATION(118 + 31, 216 - 1), &aiWaypoints[14]},		// Point 16: Southeast corner (used to point to 15)
	{LOCATION(102 + 31, 121 - 1), &aiWaypoints[5]},		// Point 17: Middle south, just below lava
	{LOCATION(124 + 31, 127 - 1), &aiWaypoints[17]},		// Point 18: North of Plymouth
	{LOCATION(94 + 31,  77 - 1), &aiWaypoints[0]},		// Point 19: In front of eastern mountain chokepoint
	};
};

void __cdecl odprintf(const char *format, ...);         // Debug print
