#pragma once

#include "Process.h"

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOGO, LEVEL_MENU, LEVEL_FIELD, LEVEL_GAMEPLAY, LEVEL_LOADING, LEVEL_END };
} 

using namespace Client;
