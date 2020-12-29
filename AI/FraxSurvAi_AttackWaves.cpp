// This file contains all AI attack wave setup/management logic.

#pragma once

#include "FraxSurvAi.h"

void FraxSurvAI::CheckAttack()
{
	// Check if it's time to attack
	if (TethysGame::Tick() > nextAttackTimeMain)
	{
		switch (curAttackWaveMain)
		{
		case 0:
			Main_AttackTeaser();
			break;
		case 1:
			Main_Attack1();
			break;
		case 2:
			Main_Attack2();
			break;
		case 3:
			Main_Attack3();
			break;
		case 4:
			Main_Attack4();
			break;
		case 5:
			Main_Attack5();
			break;
		case 6:
			Main_Attack6();
			break;
		case 7:
			Main_Attack7();
			break;
		case 8:
			Main_Attack8();
			break;
		case 9:
			Main_Attack9();
			break;
		case 10:
			Main_Attack10();
			break;
		case 11:
			Main_Attack11();
			break;
		case 12:
			Main_Attack12();
			break;
		case 13:
			Main_Attack13();
			break;
		case 14:
			Main_Attack14();
			break;
		default:
			Main_AttackLoop();
			break;
		}
	}

	// Eden outpost attack check
	if (TethysGame::CanHaveDisasters() && TethysGame::Tick() > nextAttackTimeEden)
	{
		switch (curAttackWaveEden)
		{
		case 0:
			Eden_Attack1();
			break;
		case 1:
			Eden_Attack2();
			break;
		case 2:
			Eden_Attack3();
			break;
		case 3:
			Eden_Attack4();
			break;
		case 4:
			Eden_Attack5();
			break;
		case 5:
			Eden_Attack6();
			break;
		case 6:
			Eden_Attack7();
			break;
		case 7:
			Eden_Attack8();
			break;
		case 8:
			Eden_Attack9();
			break;
		case 9:
			Eden_Attack10();
			break;
		case 10:
			Eden_AttackFinal();
			break;
		default:
			Eden_Shutdown();
			break;
		}
	}

	// Plymouth outpost attack check
	if (TethysGame::UsesDayNight() && TethysGame::Tick() > nextAttackTimePlym)
	{
		switch (curAttackWavePlym)
		{
		case 0:
			Plymouth_Attack1();
			break;
		case 1:
			Plymouth_Attack2();
			break;
		case 2:
			Plymouth_Attack3();
			break;
		case 3:
			Plymouth_Attack4();
			break;
		case 4:
			Plymouth_Attack5();
			break;
		case 5:
			Plymouth_Attack6();
			break;
		case 6:
			Plymouth_Attack7();
			break;
		case 7:
			Plymouth_Attack8();
			break;
		case 8:
			Plymouth_Attack9();
			break;
		case 10:
			Plymouth_AttackFinal();
			break;
		default:
			Plymouth_Shutdown();
			break;
		}
	}

	AttackersAttacked();
	PG1_Attacked();
	PG2_Attacked();
}

// ----------------------------------------------------------------------------
// Main base attack waves
// ----------------------------------------------------------------------------

void FraxSurvAI::Main_AttackTeaser()
{
	odprintf("Main_AttackTeaser: Begin.\n");
	// Issue the attack order and then clean up this group.
	FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
	odprintf(" >Main_AttackTeaser: Added units to Attack group.\n");
	if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_AttackTeaser: Unreinforce group 0.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapLynx, mapMicrowave, ((3 * numHumansEasy) + (4 * numHumansNormal) + (4 * numHumansHard)));
		MainOffense[0].SetTargCount(mapLynx, mapStickyfoam,((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard)));
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0],4000);
		odprintf(" >Main_AttackTeaser: Updated group 0.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));
	odprintf("Main_AttackTeaser: End.\n");
}

void FraxSurvAI::Main_Attack1()
{
	odprintf("Main_Attack1: Begin.\n");
	// Issue the attack order and then clean up this group.
	FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
	odprintf(" >Main_Attack1: Added units to Attack group.\n");
	if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_Attack1: Unreinforce group 0.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapLynx, mapMicrowave, ((3 * numHumansEasy) + (4 * numHumansNormal) + (4 * numHumansHard) + 3));
		MainOffense[0].SetTargCount(mapLynx, mapStickyfoam,((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard) + 2));
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0],4000);
		odprintf(" >Main_Attack1: Updated group 0.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));
	odprintf("Main_Attack1: End.\n");
}

void FraxSurvAI::Main_Attack2()
{
	odprintf("Main_Attack2: Begin.\n");
	// Issue the attack order and then clean up this group.
	FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
	odprintf(" >Main_Attack2: Added units to Attack group.\n");
	if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_Attack2: Unreinforce group 0.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapLynx, mapMicrowave, ((3 * numHumansEasy) + (4 * numHumansNormal) + (4 * numHumansHard)) + 3);
		MainOffense[0].SetTargCount(mapLynx, mapStickyfoam,((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard)) + 2);
		MainOffense[0].SetTargCount(mapLynx, mapRPG, (TethysGame::NoPlayers() + 2));
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0],4000);
		odprintf(" >Main_Attack2: Updated group 0.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));
	odprintf("Main_Attack2: End.\n");
}

void FraxSurvAI::Main_Attack3()
{
	odprintf("Main_Attack3: Begin.\n");
	// Issue the attack order and then clean up this group.
	FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
	odprintf(" >Main_Attack3: Added units to Attack group.\n");
	if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_Attack3: Unreinforce group 0.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapLynx, mapMicrowave, ((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard) + 2));
		MainOffense[0].SetTargCount(mapLynx, mapStickyfoam, (TethysGame::NoPlayers()+3));
		MainOffense[0].SetTargCount(mapLynx, mapRPG, ((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard)));
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0],4000);
		odprintf(" >Main_Attack3: Updated group 0.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));
	odprintf("Main_Attack3: End.\n");
}

void FraxSurvAI::Main_Attack4()
{
	odprintf("Main_Attack4: Begin.\n");
	// Issue the attack order and then clean up this group.
	FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
	odprintf(" >Main_Attack4: Added units to Attack group.\n");
	if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_Attack4: Unreinforce group 0.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapLynx, mapMicrowave, (2 * (TethysGame::NoPlayers()+1)));
		MainOffense[0].SetTargCount(mapLynx, mapStickyfoam, 0);
		MainOffense[0].SetTargCount(mapLynx, mapRPG, ((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard)) + 2);
		MainOffense[0].SetTargCount(mapLynx, mapEMP, ((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard)));
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0],4000);
		odprintf(" >Main_Attack4: Updated group 0.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));
	odprintf("Main_Attack4: End.\n");
}

void FraxSurvAI::Main_Attack5()
{
	odprintf("Main_Attack5: Begin.\n");
	// Issue the attack order and then clean up this group.
	FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
	odprintf(" >Main_Attack5: Added units to Attack group.\n");
	if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_Attack5: Unreinforce group 0.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapLynx, mapMicrowave, 0);
		MainOffense[0].SetTargCount(mapLynx, mapRPG, 0);
		MainOffense[0].SetTargCount(mapLynx, mapEMP, 0);

		MainOffense[0].SetTargCount(mapPanther, mapMicrowave, (3 * (TethysGame::NoPlayers()+1)));
		MainOffense[0].SetTargCount(mapPanther, mapRPG, ((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard)) + 2);
		MainOffense[0].SetTargCount(mapPanther, mapEMP, ((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard)));
		MainOffense[0].SetTargCount(mapPanther, mapESG, 2);
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0],4000);
		odprintf(" >Main_Attack5: Updated group 0.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));
	odprintf("Main_Attack5: End.\n");
}

