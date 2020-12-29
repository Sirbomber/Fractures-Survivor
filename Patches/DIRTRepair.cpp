#include <Outpost2DLL.h>
#include <hfl.h>
#include "OP2Types\OP2Units.h"
#include "HookHelper.h"
#include "DIRTRepair.h"


void SetupDIRTRepair()
{
	CreateTimeTrigger(1, 0, 1 * 25, "DIRTRepair");
}


void DIRTRepair()
{
	static int callNumber = 0;

	if (!isInited)
		return;

	int (__thiscall *CalcDirtDamagePrevention)(void *dirtHelperPtr) = (int (__thiscall*)(void*))OP2Addr(0x430050); // returns % prevented dmg * 10000 (2500 = 25% damage prevention)

	UnitEx curUnit;
	int dmg, dmgPrevented;

	for (int i = 0; i < TethysGame::NoPlayers(); i ++)
	{
		PlayerBuildingEnum buildingEnum(i, mapNone);
		while (buildingEnum.GetNext(curUnit))
		{
			if (!curUnit.IsLive()
				|| curUnit.GetLastCommand() == ctMoDevelop
				|| curUnit.GetLastCommand() == ctMoUnDevelop)
				continue;

			dmg = curUnit.GetDamage();
			if (dmg > 0)
			{
				dmgPrevented = CalcDirtDamagePrevention((*unitArray)[curUnit.unitID].dirtHelperPtr);
				switch (dmgPrevented)
				{
				case 2500:						// heal 1 HP every 1/4 mark
					break;
				case 1775:						// heal 1 HP every 1/2 mark
					if ((callNumber % 2) != 0)
						continue;
					break;
				case 1250:						// heal 1 HP every mark
					if (callNumber != 0)
						continue;
					break;
				case 625:
					continue;
				case 0:
					continue;
				default:
					continue;
				}

				dmg --;
				if (dmg < 0)
					dmg = 0;
				curUnit.SetDamage(dmg);
			}
		}
	}

	callNumber = (callNumber + 1) % 4;
}