# FracturedAlliance
 Mission/AI code for the Outpost 2 custom multiplayer mission, Fractured Alliance.  Can you and your allies stay one step ahead of the Blight and escape the planet while fighting off your common enemy?

-------------------------------------------------------------------------------------------
What's new in version 2
-------------------------------------------------------------------------------------------

General:
-New objectives have been added at the start of the game to explain the mission's premise and describe some of the mission's special rules and behaviors.  This should make the mission more accessible to newcomers.
-The two southern AI outposts will now always spawn, regardless of the Day/Night and Disasters settings.  These settings currently have no effect on gameplay.  This may change in the future.
-The Robot Command Center can no longer be built, since its effect is permanently enabled.
-Leisure Studies can no longer be researched, since Recreation Facilities and Forums are harmful.
-Ore is no longer randomly generated.  Instead, the map has been divided into 3 zones, and one of several pre-determined sets is randomly selected for each zone.  This change was made to ensure balanced ore placement in strategic locations.

Difficulty/Balance changes:
-Easy difficulty (high resources setting) has been made substantially more newcomer-friendly.  Players now start with more food, metal, and colonists.  All Basic Lab research has already been completed, and players start with a Standard Lab kit instead of a Basic Lab.  Players also start the game with Nursery, University, and Vehicle Factory kits.  Most importantly of all, the requirement to have Evacuation Transports and food loaded into Cargo Trucks before relocating is lifted when playing on Easy.
-Normal difficulty start (medium resources setting) is now equivalent to the v1.0 Easy difficulty.
-Hard difficulty start (low resources setting) is now equivalent to the v1.0 Normal difficulty.
-AI attack strength is now better tuned to player count, and takes player difficulty into consideration.  This should give players the same relative challenge no matter how many allies they have, instead of making a 2-player game way too hard and a 5-player game way too easy.
-AI research speed reduced.
-AI will now switch to more advanced weapons more slowly (for players on easy, at least).
-AI attack waves now progressively grow stronger, instead of sometimes getting weaker for no adequately explained reason.

Technical:
-Rewrote some mission code to use TethysAPI.  A future update may see a complete conversion to TethysAPI.
-Patch manager upgraded from HookHelper to Patcher3.
-Disasters are now handled by the Disaster Creator engine.  This slightly impacts difficulty as disasters will now specifically target human players instead of being completely random.
-First public release of the mission's source code.
-Removed the "doomsday" easter egg.