void FraxSurvAI::Main_Attack6()
{
	odprintf("Main_Attack6: Begin.\n");
	// Issue the attack order and then clean up this group.
	FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
	odprintf(" >Main_Attack6: Added units to Attack group.\n");
	if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_Attack6: Unreinforce group 0.\n");
	}

	// By now, the players have probably relocated up north.  Have our attack groups rally
	// inside the base, so units can't get picked off one at a time.  Also move one of the
	// defender groups to avoid overcrowding.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetRect(MAP_RECT(84+31, 15-1, 90+31, 28-1)); 
		
		// Start building the next attack group.
		MainOffense[0].SetTargCount(mapPanther, mapMicrowave, (TethysGame::NoPlayers()+2));
		MainOffense[0].SetTargCount(mapPanther, mapRPG, ((3 * numHumansEasy) + (3 * numHumansNormal) + (4 * numHumansHard)) - 3);
		MainOffense[0].SetTargCount(mapPanther, mapEMP, ((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard)));
		MainOffense[0].SetTargCount(mapPanther, mapESG, (TethysGame::NoPlayers()+2));
	}

	if (MainOffense[1].IsInitialized())
	{
		MainOffense[1].SetRect(MAP_RECT(53+31,  1-1, 61+31, 10-1));
	}

	if (MainDefense[2].IsInitialized())
	{
		MainDefense[2].SetRect(MAP_RECT(69+31,  6-1, 82+31, 11-1));
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0],4000);
		odprintf(" >Main_Attack6: Updated group 0.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Main_Attack6: End.\n");
}

void FraxSurvAI::Main_Attack7()
{
	odprintf("Main_Attack7: Begin.\n");
    // Issue the attack order and then clean up this group.
    FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
    Attacking[0].TakeAllUnits(MainOffense[1]);
    odprintf(" >Main_Attack7: Added groups 0/1 to attack group.\n");
	if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_Attack7: Unreinforce group 0.\n");
	}
	if (MainVF[4].IsInitialized())
	{
		MainVF[4].UnRecordVehGroup(MainOffense[1]);
		odprintf(" >Main_Attack7: Unreinforce group 1.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapLynx, mapMicrowave, TethysGame::NoPlayers()-1);
		MainOffense[0].SetTargCount(mapLynx, mapRPG, ((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard)));
		MainOffense[0].SetTargCount(mapLynx, mapEMP, ((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard)));
		MainOffense[0].SetTargCount(mapLynx, mapESG, (numHumansEasy + numHumansNormal + (2 * numHumansHard)));
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0], 6000);
		odprintf(" >Main_Attack7: Updated group 0.\n");
	}

	if (MainOffense[1].IsInitialized())
	{
	    MainOffense[1].SetTargCount(mapPanther, mapMicrowave, TethysGame::NoPlayers()-1);
		MainOffense[1].SetTargCount(mapPanther, mapRPG, ((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard) + 2));
		MainOffense[1].SetTargCount(mapPanther, mapEMP, ((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard) + 1));
		MainOffense[1].SetTargCount(mapPanther, mapESG, (numHumansEasy + numHumansNormal + (2 * numHumansHard)));
	}

    if (MainVF[4].IsInitialized())
	{
		MainVF[4].RecordVehReinforceGroup(MainOffense[1], 6000);
		odprintf(" >Main_Attack7: Updated group 1.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));
	odprintf("Main_Attack7: End.\n");
}

void FraxSurvAI::Main_Attack8()
{
	odprintf("Main_Attack8: Begin.\n");
    // Issue the attack order and then clean up this group.
    FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
    Attacking[0].TakeAllUnits(MainOffense[1]);
    if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_Attack8: Unreinforce group 0.\n");
	}
	if (MainVF[4].IsInitialized())
	{
		MainVF[4].UnRecordVehGroup(MainOffense[1]);
		odprintf(" >Main_Attack8: Unreinforce group 1.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapLynx, mapSupernova, ((2 * numHumansEasy) + (3 * numHumansNormal) + (4 * numHumansHard)));
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0], 6000);
		odprintf(" >Main_Attack8: Updated group 0.\n");
	}

	if (MainOffense[1].IsInitialized())
	{
	    MainOffense[1].SetTargCount(mapPanther, mapMicrowave, TethysGame::NoPlayers()-1);
		MainOffense[1].SetTargCount(mapPanther, mapRPG, ((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard)));
		MainOffense[1].SetTargCount(mapPanther, mapEMP, ((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard)));
		MainOffense[1].SetTargCount(mapPanther, mapESG, ((2 * numHumansEasy) + (2 * numHumansNormal) + (3 * numHumansHard) - 2));
	}

    if (MainVF[4].IsInitialized())
	{
		MainVF[4].RecordVehReinforceGroup(MainOffense[1], 6000);
		odprintf(" >Main_Attack8: Updated group 1.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Main_Attack8: End.\n");
}

