#ifndef MISCPATCHES_H
#define MISCPATCHES_H

bool SetStructurePatches();
bool SetWallPatch();
bool SetMeteorDefensePatch();
bool SetPalettePatch();
bool SetMissionEndHook();
bool SetEvacuationModulePatch();
bool SetAllyMessagePatch();
bool SetFoodStoresArePlentifulPatch();

void Unit_MoBuildWall_Trampoline();
void Unit_MoBuildWallDone_Trampoline();
void Unit_MoBuildWallDoneAll_Trampoline();
void Unit_DrawUnit_StatusLight_Trampoline();
void PaletteTrampoline();
void MissionEndTrampoline();
void AllianceFormedBrokenWithMessage_Trampoline();
void AllianceFormedBrokenFromMessage_Trampoline();

#endif