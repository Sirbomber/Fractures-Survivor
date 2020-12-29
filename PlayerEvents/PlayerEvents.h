#pragma once

#include <Outpost2DLL.h>

extern Trigger playerRebuiltAgridome[7], shareMicrobeWallTech, shareLavaWallTech;
extern bool shareTechEventsCreated;

void CreatePlayerEvents(int playerId);

void NoStructures(int playerId);
void NoAgridomes(int playerId);

Export void ShareMicrobeWallTech();
Export void ShareLavaWallTech();

Export void NoStructures_P0();
Export void NoStructures_P1();
Export void NoStructures_P2();
Export void NoStructures_P3();
Export void NoStructures_P4();
Export void NoStructures_P5();
Export void NoStructures_P6();

Export void NoAgridomes_P0();
Export void NoAgridomes_P1();
Export void NoAgridomes_P2();
Export void NoAgridomes_P3();
Export void NoAgridomes_P4();
Export void NoAgridomes_P5();
Export void NoAgridomes_P6();

Export void RebuiltAgridome_P0();
Export void RebuiltAgridome_P1();
Export void RebuiltAgridome_P2();
Export void RebuiltAgridome_P3();
Export void RebuiltAgridome_P4();
Export void RebuiltAgridome_P5();
Export void RebuiltAgridome_P6();
