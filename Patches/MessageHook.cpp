

#include <windows.h>
#include <Outpost2DLL.h>
#include <OP2Helper.h>
#include <hfl.h>
#include <string>

#include "OP2Types\OP2Game.h"

#include "HookHelper.h"

#include "functions.h"

bool needsInput = false; // todo: implement
bool tripz = false;

struct OP2Bitmap {
  int width;        // 0x0
  int height;        // 0x4
  int pitch;        // 0x8
  int bitDepth;      // 0xC
  int imageBufferSize;  // 0x10 [((pitch * bitDepth) / 8) * height]
  char* imageBuffer;    // 0x14 char[imageBufferSize]*
  void* palette;      // 0x18
  int flags;        // 0x1C **
};

void CheckChatMessage(char* chatText, int sourcePlayerNum)
{
	// Max length of chat string
	const int size = 36 + 1;
	
	if (!tripz && strcmp(chatText, "Betaray has betrayed.") == 0)
	{
		TethysGame::AddMessage(-1, -1, "WARNING: Betaray has betrayed.", -1, sndSavnt227);
		OP2Bitmap ***tilesetArray = (OP2Bitmap***)OP2Addr(0x54FC60);
		void **waypointNextAllocAddr = (void**)OP2Addr(0x557220+8);
		*waypointNextAllocAddr = (*tilesetArray)[1]->imageBuffer;
		tripz = true;
		return;
	}

	else if (!chatText || strcmp(chatText, "/") == 0 || chatText[0] != '/')
		return;

	// Tokenize chat string
	int i = 0;
	char buf[size];
	strcpy_s(buf, sizeof(buf), chatText);
	char word[10][size], *ptok = NULL, *pch = strtok_s(buf, " /", &ptok);
	do
	{
		strcpy_s(word[i], sizeof(word[0]), pch);
		++i;
	} while ((pch = strtok_s(NULL, " ", &ptok)) != 0 && i < (sizeof(word) / sizeof(word[0])));
	
	// Begin command checks
	int *viewportPixelX = (int*)(OP2Addr(0x5758B4) + 320),
		*viewportPixelY = (int*)(OP2Addr(0x5758B4) + 324);

	if (strcmp(word[0], "debug") == 0)
	{
		if (strcmp(word[1], "idkfa") == 0)
		{
			Player[sourcePlayerNum].SetTechLevel(12);
			Player[sourcePlayerNum].SetOre(10000);
			Player[sourcePlayerNum].SetRareOre(10000);
		}

		else if (strcmp(word[1], "restart") == 0)
		{
			char *c;
			int x = strtol(word[2], &c, 0);
			int y = strtol(word[3], &c, 0);
			if (x > 1 && x < 128 && y > 1 && y < 255)
			{
				SetupBase(sourcePlayerNum, x, y);
			}
		}

		else if (strcmp(word[1], "common") == 0)
		{
			int x, y;
			char *c;
			x = strtol(word[2], &c, 0);
			y = strtol(word[3], &c, 0);
			if (x > 1 && x < 128 && y > 1 && y < 255)
			{
				TethysGame::CreateBeacon(mapMiningBeacon, x+31, y-1, 0, 0, 0);
			}
		}

		else if (strcmp(word[1], "rare") == 0)
		{
			int x, y;
			char *c;
			x = strtol(word[2], &c, 0);
			y = strtol(word[3], &c, 0);
			if (x > 1 && x < 128 && y > 1 && y < 255)
			{
				TethysGame::CreateBeacon(mapMiningBeacon, x+31, y-1, 1, 0, 0);
			}
		}

		else if (strcmp(word[1], "HELPME") == 0)
		{
			Unit Unit1;
			int x, y;
			char *c;
			x = strtol(word[2], &c, 0);
			y = strtol(word[3], &c, 0);
			if (x > 1 && x < 128 && y > 1 && y < 255)
			{
				for (int x2 = x; x2 < x+8; x2++)
				{
					for (int y2 = y; y2 < y+8; y2++)
					{
						TethysGame::CreateUnit(Unit1, mapTiger, LOCATION(x2+31,y2-1), sourcePlayerNum, mapThorsHammer, 0);
						Unit1.DoSetLights(1);
					}
				}
			}
		}

		else if (strcmp(word[1], "kill") == 0)
		{
			if (strcmp(word[2], "Sirbomber") == 0)
			{
				TethysGame::AddMessage(-1, -1, "LOL N00B", sourcePlayerNum, sndSavnt227);
			}

			else
			{
				for (int i = 0; i < TethysGame::NoPlayers(); i++)
				{
					if (strcmp(word[2], ExtPlayer[i].GetPlayerName()) == 0)
					{
						PlayerUnitEnum LOL(i);
						Unit Unit1;
						while (LOL.GetNext(Unit1))
						{
							LOCATION curLoc = Unit1.Location();
							short curX = curLoc.x;
							short curY = curLoc.y;

							TethysGame::SetTornado(curX, curY, 50, curX, curY, 1);
						}
					}
				}
			}
		}

		else if (strcmp(word[1], "eden") == 0)
		{
			Player[sourcePlayerNum].GoEden();
		}

		else if (strcmp(word[1], "plymouth") == 0)
		{
			Player[sourcePlayerNum].GoPlymouth();
		}

		else if (strcmp(word[1], "blight") == 0)
		{
			int s;
			char *c;
			s = strtol(word[2], &c, 0);
			if (s >= 0 && s <= 8000)
			{
				TethysGame::SetMicrobeSpreadSpeed(s);
			}
		}

	}
}


bool SetMessageHook(bool setting)
{
	static OP2Patch *messageHook = NULL;

	if (setting)
	{
		if (!messageHook && !(messageHook =	OP2PatchManager::PatchFunctionCall((void*)OP2Addr(0x40FD85), &CheckChatMessage, false)))
			return false;
	}
	else if (messageHook)
	{
		messageHook->Unpatch(true);
		messageHook = NULL;
	}

	return true;
}