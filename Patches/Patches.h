
#pragma once

bool SetSuperSpeedPatch(bool enable);

bool SetStructureDamagePatches(bool enable);
bool SetAdvancedLabMouseoverPatch(bool enable);
bool SetStarshipCostPatch(int numColonistsRequired, int numFoodRequired);  // -1 = Disable
bool SetMissionEndHook(bool enable, void(__cdecl* pfnHook)() = nullptr);
bool SetMissionEndHook(bool enable, const char* pWinCutscene, const char* pLoseCutscene);
bool SetInstantDisasterNoWarnPatch(bool enable);

bool SetChatMessageHook(
  bool enable, void (__cdecl* pfnHook)(char* pChatText, int playerNum), bool allowCheats = false);

bool SetCreateUnitHook(bool enable);

bool SetBlightPatch(bool enable);
void SetBlightVisible(bool visible);
void BlightInfectionProc(int playerId);  // Call for each player during AIProc()

bool SetMissilePatches(bool enable);
bool SetNukePatch(bool enable);

bool SetAcidTripPatch(bool enable);
bool SetMoreAcid(bool enable);
bool CleanupAcidTrip();
bool RandomizeWeaponColors(bool enable);
bool RandomizeUiColors(bool enable);
bool RandomizeBlightColors(bool enable);
bool RandomizeWeaponLut(bool enable);
