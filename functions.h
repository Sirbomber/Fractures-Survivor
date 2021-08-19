#pragma once

// These functions all deal with game initialization
void DoSurvSetup();     // Handles morale, day/night
void DoRandomBases();   // Randomizes player starting locations
void SetupBlight();     // Set up us the Blight
void SetupTutorial();   // Creates a dummy victory condition that explains how the map is played
void SetupVictory();    // Creates victory (and defeat) conditions
void SetAllLava();      // Sets all lava rock to lava-possible.
void SpawnPlayer(int i, int x, int y);
void PlaceOre();

// Disaster setup
void ConfigureDisasterCreator();
void EnableMeteorsAndMediumPower();
void EnableStormsAndMakeMediumDisastersMoreLikely();
void EnableHighPowerDisasters();
void EnableVortexes();
void EnableApocalypticDisasters();
void EnableEndgameDisasters();

void SetupWreckage();

// Needed for Arklon's hax
#define SWARMIDING 0 // runununununNNN
#define numof(array) (sizeof(array)/sizeof(array[0]))
#define autosize(array) numof(array), array
extern HINSTANCE g_hInst;
