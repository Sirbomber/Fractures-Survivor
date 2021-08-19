
#include <climits>

#include "Tethys/API/Unit.h"
#include "Tethys/API/Player.h"
#include "Tethys/API/TethysGame.h"
#include "Tethys/API/Enumerators.h"
#include "Tethys/API/Trigger.h"
#include "Tethys/API/Mission.h"
#include "Tethys/Game/TubeConnection.h"

#include "Util.h"

using namespace Tethys;
using namespace TethysAPI;

// =====================================================================================================================
void DirtRepair(
  int  multiplier,
  int  divisor)
{
  static int callNumber = 0;

  const bool allowNegativeDamage = divisor < 0;
  if (allowNegativeDamage) {
    divisor = -divisor;
  }

  for (int i = 0; i < TethysGame::NumPlayers(); ++i) {
    if (Player[i].IsHuman()) {
      Unit curUnit;
      for (PlayerBuildingEnum buildingEnum(i, MapID::None); buildingEnum.GetNext(curUnit);) {
        auto*const pMo = curUnit.GetMapObject();

        if ((curUnit.IsLive()      == false)                  ||
            (curUnit.GetCommand()  == CommandType::Develop)   ||
            (curUnit.GetCommand()  == CommandType::UnDevelop) ||
            (pMo->pTubeConnection_ == nullptr))
        {
          continue;
        }

        int dmg = curUnit.GetDamage();
        if (dmg > 0) {
          const int dmgPrevented = pMo->pTubeConnection_->CalculateDIRTDamagePrevention();
          switch (dmgPrevented)
          {
          case 2500:            // heal HP every 1/4 mark
            break;

          case 1775:            // heal HP every 1/2 mark
            if ((callNumber % 2) != 0)
              continue;
            break;

          case 1250:            // heal HP every mark
            if (callNumber != 0)
              continue;
            break;

          case 625:
          case 0:
          default:
            continue;
          }

          //-- dmg; // Flat 1 damage reduction
          int repairAmount = (curUnit.GetMaxHitpoints() * multiplier) / divisor;
          dmg -= (multiplier >= 0) ? max(1, repairAmount) : min(-1, repairAmount);

          if (allowNegativeDamage) {
            if (dmg == INT_MIN) {
              dmg = INT_MIN + 1;
            }
          }
          else if (dmg < 0) {
            dmg = 0;
          }

          curUnit.SetDamage(dmg);
        }
      }
    }
  }

  callNumber = (callNumber + 1) % 4;
}

// =====================================================================================================================
void SetupDirtRepair() {
  CreateTimeTrigger(1 * 25, "DirtRepairProc", false);
}

// =====================================================================================================================
MISSION_API void DirtRepairProc() {
  return DirtRepair(3, 1000);
}
