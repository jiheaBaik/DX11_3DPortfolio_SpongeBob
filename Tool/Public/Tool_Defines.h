#pragma once


#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <SDKDDKVer.h>

//#define WIN32_LEAN_AND_MEAN 
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

namespace Tool
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;


	enum LEVEL { LEVEL_STATIC, LEVEL_TERRAIN, LEVEL_ANIMATION, LEVEL_EFFECT, LEVEL_UI, LEVEL_MONSTER, LEVEL_END };
}


using namespace Tool;