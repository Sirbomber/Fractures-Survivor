#include <Tethys/API/API.h>
#include <DisasterCreator/DisasterCreator.h>
#include <RequiredExports.h>

#include "..\functions.h"	// temp

using namespace Tethys;
using namespace Tethys::TethysAPI;

// DisasterCreator and associated triggers
DisasterCreator DC;
Trigger DC_Callback,
		DC_Timer1,
		DC_Timer2;

int disasterLevel = 0;				// Tracks the progression of disaster strength

const int MAX_DIS_LEVEL =      7,	// Highest value for disaster strength level
          LEVEL_1_TIME  =   7300,	// Tick to start having disasters
		  LEVEL_2_TIME  =  24100,	// Tick to start meteors and medium-strength disasters
		  LEVEL_3_TIME  =  42800,	// Tick to start storms
		  LEVEL_4_TIME  =  56000,	// Tick to enable high power disasters
		  LEVEL_5_TIME  =  83000,	// Tick to enable vortexes
		  LEVEL_6_TIME  = 129000,	// Tick to enable apocalyptic disasters
		  LEVEL_7_TIME  = 162000;	// You're screwed!

// Original timings: Quakes start around mark 80; upgrade around mark 400, and upgrade again around mark 820
//                   Meteors start around mark 260 and upgrade around mark 560
//                   Storms start around mark 462 and upgrade around mark 814

void ConfigureDisasterCreator()
{
	DC.SetMinimumWait(4300);
	DC.SetIgnoreChance(0);
	DC.SetDisasterTypeWeight(disNone, 100);
	DC.SetDisasterPowerWeight(pwrLow, 100);
	DC.SetDisasterTargetWeight(trgRandom, 100);
	DC.SetCallbackTrigger("DisasterCreator_Callback", 200);
}

// Level 1
void EnableInitialDisasters()
{
	// Start off slow.  Limit initial disasters to only light quakes mostly within the central valley (rarely they can target anywhere on the map)
	DC.DoDisaster(disNone, pwrLow, trgRandom, true, true);		// This resets the time until next disaster so we won't instantly get a quake
	DC.EnableDisaster(disQuake);
	DC.SetDisasterTypeWeight(disQuake, 95);
	DC.SetDisasterTypeWeight(disNone, 5);
	DC.SetDisasterPowerWeight(pwrLow, 100);
	DC.SetDisasterTargetWeight(trgZone, 98);
	DC.SetDisasterTargetWeight(trgRandom, 2);
	DC.AddDisasterZone(disQuake, MapRect(GameMap::At(42, 49), GameMap::At(86, 161)));
}

// Level 2
void EnableMeteorsAndMediumPower()
{
	DC.SetMinimumWait(4100);
	DC.SetIgnoreChance(2);
	DC.EnableDisaster(disMeteor);
	DC.SetDisasterTypeWeight(disQuake, 65);
	DC.SetDisasterTypeWeight(disMeteor, 30);
	DC.SetDisasterTypeWeight(disNone, 5);
	DC.SetDisasterPowerWeight(pwrLow, 85);
	DC.SetDisasterPowerWeight(pwrMedium, 15);
}

// Level 3
void EnableStormsAndMakeMediumDisastersMoreLikely()
{
	DC.SetMinimumWait(3900);
	DC.SetIgnoreChance(4);
	DC.EnableDisaster(disStorm);
	DC.SetDisasterTypeWeight(disQuake, 42);
	DC.SetDisasterTypeWeight(disStorm, 30);
	DC.SetDisasterTypeWeight(disMeteor, 25);
	DC.SetDisasterTypeWeight(disNone, 3);
	DC.SetDisasterPowerWeight(pwrLow, 70);
	DC.SetDisasterPowerWeight(pwrMedium, 30);
	DC.AddDisasterZone(disStorm, MapRect(GameMap::At(1, 40), GameMap::At(32, 221)));
	DC.AddDisasterZone(disStorm, MapRect(GameMap::At(100, 31), GameMap::At(128, 221)));
}