void FraxSurvAI::Main_Attack9()
{
	odprintf("Main_Attack9: Begin.\n");
    // Issue the attack order and then clean up this group.
    FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
    Attacking[0].TakeAllUnits(MainOffense[1]);
    if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_Attack9: Unreinforce group 0.\n");
	}
	if (MainVF[4].IsInitialized())
	{
		MainVF[4].UnRecordVehGroup(MainOffense[1]);
		odprintf(" >Main_Attack9: Unreinforce group 1.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapLynx, mapSupernova, ((2 * numHumansEasy) + (3 * numHumansNormal) + (4 * numHumansHard) + 6));
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0], 6000);
		odprintf(" >Main_Attack9: Updated group 0.\n");
	}

	if (MainOffense[1].IsInitialized())
	{
		MainOffense[1].SetTargCount(mapPanther, mapMicrowave, TethysGame::NoPlayers() - 1);
		MainOffense[1].SetTargCount(mapPanther, mapRPG, (2 * (TethysGame::NoPlayers() + 1)) - 1);
		MainOffense[1].SetTargCount(mapPanther, mapEMP, (2 * TethysGame::NoPlayers()) - 1);
	    MainOffense[1].SetTargCount(mapPanther, mapESG, (2 * (TethysGame::NoPlayers())) - 1);
	}

    if (MainVF[4].IsInitialized())
	{
		MainVF[4].RecordVehReinforceGroup(MainOffense[1], 6000);
		odprintf(" >Main_Attack9: Updated group 1.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));
	odprintf("Main_Attack9: End.\n");
}

void FraxSurvAI::Main_Attack10()
{
	odprintf("Main_Attack10: Begin.\n");
    // Issue the attack order and then clean up this group.
    FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
    Attacking[0].TakeAllUnits(MainOffense[1]);
    if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_Attack10: Unreinforce group 0.\n");
	}
	if (MainVF[4].IsInitialized())
	{
		MainVF[4].UnRecordVehGroup(MainOffense[1]);
		odprintf(" >Main_Attack10: Unreinforce group 1.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapLynx, mapSupernova, 0);
		MainOffense[0].SetTargCount(mapPanther, mapMicrowave, (TethysGame::NoPlayers() / 2 + 1) - 1);
		MainOffense[0].SetTargCount(mapPanther, mapRPG, (numHumansEasy + numHumansNormal + (2 * numHumansHard) + 1));
		MainOffense[0].SetTargCount(mapPanther, mapEMP, (numHumansEasy + numHumansNormal + (2 * numHumansHard) + 1));
		MainOffense[0].SetTargCount(mapPanther, mapESG, (numHumansEasy + numHumansNormal + (2 * numHumansHard) + 1));
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0], 6000);
		odprintf(" >Main_Attack10: Updated group 0.\n");
	}

	if (MainOffense[1].IsInitialized())
	{
		MainOffense[1].SetTargCount(mapPanther, mapMicrowave, 0);
		MainOffense[1].SetTargCount(mapPanther, mapRPG, 0);
	    MainOffense[1].SetTargCount(mapPanther, mapEMP, 0);
		MainOffense[1].SetTargCount(mapPanther, mapESG, 0);
		MainOffense[1].SetTargCount(mapTiger, mapMicrowave, TethysGame::NoPlayers() + 1);
		MainOffense[1].SetTargCount(mapTiger, mapRPG, TethysGame::NoPlayers() + 3);
	    MainOffense[1].SetTargCount(mapTiger, mapEMP, (numHumansEasy + numHumansNormal + (2 * numHumansHard) + 3));
		MainOffense[1].SetTargCount(mapTiger, mapESG, TethysGame::NoPlayers() + 3);
	}

    if (MainVF[4].IsInitialized())
	{
		MainVF[4].RecordVehReinforceGroup(MainOffense[1], 6000);
		odprintf(" >Main_Attack10: Updated group 1.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));
	odprintf("Main_Attack10: End.\n");
}

void FraxSurvAI::Main_Attack11()
{
	odprintf("Main_Attack11: Begin.\n");
    // Issue the attack order and then clean up this group.
    FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
    Attacking[0].TakeAllUnits(MainOffense[1]);
    if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_Attack11: Unreinforce group 0.\n");
	}
	if (MainVF[4].IsInitialized())
	{
		MainVF[4].UnRecordVehGroup(MainOffense[1]);
		odprintf(" >Main_Attack11: Unreinforce group 1.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapPanther, mapMicrowave, TethysGame::NoPlayers() + 3);
		MainOffense[0].SetTargCount(mapPanther, mapRPG, 2 * TethysGame::NoPlayers() );
		MainOffense[0].SetTargCount(mapPanther, mapEMP, 2 * TethysGame::NoPlayers() );
		MainOffense[0].SetTargCount(mapPanther, mapESG, 2 * (TethysGame::NoPlayers()-1) );
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0], 6000);
		odprintf(" >Main_Attack11: Updated group 0.\n");
	}

	if (MainOffense[1].IsInitialized())
	{
		MainOffense[1].SetTargCount(mapTiger, mapMicrowave, TethysGame::NoPlayers() + 1);
		MainOffense[1].SetTargCount(mapTiger, mapRPG, 2 * (TethysGame::NoPlayers() + 1));
		MainOffense[1].SetTargCount(mapTiger, mapEMP, 2 * (TethysGame::NoPlayers() + 1));
		MainOffense[1].SetTargCount(mapTiger, mapESG, 2 * TethysGame::NoPlayers());
	}

    if (MainVF[4].IsInitialized())
	{
		MainVF[4].RecordVehReinforceGroup(MainOffense[1], 6000);
		odprintf(" >Main_Attack11: Updated group 1.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));
	odprintf("Main_Attack11: End.\n");
}

void FraxSurvAI::Main_Attack12()
{
	odprintf("Main_Attack12: Begin.\n");
    // Issue the attack order and then clean up this group.
    FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
    Attacking[0].TakeAllUnits(MainOffense[1]);
    if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_Attack12: Unreinforce group 0.\n");
	}
	if (MainVF[4].IsInitialized())
	{
		MainVF[4].UnRecordVehGroup(MainOffense[1]);
		odprintf(" >Main_Attack12: Unreinforce group 1.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapPanther, mapMicrowave, TethysGame::NoPlayers() + 4);
		MainOffense[0].SetTargCount(mapPanther, mapRPG, 3 * TethysGame::NoPlayers() );
		MainOffense[0].SetTargCount(mapPanther, mapEMP, 3 * TethysGame::NoPlayers() );
		MainOffense[0].SetTargCount(mapPanther, mapESG, 2 * (TethysGame::NoPlayers()) );
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0], 6000);
		odprintf(" >Main_Attack12: Updated group 0.\n");
	}

	if (MainOffense[1].IsInitialized())
	{
		MainOffense[1].SetTargCount(mapTiger, mapMicrowave, TethysGame::NoPlayers() + 2);
		MainOffense[1].SetTargCount(mapTiger, mapRPG, 2 * (TethysGame::NoPlayers() + 2));
		MainOffense[1].SetTargCount(mapTiger, mapEMP, 2 * (TethysGame::NoPlayers() + 2));
		MainOffense[1].SetTargCount(mapTiger, mapESG, 3 * TethysGame::NoPlayers());
	}

    if (MainVF[4].IsInitialized())
	{
		MainVF[4].RecordVehReinforceGroup(MainOffense[1], 6000);
		odprintf(" >Main_Attack12: Updated group 1.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Main_Attack12: End.\n");
}

void FraxSurvAI::Main_Attack13()
{
	odprintf("Main_Attack13: Begin.\n");
    // Issue the attack order and then clean up this group.
    FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
    Attacking[0].TakeAllUnits(MainOffense[1]);
    if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_Attack13: Unreinforce group 0.\n");
	}
	if (MainVF[4].IsInitialized())
	{
		MainVF[4].UnRecordVehGroup(MainOffense[1]);
		odprintf(" >Main_Attack13: Unreinforce group 1.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapPanther, mapMicrowave, 2 * (TethysGame::NoPlayers()+2) );
		MainOffense[0].SetTargCount(mapPanther, mapRPG, 3 * (TethysGame::NoPlayers()+1) );
		MainOffense[0].SetTargCount(mapPanther, mapEMP, 3 * (TethysGame::NoPlayers()+1) );
		MainOffense[0].SetTargCount(mapPanther, mapESG, 2 * (TethysGame::NoPlayers()+1) );
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0], 6000);
		odprintf(" >Main_Attack12: Updated group 0.\n");
	}

	if (MainOffense[1].IsInitialized())
	{
		MainOffense[1].SetTargCount(mapTiger, mapMicrowave, TethysGame::NoPlayers() + 2);
		MainOffense[1].SetTargCount(mapTiger, mapRPG, 3 * (TethysGame::NoPlayers() + 2));
		MainOffense[1].SetTargCount(mapTiger, mapEMP, 3 * (TethysGame::NoPlayers() + 2));
		MainOffense[1].SetTargCount(mapTiger, mapESG, 3 * TethysGame::NoPlayers());
	}

    if (MainVF[4].IsInitialized())
	{
		MainVF[4].RecordVehReinforceGroup(MainOffense[1], 6000);
		odprintf(" >Main_Attack13: Updated group 1.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Main_Attack13: End.\n");
}

void FraxSurvAI::Main_Attack14()
{
	odprintf("Main_Attack14: Begin.\n");
    // Issue the attack order and then clean up this group.
    FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
    Attacking[0].TakeAllUnits(MainOffense[1]);
    if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_Attack14: Unreinforce group 0.\n");
	}
	if (MainVF[4].IsInitialized())
	{
		MainVF[4].UnRecordVehGroup(MainOffense[1]);
		odprintf(" >Main_Attack14: Unreinforce group 1.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapLynx, mapSupernova, 16);
		MainOffense[0].SetTargCount(mapPanther, mapMicrowave, 0);
		MainOffense[0].SetTargCount(mapPanther, mapRPG, 5 * TethysGame::NoPlayers() );
		MainOffense[0].SetTargCount(mapPanther, mapEMP, 5 * TethysGame::NoPlayers() );
	    MainOffense[0].SetTargCount(mapPanther, mapESG, 3 * TethysGame::NoPlayers() );
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0], 6000);
		odprintf(" >Main_Attack14: Updated group 0.\n");
	}

	if (MainOffense[1].IsInitialized())
	{
		MainOffense[1].SetTargCount(mapPanther, mapMicrowave, 0);
		MainOffense[1].SetTargCount(mapPanther, mapRPG, 0);
	    MainOffense[1].SetTargCount(mapPanther, mapEMP, 0);
		MainOffense[1].SetTargCount(mapPanther, mapESG, 0);
		MainOffense[1].SetTargCount(mapTiger, mapMicrowave, 3 * TethysGame::NoPlayers() );
		MainOffense[1].SetTargCount(mapTiger, mapRPG, 4 * TethysGame::NoPlayers() - 1);
	    MainOffense[1].SetTargCount(mapTiger, mapEMP, 4 * TethysGame::NoPlayers() - 1);
		MainOffense[1].SetTargCount(mapTiger, mapESG, 4 * TethysGame::NoPlayers() - 1);
	}

    if (MainVF[4].IsInitialized())
	{
		MainVF[4].RecordVehReinforceGroup(MainOffense[1], 6000);
		odprintf(" >Main_Attack14: Updated group 1.\n");
	}

	curAttackWaveMain++;
	nextAttackTimeMain += (MAIN_DELAY_MIN - 600 + TethysGame::GetRand(MAIN_DELAY_MAX - 200 - MAIN_DELAY_MIN + 1));

	odprintf("Main_Attack14: End.\n");
}

