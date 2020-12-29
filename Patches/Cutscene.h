#ifndef CUTSCENE_H
#define CUTSCENE_H

#include <windows.h>

bool PlayAvi(const char *fileName, bool isAtMissionEnd = false);

void CenterWindow(HWND hWnd, bool clipToScreen = false);
void PaintWndRectBlack(HWND hWnd, LPRECT paintRect);
HWND CreateBlackWindow(HWND hWndParent);

LRESULT CALLBACK BlackWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInstDLL; // Need to set this in DllMain

#endif