// Level 4
void EnableHighPowerDisasters()
{
	DC.SetDisasterPowerWeight(pwrLow, 45);
	DC.SetDisasterPowerWeight(pwrMedium, 40);
	DC.SetDisasterPowerWeight(pwrHigh, 15);
	DC.SetDisasterTargetWeight(trgZone, 78);
	DC.SetDisasterTargetWeight(trgPlayer, 20);
	DC.SetDisasterTargetWeight(trgRandom, 2);
}

// Level 5
void EnableVortexes()
{
	DC.SetMinimumWait(3400);
	DC.SetIgnoreChance(7);
	DC.EnableDisaster(disVortex);
	DC.SetDisasterTypeWeight(disQuake, 40);
	DC.SetDisasterTypeWeight(disStorm, 30);
	DC.SetDisasterTypeWeight(disMeteor, 20);
	DC.SetDisasterTypeWeight(disVortex, 9);
	DC.SetDisasterTypeWeight(disNone, 1);
	DC.SetDisasterPowerWeight(pwrLow, 15);
	DC.SetDisasterPowerWeight(pwrMedium, 65);
	DC.SetDisasterPowerWeight(pwrHigh, 20);
	DC.AddDisasterZone(disVortex, MapRect(GameMap::At(7, 19), GameMap::At(31, 54)));
	DC.AddDisasterZone(disVortex, MapRect(GameMap::At(101, 31), GameMap::At(124, 100)));
	DC.AddDisasterZone(disVortex, MapRect(GameMap::At(21, 35), GameMap::At(103, 101)));
}

// Level 6
void EnableApocalypticDisasters()
{
	DC.SetDisasterPowerWeight(pwrLow, 10);
	DC.SetDisasterPowerWeight(pwrMedium, 55);
	DC.SetDisasterPowerWeight(pwrHigh, 30);
	DC.SetDisasterPowerWeight(pwrApocalyptic, 5);
}

// Level 7
void EnableEndgameDisasters()
{
	DC.SetMinimumWait(2900);
	DC.SetIgnoreChance(11);
	DC.SetDisasterPowerWeight(pwrLow, 0);
	DC.SetDisasterPowerWeight(pwrMedium, 10);
	DC.SetDisasterPowerWeight(pwrHigh, 60);
	DC.SetDisasterPowerWeight(pwrApocalyptic, 30);
}

Export void DisasterCreator_Callback()
{
	DC.RollRandom();

	switch (disasterLevel)
	{
	case 7:
		break;
	case 6:
		if (TethysGame::Tick() >= LEVEL_7_TIME)
		{
			EnableEndgameDisasters();
			disasterLevel++;
		}
		break;
	case 5:
		if (TethysGame::Tick() >= LEVEL_6_TIME)
		{
			EnableApocalypticDisasters();
			disasterLevel++;
		}
		break;
	case 4:
		if (TethysGame::Tick() >= LEVEL_5_TIME)
		{
			EnableVortexes();
			disasterLevel++;
		}
		break;
	case 3:
		if (TethysGame::Tick() >= LEVEL_4_TIME)
		{
			EnableHighPowerDisasters();
			disasterLevel++;
		}
		break;
	case 2:
		if (TethysGame::Tick() >= LEVEL_3_TIME)
		{
			EnableStormsAndMakeMediumDisastersMoreLikely();
			disasterLevel++;
		}
		break;
	case 1:
		if (TethysGame::Tick() >= LEVEL_2_TIME)
		{
			EnableMeteorsAndMediumPower();
			disasterLevel++;
		}
		break;
	case 0:
		if (TethysGame::Tick() >= LEVEL_1_TIME)
		{
			EnableInitialDisasters();
			disasterLevel++;
		}
		break;
	}
}
