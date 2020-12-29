#ifndef BLIGHTHOOK_H
#define BLIGHTHOOK_H

#include <OP2Types.h>

extern MapTile *infectedTiles;
extern OP2BlightMgr *blightObj;
extern bool blightHooked;

extern FightGroup microbePoofGroup;

bool SetBlightHook(bool boolOn);
void SetBlightVisible(bool isVisible);
int TileOffset(LOCATION where);
int TileOffset(int tileX, int tileY);
void BlightHook_InfectionProc(int playerId);
void BlightHook_DoInfect(UnitEx & unit);
void LevelDll_RaiseUnitTransferredEvent_Trampoline();

Export void FindInfectedTankTargets();

// Extended unit flag for marking vehicles that have entered infected terrain
#define UNIT_EX_INFECTEDVEHICLE			0x40000000

#endif