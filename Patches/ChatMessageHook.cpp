
#include "Patcher.h"
#include "Util.h"

using namespace Patcher;

// =====================================================================================================================
bool SetChatMessageHook(
  bool            enable,
  void (__cdecl*  pfnHook)(char* pChatText, int playerNum),
  bool            allowCheats)
{
  static PatchContext patcher;
  bool success = true;

  if (enable && (pfnHook != nullptr)) {
    // Replace CheckChatForCheatCode() with our own function.
    if (allowCheats) {
      static decltype(pfnHook) pfnUserHook          = nullptr;
      static decltype(pfnHook) pfnCheckForCheatCode = nullptr;

      pfnUserHook = pfnHook;
      patcher.Hook(0x58642A,
                   [](char* pChatText, int playerNum)
                     { pfnUserHook(pChatText, playerNum);  pfnCheckForCheatCode(pChatText, playerNum); },
                   &pfnCheckForCheatCode);

      success = (patcher.GetStatus() == Status::Ok);
    }
    else {
      success = (patcher.Hook(0x58642A, pfnHook) == Status::Ok);
    }
  }

  if ((enable == false) || (success == false)) {
    success &= ((patcher.RevertAll() == Status::Ok));
  }

  return success;
}