void FraxSurvAI::Main_AttackLoop()
{
	odprintf("Main_AttackLoop: Begin.\n");
    // Issue the attack order and then clean up this group.
    FindNearestCommandCenter();
	Attacking[0].TakeAllUnits(MainOffense[0]);
    Attacking[0].TakeAllUnits(MainOffense[1]);

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].UnRecordVehGroup(MainOffense[0]);
		odprintf(" >Main_AttackLoop: Unreinforce group 0.\n");
	}
	if (MainVF[4].IsInitialized())
	{
		MainVF[4].UnRecordVehGroup(MainOffense[1]);
		odprintf(" >Main_AttackLoop: Unreinforce group 1.\n");
	}

    // Start building the next attack group.
	if (MainOffense[0].IsInitialized())
	{
		MainOffense[0].SetTargCount(mapLynx, mapSupernova, 0);
		MainOffense[0].SetTargCount(mapPanther, mapRPG, 5 * TethysGame::NoPlayers() );
		MainOffense[0].SetTargCount(mapPanther, mapEMP, 5 * TethysGame::NoPlayers() );
	    MainOffense[0].SetTargCount(mapPanther, mapESG, 4 * TethysGame::NoPlayers() );
	}

    if (MainVF[3].IsInitialized())
	{
		MainVF[3].RecordVehReinforceGroup(MainOffense[0], 6000);
		odprintf(" >Main_AttackLoop: Updated group 0.\n");
	}

	if (MainOffense[1].IsInitialized())
	{
		MainOffense[1].SetTargCount(mapTiger, mapMicrowave, 3 * TethysGame::NoPlayers() );
		MainOffense[1].SetTargCount(mapTiger, mapRPG, 4 * TethysGame::NoPlayers() + 1);
	    MainOffense[1].SetTargCount(mapTiger, mapEMP, 4 * TethysGame::NoPlayers() + 1);
		MainOffense[1].SetTargCount(mapTiger, mapESG, 4 * TethysGame::NoPlayers() + 1);
	}

    if (MainVF[4].IsInitialized())
	{
		MainVF[4].RecordVehReinforceGroup(MainOffense[1], 6000);
		odprintf(" >Main_AttackLoop: Updated group 1.\n");
	}

}

// ----------------------------------------------------------------------------
// Eden outpost attack waves
// ----------------------------------------------------------------------------

