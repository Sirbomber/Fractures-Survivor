#include "FraxSurvAi.h"

// Research + assigned scientist list
int StdTechList[][2] = { {8306,12},		// Enhanced Defensive Fortifications
						{7206,12},      // Scout-Class Drive Train Refit						
						{8302,18},		// Efficiency Engineering (Plymouth)
						{8203,12},		// High-Powered Explosives
						{7202,14},		// Hot-Cracking Column Efficiency
						{-1,-1} };		// End of list

int AdvTechList[][2] = { {7201,17},		// Rare Ore Extraction
						{7203,17},		// Smelter Postprocessing
						{8309,11},		// Reinforced Panther Construction
						{10303,17},		// Advanced Armoring Systems
						{8407,14},		// Panther Drive Train Refit
						{7212,14},		// Extended-Range Projectile Launcher
						{9705,15},		// Advanced Microwave Power System
						{8202,16},		// Panther Weapons Systems Upgrade
						{10306,16},		// Grenade Loading Mechanism (Plymouth)
						{11999,10},		// Tiger Speed Modification
						{-1,-1} };		// End of list

void FraxSurvAI::LabStuff()
{
	odprintf("Begin LabStuff\n");
	// Standard Lab
	if (!StdLab.IsLive())
	{
		// Check if the lab has been rebuilt.
		UnitEx i;
		PlayerBuildingEnum GetSLab(aiNum, mapStandardLab);
		if (GetSLab.GetNext(i) && i.IsLive())
		{
			StdLab = i;
		}
	}

	else if (TethysGame::Tick() > LastResearchTime + 10000)
	{
		if (!StdLab.IsBusy())
		{
			// Make sure we're not already at the end of the list.
			if (StdTechList[curStdTech][0] != -1)
			{
				//odprintf("Next tech: %d.\n",StdTechList[curStdTech][0]);
				// If we already have the current tech, move down the list.
				if (Player[aiNum].HasTechnology(StdTechList[curStdTech][0]))
				{
					curStdTech++;
					LastResearchTime = TethysGame::Tick();
				}

				//odprintf("Trying to research tech %d at the Standard Lab.\n",StdTechList[curStdTech][0]);
				// Have to check if we're at the end of the list again.
				if (StdTechList[curStdTech][0] != -1)
				{
					// Do research!
					int test = -1;
					for (int i = 0; i < ((OP2Research*)researchObj)->numTechs; ++i)
					{
						if (((OP2Research*)researchObj)->techInfos[i]->techID == StdTechList[curStdTech][0])
						{
							test = i;
						}
					}

					if (test != -1)
					{
						StdLab.DoResearch(test, StdTechList[curStdTech][1]);
					}
				}
			}
		}
	}

	// Advanced Lab
	if (!AdvLab.IsLive())
	{
		// Check if the lab has been rebuilt.
		UnitEx i;
		PlayerBuildingEnum GetALab(aiNum, mapAdvancedLab);
	    if (GetALab.GetNext(i) && i.IsLive())
		{
			AdvLab = i;
		}
	}

	else if (TethysGame::Tick() > LastResearchTime + 10000)
	{
		if (!AdvLab.IsBusy())
		{
			// Make sure we're not already at the end of the list.
			if (AdvTechList[curAdvTech][0] != -1)
			{
				// If we already have the current tech, move down the list.
				if (Player[aiNum].HasTechnology(AdvTechList[curAdvTech][0]))
				{
					curAdvTech++;
					LastResearchTime = TethysGame::Tick();
				}

				// Have to check if we're at the end of the list again.
				if (AdvTechList[curAdvTech][0] != -1)
				{
					// Do research!
					int test = -1;
					for (int i = 0; i < ((OP2Research*)researchObj)->numTechs; ++i)
					{
						if (((OP2Research*)researchObj)->techInfos[i]->techID == AdvTechList[curAdvTech][0])
						{
							test = i;
						}
					}

					if (test != -1)
					{
						AdvLab.DoResearch(test, AdvTechList[curAdvTech][1]);
					}
				}
			}
		}
	}
odprintf("End LabStuff\n");
}
