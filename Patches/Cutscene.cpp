
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winuser.h>
#include <vfw.h>

#include "Tethys/Resource/CConfig.h"
#include "Tethys/Resource/ResManager.h"
#include "Tethys/Resource/SoundManager.h"
#include "Tethys/Game/TApp.h"

#include "Patcher.h"
#include "Util.h"
#include "..\functions.h"

using namespace Tethys;

HWND CreateBlackWindow(HWND hWndParent);

// =====================================================================================================================
static void PaintWndRectBlack(
  HWND    hWnd,
  LPRECT  paintRect)
{
  if (IsRectEmpty(paintRect) == false) {
    const HDC hDC = GetWindowDC(hWnd);
    PatBlt(hDC,
           paintRect->left,
           paintRect->top,
           (paintRect->right - paintRect->left),
           (paintRect->bottom - paintRect->top),
           BLACKNESS);
    ReleaseDC(hWnd, hDC);
    ValidateRect(hWnd, paintRect);
  }
}

// =====================================================================================================================
static void PaintWndBlack(
  HWND  hWnd)
{
  RECT wndRect;
  GetWindowRect(hWnd, &wndRect);

  if (IsRectEmpty(&wndRect) == false) {
    PaintWndRectBlack(hWnd, &wndRect);
  }
}

// =====================================================================================================================
static LRESULT CALLBACK BlackWndProc(
  HWND    hWnd,
  UINT    msg,
  WPARAM  wParam,
  LPARAM  lParam)
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

  default:
    break;
  }

  return TRUE;
}

// =====================================================================================================================
static HWND CreateBlackWindow(
  HWND hWndParent)
{
  if ((hWndParent == nullptr) || (g_hInst == nullptr)) {
    return nullptr;
  }

  RECT wndRect;
  GetWindowRect(hWndParent, &wndRect);

  WNDCLASS wc = { };
  if (GetClassInfo(static_cast<HINSTANCE>(g_hInst), TEXT("Blackness"), &wc) == FALSE) {
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hInstance     = static_cast<HINSTANCE>(g_hInst);
    wc.lpfnWndProc   = BlackWndProc;
    wc.lpszClassName = TEXT("Blackness");
    wc.lpszMenuName  = nullptr;
    wc.style         = (CS_HREDRAW | CS_VREDRAW);
    RegisterClass(&wc);
  }

  const HWND hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                                   TEXT("Blackness"),
                                   TEXT(""),
                                   (WS_POPUP | WS_VISIBLE),
                                   wndRect.left,
                                   wndRect.top,
                                   (wndRect.right  - wndRect.left),
                                   (wndRect.bottom - wndRect.top),
                                   hWndParent,
                                   nullptr,
                                   static_cast<HINSTANCE>(g_hInst),
                                   nullptr);

  ShowWindow(hWnd, TRUE);
  UpdateWindow(hWnd);

  return hWnd;
}

// =====================================================================================================================
static void CenterWindow(
  HWND  hWnd)
{
  const HWND hWndParent = GetParent(hWnd);

  RECT rect;
  RECT parentRect;
  GetWindowRect(hWnd, &rect);
  GetWindowRect(hWndParent, &parentRect);

  const int width  = rect.right  - rect.left;
  const int height = rect.bottom - rect.top;

  int x = (((parentRect.right  - parentRect.left) - width)  / 2) + parentRect.left;
  int y = (((parentRect.bottom - parentRect.top)  - height) / 2) + parentRect.top;

  MoveWindow(hWnd, x, y, width, height, FALSE);
}

// =====================================================================================================================
bool PlayAvi(
  const char*  pFileName,
  bool         isAtMissionEnd)
{
  bool returnedValue = true;

  char pathBuf[512];
  if (g_resManager.GetFilePath(pFileName, pathBuf) == 0) {
    return false;
  }

  g_musicManager.Pause();
  g_soundManager.Pause();
  g_tApp.OnPauseGame(1);

  const HWND hFrameWnd = FindWindow("Dans_RULE_UIFrame", nullptr);
  if (hFrameWnd == nullptr) {
    return false;
  }

  // ** TODO Set volume based on ini setting
  const HWND hBlackWnd = CreateBlackWindow(hFrameWnd);
  static constexpr DWORD WndFlags = WS_POPUP | WS_VISIBLE | MCIWNDF_NOPLAYBAR | MCIWNDF_NOMENU | WS_EX_OVERLAPPEDWINDOW;
  const  HWND hAviWnd = MCIWndCreate(hFrameWnd, GetModuleHandle("Outpost2.exe"), WndFlags, pathBuf);

  if (hAviWnd != nullptr) {
    const DWORD startTick = GetTickCount();

    CenterWindow(hAviWnd);
    MCIWndSetTimeFormat(hAviWnd, "ms");
    MCIWndSetRepeat(hAviWnd, false);

    MCIWndPlay(hAviWnd);

    while (true) {
      if (MCIWndGetPosition(hAviWnd) >= MCIWndGetLength(hAviWnd)) {
        break;
      }

      MSG msg;
      if (GetMessage(&msg, nullptr, 0, 0) >= 0) {
        if ((msg.message == WM_LBUTTONDOWN) || (msg.message == WM_KEYDOWN))  {
          if (msg.time >= startTick) {
            break;
          }
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      else {
        returnedValue = false;
        break;
      }
    }
    if (isAtMissionEnd) {
      // Lock Dans_RULE_UIFrame to prevent the window frame from flickering before OP2Shell loads
      SendMessage(hFrameWnd, WM_SETREDRAW, 0, 0);
    }

    MCIWndStop(hAviWnd);
    MCIWndDestroy(hAviWnd);
  }
  else {
    returnedValue = false;
  }

  if (hBlackWnd != nullptr) {
    SendMessage(hBlackWnd, WM_DESTROY, 0, 0);
  }

  g_tApp.OnPauseGame(0);
  g_musicManager.Unpause();
  g_soundManager.Unpause();

  return returnedValue;
}
