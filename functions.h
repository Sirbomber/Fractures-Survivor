// Survivor Functions File
// Part of an effort to make creating new Survivor maps easier.

#pragma once

// These functions all deal with game initialization
void DoSurvSetup();     // Handles morale, day/night
void DoRandomBases();   // Randomizes player starting locations
void SetupDisasters();  // Setups all disasters, excluding Blight
void SetupBlight();     // Set up us the Blight
void SetupInitRes();    // Setup starting resources for all players
void SetupVictory();    // Creates victory (and defeat) conditions
void SetAllLava();      // Sets all lava rock to lava-possible.

// These functions handle base creation
void SetupBase(int i, int x, int y);

void SetupMines();      // Setups mining beacon, fumaroles, and magma vents
void SetupWreckage();

// Needed for Arklon's hax
#define numof(array) (sizeof(array)/sizeof(array[0]))