void FraxSurvAI::Eden_Attack1()
{
	odprintf("Eden_Attack1: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(EdenOffense);
	EdenVF[1].UnRecordVehGroup(EdenOffense);
	odprintf(" >Eden_Attack1: Added units to attack group.\n");

	// Prepare attack 2's units
	EdenOffense.SetTargCount(mapLynx, mapLaser, 1 + (2 * (TethysGame::NoPlayers() + 1)));
	EdenVF[1].RecordVehReinforceGroup(EdenOffense, 7000);
	odprintf(" >Eden_Attack1: Updated Eden group.\n");

	curAttackWaveEden++;
	nextAttackTimeEden += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Eden_Attack1: End.\n");
}

void FraxSurvAI::Eden_Attack2()
{
	odprintf("Eden_Attack2: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(EdenOffense);
	EdenVF[1].UnRecordVehGroup(EdenOffense);
	odprintf(" >Eden_Attack1: Added units to attack group.\n");

	// Preapre attack 3's units.
	EdenOffense.SetTargCount(mapLynx, mapLaser, 2 + (3 * (TethysGame::NoPlayers())));
	EdenVF[1].RecordVehReinforceGroup(EdenOffense, 7000);
	odprintf(" >Eden_Attack2: Updated Eden group.\n");

	curAttackWaveEden++;
	nextAttackTimeEden += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Eden_Attack2: End.\n");
}

void FraxSurvAI::Eden_Attack3()
{
	odprintf("Eden_Attack3: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(EdenOffense);
	EdenVF[1].UnRecordVehGroup(EdenOffense);
	odprintf(" >Eden_Attack3: Added units to attack group.\n");

	// Prepare attack 4's units
	EdenOffense.SetTargCount(mapLynx, mapLaser, 2 + (2 * (TethysGame::NoPlayers() - 1)));
	EdenOffense.SetTargCount(mapLynx, mapRailGun, 3 + TethysGame::NoPlayers());
	EdenVF[1].RecordVehReinforceGroup(EdenOffense, 7000);
	odprintf(" >Eden_Attack3: Updated Eden group.\n");

	curAttackWaveEden++;
	nextAttackTimeEden += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Eden_Attack3: End.\n");
}

void FraxSurvAI::Eden_Attack4()
{
	odprintf("Eden_Attack4: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(EdenOffense);
	EdenVF[1].UnRecordVehGroup(EdenOffense);
	odprintf(" >Eden_Attack4: Added units to attack group.\n");

	EdenOffense.SetTargCount(mapLynx, mapLaser, (2 * (TethysGame::NoPlayers())));
	EdenOffense.SetTargCount(mapLynx, mapRailGun, 1 + (2 * (TethysGame::NoPlayers() + 1)));
	EdenVF[1].RecordVehReinforceGroup(EdenOffense, 7000);
	odprintf(" >Eden_Attack4: Updated Eden group.\n");

	EdenDefense.SetTargCount(mapLynx, mapLaser, 0);
	EdenDefense.SetTargCount(mapPanther, mapLaser, 10);
	odprintf(" >Eden_Attack4: Updated defenders.\n");

	Unit OldDefender;
	GroupEnumerator TakeOldDefenders(EdenDefense);
	while (TakeOldDefenders.GetNext(OldDefender))
	{
		if (OldDefender.GetType() == mapLynx && OldDefender.GetWeapon() == mapLaser)
		{
			EdenOffense.TakeUnit(OldDefender);
		}
	}

	curAttackWaveEden++;
	nextAttackTimeEden += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Eden_Attack4: End.\n");
}

void FraxSurvAI::Eden_Attack5()
{
	odprintf("Eden_Attack5: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(EdenOffense);
	EdenVF[1].UnRecordVehGroup(EdenOffense);
	odprintf(" >Eden_Attack5: Added units to attack group.\n");

	EdenOffense.SetTargCount(mapLynx, mapRailGun, 0);
	EdenOffense.SetTargCount(mapPanther, mapLaser, 3 + (2 * (TethysGame::NoPlayers())));
	EdenOffense.SetTargCount(mapPanther, mapRailGun, 4 + (2 * TethysGame::NoPlayers()));
	EdenVF[1].RecordVehReinforceGroup(EdenOffense, 7000);
	odprintf(" >Eden_Attack5: Updated Eden group.\n");

	EdenDefense.SetTargCount(mapPanther, mapRailGun, 12);
	odprintf(" >Eden_Attack5: Updated defenders.\n");

	Unit OldDefender;
	GroupEnumerator TakeOldDefenders(EdenDefense);
	while (TakeOldDefenders.GetNext(OldDefender))
	{
		if (OldDefender.GetType() == mapLynx)
		{
			EdenOffense.TakeUnit(OldDefender);
		}
	}

	curAttackWaveEden++;
	nextAttackTimeEden += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Eden_Attack5: End.\n");
}

void FraxSurvAI::Eden_Attack6()
{
	odprintf("Eden_Attack6: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(EdenOffense);
	EdenVF[1].UnRecordVehGroup(EdenOffense);
	odprintf(" >Eden_Attack6: Added units to attack group.\n");

	EdenOffense.SetTargCount(mapPanther, mapLaser, (2 * (TethysGame::NoPlayers() + 1)));
	EdenOffense.SetTargCount(mapPanther, mapRailGun, 1 + (3 * (TethysGame::NoPlayers())));
	EdenOffense.SetTargCount(mapPanther, mapThorsHammer, TethysGame::NoPlayers());
	EdenOffense.SetTargCount(mapPanther, mapAcidCloud, TethysGame::NoPlayers());

	EdenOffense.SetTargCount(mapLynx, mapLaser, 0);
	EdenOffense.SetTargCount(mapLynx, mapRailGun, 0);
	EdenOffense.SetTargCount(mapLynx, mapAcidCloud, 0);
	EdenOffense.SetTargCount(mapLynx, mapThorsHammer, 0);
	EdenVF[1].RecordVehReinforceGroup(EdenOffense, 7000);
	odprintf(" >Eden_Attack6: Updated Eden group.\n");

	EdenDefense.SetTargCount(mapPanther, mapLaser, 0);
	EdenDefense.SetTargCount(mapPanther, mapRailGun, 14);
	EdenDefense.SetTargCount(mapPanther, mapAcidCloud, 8);
	odprintf(" >Eden_Attack6: Updated defenders.\n");

	Unit OldDefender;
	GroupEnumerator TakeOldDefenders(EdenDefense);
	while (TakeOldDefenders.GetNext(OldDefender))
	{
		if (OldDefender.GetType() == mapPanther && OldDefender.GetWeapon() == mapLaser)
		{
			EdenOffense.TakeUnit(OldDefender);
		}
	}

	curAttackWaveEden++;
	nextAttackTimeEden += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Eden_Attack6: End.\n");
}

void FraxSurvAI::Eden_Attack7()
{
	odprintf("Eden_Attack7: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(EdenOffense);
	EdenVF[1].UnRecordVehGroup(EdenOffense);
	odprintf(" >Eden_Attack7: Added units to attack group.\n");

	EdenOffense.SetTargCount(mapPanther, mapLaser, TethysGame::NoPlayers());
	EdenOffense.SetTargCount(mapPanther, mapThorsHammer, 1 + (2 * (TethysGame::NoPlayers() + 1)));
	EdenOffense.SetTargCount(mapPanther, mapAcidCloud, (2 * (TethysGame::NoPlayers() + 1)));

	EdenOffense.SetTargCount(mapPanther, mapRailGun, 0);
	EdenVF[1].RecordVehReinforceGroup(EdenOffense, 7000);
	odprintf(" >Eden_Attack7: Updated Eden group.\n");

	Unit OldDefender;
	GroupEnumerator TakeOldDefenders(EdenDefense);
	while (TakeOldDefenders.GetNext(OldDefender))
	{
		if (OldDefender.GetType() == mapPanther && OldDefender.GetWeapon() == mapRailGun)
		{
			EdenOffense.TakeUnit(OldDefender);
		}
	}

	curAttackWaveEden++;
	nextAttackTimeEden += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Eden_Attack7: End.\n");
}

void FraxSurvAI::Eden_Attack8()
{
	odprintf("Eden_Attack8: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(EdenOffense);
	EdenVF[1].UnRecordVehGroup(EdenOffense);
	odprintf(" >Eden_Attack8: Added units to attack group.\n");

	EdenOffense.SetTargCount(mapTiger, mapLaser, 1 + TethysGame::NoPlayers());
	EdenOffense.SetTargCount(mapTiger, mapThorsHammer, (2 * (TethysGame::NoPlayers() + 1)));
	EdenOffense.SetTargCount(mapPanther, mapAcidCloud, (2 * (TethysGame::NoPlayers())));

	EdenOffense.SetTargCount(mapPanther, mapLaser, 0);
	EdenOffense.SetTargCount(mapPanther, mapThorsHammer, 0);
	EdenOffense.SetTargCount(mapPanther, mapAcidCloud, 0);
	EdenVF[1].RecordVehReinforceGroup(EdenOffense, 7000);
	odprintf(" >Eden_Attack8: Updated Eden group.\n");

	EdenDefense.SetTargCount(mapPanther, mapLaser, 0);
	EdenDefense.SetTargCount(mapPanther, mapRailGun, 0);
	EdenDefense.SetTargCount(mapPanther, mapThorsHammer, 0);
	EdenDefense.SetTargCount(mapPanther, mapAcidCloud, 0);
	EdenDefense.SetTargCount(mapTiger, mapThorsHammer, 10);
	EdenDefense.SetTargCount(mapTiger, mapAcidCloud, 8);
	odprintf(" >Eden_Attack8: Updated defenders.\n");

	Unit OldDefender;
	GroupEnumerator TakeOldDefenders(EdenDefense);
	while (TakeOldDefenders.GetNext(OldDefender))
	{
		if (OldDefender.GetType() == mapPanther)
		{
			EdenOffense.TakeUnit(OldDefender);
		}
	}

	// Wave 9 intentionally skipped.
	curAttackWaveEden += 2;
	nextAttackTimeEden += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Eden_Attack8: End.\n");
}

void FraxSurvAI::Eden_Attack9()
{
	odprintf("Eden_Attack9: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(EdenOffense);
	EdenVF[1].UnRecordVehGroup(EdenOffense);
	odprintf(" >Eden_Attack9: Added units to attack group.\n");

	EdenOffense.SetTargCount(mapTiger, mapLaser, 6);
	//EdenOffense.SetTargCount(mapTiger, mapRailGun, 10);
	EdenOffense.SetTargCount(mapTiger, mapThorsHammer, (2 * (TethysGame::NoPlayers() - 1)));
	EdenOffense.SetTargCount(mapTiger, mapAcidCloud, (2 * (TethysGame::NoPlayers() - 2)));

	EdenVF[1].RecordVehReinforceGroup(EdenOffense, 7000);
	odprintf(" >Eden_Attack9: Updated Eden group.\n");

	curAttackWaveEden++;
	nextAttackTimeEden += (MAIN_DELAY_MIN - 200 + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN - 200 + 1));

	odprintf("Eden_Attack9: End.\n");
}

void FraxSurvAI::Eden_Attack10()
{
	odprintf("Eden_Attack10: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(EdenOffense);
	EdenVF[1].UnRecordVehGroup(EdenOffense);
	odprintf(" >Eden_Attack10: Added units to attack group.\n");

	EdenOffense.SetTargCount(mapTiger, mapLaser, 4);
	EdenOffense.SetTargCount(mapTiger, mapRailGun, 0);
	EdenOffense.SetTargCount(mapTiger, mapThorsHammer, 4 + (4 * (TethysGame::NoPlayers() + 2)));
	EdenOffense.SetTargCount(mapTiger, mapAcidCloud, 4 + (3 * (TethysGame::NoPlayers() + 1)));

	EdenVF[1].RecordVehReinforceGroup(EdenOffense, 7000);
	odprintf(" >Eden_Attack10: Updated Eden group.\n");

	curAttackWaveEden++;
	nextAttackTimeEden += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Eden_Attack10: End.\n");
}

void FraxSurvAI::Eden_AttackFinal()
{
	odprintf("Eden_AttackFinal: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(EdenOffense);
	EdenVF[1].UnRecordVehGroup(EdenOffense);
	odprintf(" >Eden_AttackFinal: Added units to attack group.\n");

	EdenOffense.SetTargCount(mapLynx, mapLaser, 0);
	EdenOffense.SetTargCount(mapLynx, mapRailGun, 0);
	EdenOffense.SetTargCount(mapLynx, mapThorsHammer, 0);
	EdenOffense.SetTargCount(mapLynx, mapAcidCloud, 0);
	EdenOffense.SetTargCount(mapPanther, mapLaser, 0);
	EdenOffense.SetTargCount(mapPanther, mapRailGun, 0);
	EdenOffense.SetTargCount(mapPanther, mapThorsHammer, 0);
	EdenOffense.SetTargCount(mapPanther, mapAcidCloud, 0);
	EdenOffense.SetTargCount(mapTiger, mapLaser, 0);
	EdenOffense.SetTargCount(mapTiger, mapRailGun, 0);
	EdenOffense.SetTargCount(mapTiger, mapThorsHammer, 0);
	EdenOffense.SetTargCount(mapTiger, mapAcidCloud, 0);

	EdenOffense.SetTargCount(mapEvacuationTransport, mapNone, 3);
	EdenOffense.ClearGuarderdRects();
	EdenOffense.SetRect(MAP_RECT(87 + 31, 16 - 1, 90 + 31, 20 - 1));
	EdenVF[1].RecordVehReinforceGroup(EdenOffense, 9999);
	odprintf(" >Eden_AttackFinal: Updated attackers (prepare for evac).\n");

	curAttackWaveEden++;
	nextAttackTimeEden += 2200;
	odprintf("Eden_AttackFinal: End.\n");
}

void FraxSurvAI::Eden_Shutdown()
{
	odprintf("Eden_Shutdown: Begin.\n");
	// Idle the CC and cleanup any leftover groups or triggers.
	if (EdenCC.IsLive() && EdenCC.OwnerID() == aiNum && EdenCC.GetType() == mapCommandCenter)
	{
		odprintf(" >Eden_Shutdown: CC is live.  Idling.\n");
		EdenCC.DoIdle();
		odprintf(" >Eden_Shutdown: CC idled.\n");
	}

	// Cleanup groups and triggers.  Merge existing groups to main base defenses.
	odprintf(" >Eden_Shutdown: Cleaning up.\n");
	Unit ToRemove;
	if (EdenCommon.IsInitialized())
	{
		SafeDestroyGroup(EdenCommon);
		odprintf(" >Eden_Shutdown: Common ore group destroyed.\n");
	}

	if (EdenVF[0].IsInitialized())
	{
		SafeDestroyGroup(EdenVF[0]);
		odprintf(" >Eden_Shutdown: VF 0 destroyed.\n");
	}

	if (EdenVF[1].IsInitialized())
	{
		SafeDestroyGroup(EdenVF[1]);
		odprintf(" >Eden_Shutdown: VF 1 destroyed.\n");
	}

	if (EdenRepairs.IsInitialized())
	{
		SafeDestroyGroup(EdenRepairs);
		odprintf(" >Eden_Shutdown: Repair group destroyed.\n");
	}

	GroupEnumerator DeleteAll(Player[aiNum].GetDefaultGroup());
	while (DeleteAll.GetNext(ToRemove))
	{
		Player[aiNum].GetDefaultGroup().RemoveUnit(ToRemove);
	}

	EdenDefense.ClearGuarderdRects();
	EdenDefense.SetRect(MAP_RECT(115 + 31, 52 - 1, 124 + 31, 60 - 1));
	EdenDefense.AddGuardedRect(MAP_RECT(57 + 31, 9 - 1, 91 + 31, 31 - 1));
	EdenDefense.AddGuardedRect(MAP_RECT(106 + 31, 68 - 1, 128 + 31, 79 - 1));
	odprintf(" >Eden_Shutdown: Relocated surviving Eden defenders.\n");
	//EdenDefense.SetDeleteWhenEmpty(1);
	//odprintf(" >Eden_Shutdown: Set defenders delete when empty.\n");
	//EdenOffense.SetDeleteWhenEmpty(1);
	//odprintf(" >Eden_Shutdown: Set attackers delete when empty.\n");
	odprintf("Eden_Shutdown: End.\n");
}

// ----------------------------------------------------------------------------
// Plymouth outpost attack waves
// ----------------------------------------------------------------------------

void FraxSurvAI::Plymouth_Attack1()
{
	odprintf("Plymouth_Attack1: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(PlymouthOffense);
	odprintf(" >Plymouth_Attack1: Added units to attack group.\n");
	PlymouthVF[1].UnRecordVehGroup(PlymouthOffense);
	odprintf(" >Plymouth_Attack1: Unreinforced group 0.\n");

	PlymouthOffense.SetTargCount(mapLynx, mapStickyfoam, (TethysGame::NoPlayers() - 1));
	PlymouthOffense.SetTargCount(mapLynx, mapMicrowave, (2 * (TethysGame::NoPlayers()) - 1));
	PlymouthOffense.SetTargCount(mapLynx, mapStarflare, 2);
	PlymouthVF[1].RecordVehReinforceGroup(PlymouthOffense, 7000);
	odprintf(" >Plymouth_Attack1: Updated attackers.\n");

	curAttackWavePlym++;
	nextAttackTimePlym += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Plymouth_Attack1: End.\n");
}

void FraxSurvAI::Plymouth_Attack2()
{
	odprintf("Plymouth_Attack2: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(PlymouthOffense);
	odprintf(" >Plymouth_Attack2: Added units to attack group.\n");
	PlymouthVF[1].UnRecordVehGroup(PlymouthOffense);
	odprintf(" >Plymouth_Attack2: Unreinforced offense.\n");

	PlymouthOffense.SetTargCount(mapLynx, mapMicrowave, (2 * (TethysGame::NoPlayers() + 1) + 1));
	PlymouthOffense.SetTargCount(mapLynx, mapStickyfoam, TethysGame::NoPlayers() + 2);
	PlymouthOffense.SetTargCount(mapLynx, mapStarflare, 3);

	PlymouthVF[1].RecordVehReinforceGroup(PlymouthOffense, 7000);
	odprintf(" >Plymouth_Attack2: Updated attackers.\n");

	curAttackWavePlym++;
	nextAttackTimePlym += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Plymouth_Attack2: End.\n");
}

void FraxSurvAI::Plymouth_Attack3()
{
	odprintf("Plymouth_Attack3: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(PlymouthOffense);
	odprintf(" >Plymouth_Attack3: Added units to attack group.\n");
	PlymouthVF[1].UnRecordVehGroup(PlymouthOffense);
	odprintf(" >Plymouth_Attack3: Unreinforced offense.\n");

	PlymouthOffense.SetTargCount(mapLynx, mapRPG, (2 * (TethysGame::NoPlayers())) - 1);
	PlymouthOffense.SetTargCount(mapLynx, mapMicrowave, (TethysGame::NoPlayers()) + 2);
	PlymouthOffense.SetTargCount(mapLynx, mapEMP, (2 * (TethysGame::NoPlayers() - 1)));
	PlymouthOffense.SetTargCount(mapLynx, mapStickyfoam, 0);
	PlymouthOffense.SetTargCount(mapLynx, mapStarflare, 0);
	PlymouthVF[1].RecordVehReinforceGroup(PlymouthOffense, 7000);
	odprintf(" >Plymouth_Attack3: Updated attackers.\n");

	// Update defense while we're at it
	PlymouthDefense.SetTargCount(mapLynx, mapStickyfoam, 0);
	PlymouthDefense.SetTargCount(mapLynx, mapRPG, 9);
	PlymouthDefense.SetTargCount(mapLynx, mapEMP, 8);
	odprintf(" >Plymouth_Attack3: Updated defenders.\n");

	Unit OldDefender;
	GroupEnumerator TakeOldDefenders(PlymouthDefense);
	while (TakeOldDefenders.GetNext(OldDefender))
	{
		if (OldDefender.GetType() == mapLynx && OldDefender.GetWeapon() == mapStickyfoam)
		{
			PlymouthOffense.TakeUnit(OldDefender);
		}
	}

	curAttackWavePlym++;
	nextAttackTimePlym += (MAIN_DELAY_MIN - 400 + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN - 400 + 1));

	odprintf("Plymouth_Attack3: End.\n");
}

void FraxSurvAI::Plymouth_Attack4()
{
	odprintf("Plymouth_Attack4: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(PlymouthOffense);
	odprintf(" >Plymouth_Attack4: Added units to attack group.\n");
	PlymouthVF[1].UnRecordVehGroup(PlymouthOffense);
	odprintf(" >Plymouth_Attack4: Unreinforced offense.\n");

	PlymouthOffense.SetTargCount(mapLynx, mapRPG, (3 * (TethysGame::NoPlayers())) - 1);
	PlymouthOffense.SetTargCount(mapLynx, mapMicrowave, 2 * TethysGame::NoPlayers());
	PlymouthOffense.SetTargCount(mapLynx, mapEMP, (2 * (TethysGame::NoPlayers())));
	//PlymouthOffense.SetTargCount(mapLynx, mapESG, TethysGame::NoPlayers() - 1 );
	PlymouthVF[1].RecordVehReinforceGroup(PlymouthOffense, 7000);
	odprintf(" >Plymouth_Attack4: Updated attackers.\n");

	curAttackWavePlym++;
	nextAttackTimePlym += (MAIN_DELAY_MIN - 200 + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN - 200 + 1));

	odprintf("Plymouth_Attack4: End.\n");
}

void FraxSurvAI::Plymouth_Attack5()
{
	odprintf("Plymouth_Attack5: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(PlymouthOffense);
	odprintf(" >Plymouth_Attack5: Added units to attack group.\n");
	PlymouthVF[1].UnRecordVehGroup(PlymouthOffense);
	odprintf(" >Plymouth_Attack5: Unreinforced offense.\n");

	PlymouthOffense.SetTargCount(mapLynx, mapMicrowave, 12 * TethysGame::NoPlayers());
	PlymouthOffense.SetTargCount(mapLynx, mapRPG, 0);
	PlymouthOffense.SetTargCount(mapLynx, mapEMP, 0);
	PlymouthOffense.SetTargCount(mapLynx, mapESG, 0);
	PlymouthVF[1].RecordVehReinforceGroup(PlymouthOffense, 7000);
	odprintf(" >Plymouth_Attack5: Updated attackers.\n");

	// Update defense while we're at it
	PlymouthDefense.SetTargCount(mapLynx, mapMicrowave, 0);
	PlymouthDefense.SetTargCount(mapLynx, mapRPG, 15);
	PlymouthDefense.SetTargCount(mapLynx, mapEMP, 10);
	PlymouthDefense.SetTargCount(mapLynx, mapESG, 5);
	odprintf(" >Plymouth_Attack5: Updated defenders.\n");

	Unit OldDefender;
	GroupEnumerator TakeOldDefenders(PlymouthDefense);
	while (TakeOldDefenders.GetNext(OldDefender))
	{
		if (OldDefender.GetType() == mapLynx && OldDefender.GetWeapon() == mapMicrowave)
		{
			PlymouthOffense.TakeUnit(OldDefender);
		}
	}

	curAttackWavePlym++;
	nextAttackTimePlym += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Plymouth_Attack5: End.\n");
}

void FraxSurvAI::Plymouth_Attack6()
{
	odprintf("Plymouth_Attack6: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(PlymouthOffense);
	odprintf(" >Plymouth_Attack6: Added units to attack group.\n");
	PlymouthVF[1].UnRecordVehGroup(PlymouthOffense);
	odprintf(" >Plymouth_Attack6: Unreinforced offense.\n");

	PlymouthOffense.SetTargCount(mapPanther, mapRPG, (3 * (TethysGame::NoPlayers() - 2)));
	PlymouthOffense.SetTargCount(mapPanther, mapEMP, (2 * (TethysGame::NoPlayers())));
	PlymouthOffense.SetTargCount(mapPanther, mapESG, TethysGame::NoPlayers());
	PlymouthOffense.SetTargCount(mapLynx, mapMicrowave, 0);
	PlymouthVF[1].RecordVehReinforceGroup(PlymouthOffense, 7000);
	odprintf(" >Plymouth_Attack6: Updated attackers.\n");

	PlymouthDefense.SetTargCount(mapLynx, mapMicrowave, 0);
	PlymouthDefense.SetTargCount(mapLynx, mapRPG, 0);
	PlymouthDefense.SetTargCount(mapLynx, mapEMP, 0);
	PlymouthDefense.SetTargCount(mapLynx, mapESG, 0);
	PlymouthDefense.SetTargCount(mapPanther, mapRPG, 12);
	PlymouthDefense.SetTargCount(mapPanther, mapEMP, 14);
	PlymouthDefense.SetTargCount(mapPanther, mapESG, 6);

	Unit OldDefender;
	GroupEnumerator TakeOldDefenders(PlymouthDefense);
	while (TakeOldDefenders.GetNext(OldDefender))
	{
		if (OldDefender.GetType() == mapLynx)
		{
			PlymouthOffense.TakeUnit(OldDefender);
		}
	}

	curAttackWavePlym++;
	nextAttackTimePlym += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Plymouth_Attack6: End.\n");
}

void FraxSurvAI::Plymouth_Attack7()
{
	odprintf("Plymouth_Attack7: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(PlymouthOffense);
	odprintf(" >Plymouth_Attack7: Added units to attack group.\n");
	PlymouthVF[1].UnRecordVehGroup(PlymouthOffense);
	odprintf(" >Plymouth_Attack7: Unreinforced offense.\n");

	PlymouthOffense.SetTargCount(mapPanther, mapRPG, (2 * (TethysGame::NoPlayers() + 1)));
	PlymouthOffense.SetTargCount(mapPanther, mapMicrowave, 2 * TethysGame::NoPlayers());
	PlymouthOffense.SetTargCount(mapPanther, mapEMP, (3 * (TethysGame::NoPlayers() + 1)));
	PlymouthOffense.SetTargCount(mapPanther, mapESG, TethysGame::NoPlayers());
	PlymouthOffense.SetTargCount(mapLynx, mapSupernova, 6);
	PlymouthVF[1].RecordVehReinforceGroup(PlymouthOffense, 7000);
	odprintf(" >Plymouth_Attack7: Updated attackers.\n");

	curAttackWavePlym++;
	nextAttackTimePlym += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Plymouth_Attack7: End.\n");
}

void FraxSurvAI::Plymouth_Attack8()
{
	odprintf("Plymouth_Attack8: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(PlymouthOffense);
	odprintf(" >Plymouth_Attack8: Added units to attack group.\n");
	PlymouthVF[1].UnRecordVehGroup(PlymouthOffense);
	odprintf(" >Plymouth_Attack8: Unreinforced offense.\n");

	PlymouthOffense.SetTargCount(mapPanther, mapRPG, (2 * (TethysGame::NoPlayers() - 2)));
	PlymouthOffense.SetTargCount(mapPanther, mapMicrowave, TethysGame::NoPlayers());
	PlymouthOffense.SetTargCount(mapPanther, mapEMP, (2 * (TethysGame::NoPlayers() + 2)));
	PlymouthOffense.SetTargCount(mapPanther, mapESG, (3 * TethysGame::NoPlayers()));
	PlymouthOffense.SetTargCount(mapLynx, mapSupernova, 0);
	PlymouthVF[1].RecordVehReinforceGroup(PlymouthOffense, 7000);
	odprintf(" >Plymouth_Attack8: Updated attackers.\n");

	// Attack wave 9 intentionally skipped
	curAttackWavePlym += 2;
	nextAttackTimePlym += (MAIN_DELAY_MIN -200 + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN - 200 + 1));

	odprintf("Plymouth_Attack8: End.\n");
}

void FraxSurvAI::Plymouth_Attack9()
{
	odprintf("Plymouth_Attack9: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(PlymouthOffense);
	odprintf(" >Plymouth_Attack9: Added units to attack group.\n");
	PlymouthVF[1].UnRecordVehGroup(PlymouthOffense);
	odprintf(" >Plymouth_Attack9: Unreinforced offense.\n");

	PlymouthOffense.SetTargCount(mapTiger, mapRPG, (2 * (TethysGame::NoPlayers() - 1)));
	PlymouthOffense.SetTargCount(mapTiger, mapMicrowave, TethysGame::NoPlayers() - 1);
	PlymouthOffense.SetTargCount(mapTiger, mapEMP, (3 * (TethysGame::NoPlayers() - 1)));
	PlymouthOffense.SetTargCount(mapTiger, mapESG, (3 * TethysGame::NoPlayers() - 2));

	PlymouthOffense.SetTargCount(mapPanther, mapRPG, 0);
	PlymouthOffense.SetTargCount(mapPanther, mapMicrowave, 0);
	PlymouthOffense.SetTargCount(mapPanther, mapEMP, 0);
	PlymouthOffense.SetTargCount(mapPanther, mapESG, 0);
	PlymouthVF[1].RecordVehReinforceGroup(PlymouthOffense, 7000);
	odprintf(" >Plymouth_Attack9: Updated attackers.\n");

	curAttackWavePlym++;
	nextAttackTimePlym += (MAIN_DELAY_MIN + TethysGame::GetRand(MAIN_DELAY_MAX - MAIN_DELAY_MIN + 1));

	odprintf("Plymouth_Attack9: End.\n");
}

void FraxSurvAI::Plymouth_AttackFinal()
{
	odprintf("Plymouth_AttackFinal: Begin.\n");
	// Issue the attack order and then clean up this group.
	Attacking[1].TakeAllUnits(PlymouthOffense);
	PlymouthVF[1].UnRecordVehGroup(PlymouthOffense);
	odprintf(" >Plymouth_AttackFinal: Added units to attack group.\n");

	PlymouthOffense.SetTargCount(mapLynx, mapMicrowave, 0);
	PlymouthOffense.SetTargCount(mapLynx, mapRPG, 0);
	PlymouthOffense.SetTargCount(mapLynx, mapStickyfoam, 0);
	PlymouthOffense.SetTargCount(mapLynx, mapEMP, 0);
	PlymouthOffense.SetTargCount(mapLynx, mapESG, 0);
	PlymouthOffense.SetTargCount(mapLynx, mapStarflare, 0);
	PlymouthOffense.SetTargCount(mapLynx, mapSupernova, 0);

	PlymouthOffense.SetTargCount(mapPanther, mapMicrowave, 0);
	PlymouthOffense.SetTargCount(mapPanther, mapRPG, 0);
	PlymouthOffense.SetTargCount(mapPanther, mapStickyfoam, 0);
	PlymouthOffense.SetTargCount(mapPanther, mapEMP, 0);
	PlymouthOffense.SetTargCount(mapPanther, mapESG, 0);
	PlymouthOffense.SetTargCount(mapPanther, mapStarflare, 0);
	PlymouthOffense.SetTargCount(mapPanther, mapSupernova, 0);

	PlymouthOffense.SetTargCount(mapTiger, mapMicrowave, 0);
	PlymouthOffense.SetTargCount(mapTiger, mapRPG, 0);
	PlymouthOffense.SetTargCount(mapTiger, mapStickyfoam, 0);
	PlymouthOffense.SetTargCount(mapTiger, mapEMP, 0);
	PlymouthOffense.SetTargCount(mapTiger, mapESG, 0);
	PlymouthOffense.SetTargCount(mapTiger, mapStarflare, 0);
	PlymouthOffense.SetTargCount(mapTiger, mapSupernova, 0);

	PlymouthOffense.SetTargCount(mapEvacuationTransport, mapNone, 3);
	PlymouthOffense.ClearGuarderdRects();
	PlymouthOffense.SetRect(MAP_RECT(87 + 31, 16 - 1, 90 + 31, 20 - 1));
	PlymouthVF[1].RecordVehReinforceGroup(PlymouthOffense, 9999);
	odprintf(" >Plymouth_AttackFinal: Updated attackers (prepare for evac).\n");

	curAttackWavePlym++;
	nextAttackTimePlym += 2200;

	odprintf("Plymouth_AttackFinal: End.\n");
}

void FraxSurvAI::Plymouth_Shutdown()
{
	odprintf("Plymouth_Shutdown: Begin.\n");
	// Idle the CC and cleanup any leftover groups or triggers.
	if (PlymOutpostCC.IsLive() && PlymOutpostCC.OwnerID() == aiNum && PlymOutpostCC.GetType() == mapCommandCenter)
	{
		odprintf(" >Plymouth_Shutdown: CC is live.  Idling.\n");
		PlymOutpostCC.DoIdle();
		odprintf(" >Plymouth_Shutdown: CC idled.\n");
	}

	// Cleanup groups and triggers.  Merge existing groups to main base defenses.
	odprintf(" >Plymouth_Shutdown: Cleaning up.\n");

	Unit ToRemove;
	if (PlymouthCommon[0].IsInitialized())
	{
		SafeDestroyGroup(PlymouthCommon[0]);
		odprintf(" >Plymouth_Shutdown: Common ore group 0 destroyed.\n");
	}

	if (PlymouthCommon[1].IsInitialized())
	{
		SafeDestroyGroup(PlymouthCommon[1]);
		odprintf(" >Plymouth_Shutdown: Common ore group 1 destroyed.\n");
	}

	if (PlymouthVF[0].IsInitialized())
	{
		SafeDestroyGroup(PlymouthVF[0]);
		odprintf(" >Plymouth_Shutdown: VF 0 destroyed.\n"); \
	}

	if (PlymouthVF[1].IsInitialized())
	{
		SafeDestroyGroup(PlymouthVF[1]);
		odprintf(" >Plymouth_Shutdown: VF 1 destroyed.\n");
	}

	if (PlymouthSF.IsInitialized())
	{
		SafeDestroyGroup(PlymouthSF);
		odprintf(" >Plymouth_Shutdown: SF destroyed.\n");
	}

	GroupEnumerator DeleteAll(Player[aiNum].GetDefaultGroup());
	while (DeleteAll.GetNext(ToRemove))
	{
		Player[aiNum].GetDefaultGroup().RemoveUnit(ToRemove);
	}

	PlymouthDefense.ClearGuarderdRects();
	PlymouthDefense.SetRect(MAP_RECT(115 + 31, 52 - 1, 124 + 31, 60 - 1));
	PlymouthDefense.AddGuardedRect(MAP_RECT(57 + 31, 9 - 1, 91 + 31, 31 - 1));
	PlymouthDefense.AddGuardedRect(MAP_RECT(106 + 31, 68 - 1, 128 + 31, 79 - 1));
	odprintf(" >Plymouth_Shutdown: Relocated surviving Plymouth defenders.\n");
	//PlymouthDefense.SetDeleteWhenEmpty(1);
	//odprintf(" >Plymouth_Shutdown: Set defenders delete when empty.\n");
	//PlymouthOffense.SetDeleteWhenEmpty(1);
	//odprintf(" >Plymouth_Shutdown: Set attackers delete when empty.\n");

	odprintf("Plymouth_Shutdown: End.\n");
}

// ----------------------------------------------------------------------------
// Patrol groups
// ----------------------------------------------------------------------------

void FraxSurvAI::AttackersAttacked()
{
	bool CloseEnough = false;

	Unit Unit1;
	if (!AttackGroupAttacked && Attacking[0].IsInitialized())
	{
		GroupEnumerator GetAttackingUnit(Attacking[0]);
		while (GetAttackingUnit.GetNext(Unit1))
		{
			Unit MaybeEnemy;
			InRectEnumerator GetNearbyUnits(MAP_RECT(Unit1.Location().x - 5, Unit1.Location().y - 5, Unit1.Location().x + 5, Unit1.Location().y + 5));
			while (GetNearbyUnits.GetNext(MaybeEnemy))
			{
				if (MaybeEnemy.OwnerID() < aiNum && MaybeEnemy.GetType() != mapScout)
				{
					CloseEnough = true;
					break;
				}
			}
			if (CloseEnough)
			{
				break;
			}
		}

		if (!AttackGroupAttacked && (CloseEnough || Attacking[0].HasBeenAttacked()))
		{
			Attacking[0].ClearPatrolMode();
			Attacking[0].SetAttackType(mapCommandCenter);
			Attacking[0].DoAttackEnemy();

			// Destroy this trigger
			AttackGroupAttacked = true;

			if (attackRoute != NULL)
			{
				delete[] attackRoute;
				attackRoute = NULL;
			}
		}
	}

	else
	{
		odprintf("ERROR: Attack group not initialized!!!\n");
	}
}

void FraxSurvAI::PG1_Attacked()
{
	if (!Patrol1Attacked && PatrolGroups[0].HasBeenAttacked())
	{
		PatrolGroups[0].ClearPatrolMode();
		PatrolGroups[0].SetAttackType(mapCommandCenter);
		PatrolGroups[0].DoAttackEnemy();
		Patrol1Attacked = true;
	}
}

void FraxSurvAI::PG2_Attacked()
{
	if (!Patrol2Attacked && PatrolGroups[1].HasBeenAttacked())
	{
		PatrolGroups[1].ClearPatrolMode();
		PatrolGroups[1].SetAttackType(mapCommandCenter);
		PatrolGroups[1].DoAttackEnemy();
		Patrol2Attacked = true;
	}
}