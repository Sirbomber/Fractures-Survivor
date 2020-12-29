#include <windows.h>
#include <winuser.h>
#include <vfw.h>
#include <Outpost2DLL.h>
#include <hfl.h>
#include <OP2App\CConfig.h>
#include <OP2App\ResManager.h>
#include <OP2App\TApp.h>
#include "HookHelper.h"
#include "Cutscene.h"

HINSTANCE hInstDLL = NULL;

bool PlayAvi(const char *fileName, bool isAtMissionEnd)
{
	HWND frameWnd, blackWnd, aviWnd;
	DWORD startTick;
	MSG msg;
	char pathBuf[0x104], pathBufZ[0x104];
	memset(pathBuf, 0, sizeof(pathBuf));
	memset(pathBufZ, 0, sizeof(pathBufZ));
	bool returnedValue = true;

	if (!isInited)
		return false;

	void *musicManagerObj = (void*)OP2Addr(0x565390);
	void (__thiscall *MusicManager_Pause)(void *thisPtr) = (void (__thiscall*)(void*))OP2Addr(0x450BB0);
	void (__thiscall *MusicManager_Unpause)(void *thisPtr) = (void (__thiscall*)(void*))OP2Addr(0x450C20);
	void *soundEffectsManagerObj = (void*)OP2Addr(0x56D250);
	void (__thiscall *SoundEffectsManager_Pause)(void *thisPtr) = (void (__thiscall*)(void*))OP2Addr(0x47E6A0);
	void (__thiscall *SoundEffectsManager_Unpause)(void *thisPtr) = (void (__thiscall*)(void*))OP2Addr(0x47E700);

	if (g_ResManager.GetFilePath(fileName, pathBuf) == 0)
		return false;
	g_ResManager.GetFilePath("MEBL.AVI", pathBufZ);

	MusicManager_Pause(musicManagerObj);
	SoundEffectsManager_Pause(soundEffectsManagerObj);
	gTApp.OnPauseGame(1);

	frameWnd = FindWindow("Dans_RULE_UIFrame", NULL);
	if (frameWnd == NULL)
		return false;

	blackWnd = CreateBlackWindow(frameWnd);
	aviWnd = MCIWndCreate(frameWnd, GetModuleHandle("Outpost2.exe"), WS_POPUP | WS_VISIBLE | MCIWNDF_NOPLAYBAR | MCIWNDF_NOMENU | WS_EX_OVERLAPPEDWINDOW, pathBuf);
	if (aviWnd != NULL)
	{
		startTick = GetTickCount();

		CenterWindow(aviWnd);
		MCIWndSetTimeFormat(aviWnd, "ms");
		MCIWndSetRepeat(aviWnd, false);

		MCIWndPlay(aviWnd);

		while (true)
		{
			if (fileName == "MPF.AVIZ" && pathBufZ[0] && MCIWndGetPosition(aviWnd) >= 5000)
			{
				MCIWndStop(aviWnd);
				MCIWndOpen(aviWnd, pathBufZ, NULL);
				MCIWndPlay(aviWnd);
			}
			if (MCIWndGetPosition(aviWnd) >= MCIWndGetLength(aviWnd))
				break;

			if (GetMessage(&msg, NULL, NULL, NULL) >= 0)
			{
				if (msg.message == WM_LBUTTONDOWN || msg.message == WM_KEYDOWN)
				{
					if (msg.time >= startTick)
						break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				returnedValue = false;
				break;
			}
		}
		if (isAtMissionEnd)
		{
			// Lock Dans_RULE_UIFrame to prevent the window frame from flickering before OP2Shell loads (needs to be unlocked during DLL_PROCESS_DETACH)
			LockWindowUpdate(frameWnd);
		}

		MCIWndStop(aviWnd);
		MCIWndDestroy(aviWnd);
		aviWnd = NULL;
	}
	else
		returnedValue = false;
	if (blackWnd != NULL)
		SendMessage(blackWnd, WM_DESTROY, NULL, NULL);

	gTApp.OnPauseGame(0);
	SoundEffectsManager_Unpause(soundEffectsManagerObj);
	MusicManager_Unpause(musicManagerObj);

	return returnedValue;
}


void CenterWindow(HWND hWnd, bool clipToScreen)
{
	HWND hWndParent;
	RECT rect, parentRect;
	int width, height;      
	int screenWidth, screenHeight;
	int x, y;

	hWndParent = GetParent(hWnd);

	GetWindowRect(hWnd, &rect);
	GetWindowRect(hWndParent, &parentRect);

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	x = ((parentRect.right - parentRect.left) - width) / 2 + parentRect.left;
	y = ((parentRect.bottom - parentRect.top) - height) / 2 + parentRect.top;

	if (clipToScreen)
	{
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;
		if (x + width > screenWidth)
			x = screenWidth - width;
		if (y + height > screenHeight)
			y = screenHeight - height;
	}

	MoveWindow(hWnd, x, y, width, height, FALSE);
}


HWND CreateBlackWindow(HWND hWndParent)
{
	HWND hWnd;
	WNDCLASS wc;
	RECT wndRect = {0, 0, 0, 0};

	if (hWndParent == NULL || hInstDLL == NULL)
		return NULL;

	GetWindowRect(hWndParent, &wndRect);

	if (!GetClassInfo(hInstDLL, TEXT("Blackness"), &wc))
	{
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;  
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hInstance = hInstDLL;
		wc.lpfnWndProc = BlackWndProc;
		wc.lpszClassName = TEXT("Blackness");

		wc.lpszMenuName = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW;

		RegisterClass(&wc);
	}

	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
		TEXT("Blackness"), TEXT(""),
		WS_POPUP | WS_VISIBLE,
		wndRect.left, wndRect.top, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top,
		hWndParent, NULL,
		hInstDLL, NULL);

	ShowWindow(hWnd, TRUE);
    UpdateWindow(hWnd);

	return hWnd;
}


void PaintWndRectBlack(HWND hWnd, LPRECT paintRect)
{
	HDC hDC;

	if (IsRectEmpty(paintRect))
		return;

	hDC = GetWindowDC(hWnd);
	PatBlt(hDC, paintRect->left, paintRect->top, paintRect->right - paintRect->left, paintRect->bottom - paintRect->top, BLACKNESS);
	ReleaseDC(hWnd, hDC);
	ValidateRect(hWnd, paintRect);
}


void PaintWndBlack(HWND hWnd)
{
	RECT wndRect;

	GetWindowRect(hWnd, &wndRect);
	if (IsRectEmpty(&wndRect))
		return;
	PaintWndRectBlack(hWnd, &wndRect);
}


LRESULT CALLBACK BlackWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT updateRect;

	switch (msg)
	{
	case WM_CREATE:
		PaintWndBlack(hWnd);
		break;
	case WM_PAINT:
	case WM_NCPAINT:
		GetUpdateRect(hWnd, &updateRect, false);
		PaintWndRectBlack(hWnd, &updateRect);
		break;
	case WM_DESTROY:
		DestroyWindow(hWnd);
		break;
	}

	return TRUE;
}