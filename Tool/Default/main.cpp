// Dear ImGui: standalone example application for DirectX 9
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs


#include <d3d11.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <d3dcompiler.h>
#include <Effects11/d3dx11effect.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>

//#include <tchar.h>

#include"MainTool.h"
#include"GameInstance.h"
// Data
HWND		g_hWnd;
HINSTANCE	g_hInst;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR		IpCmdLine,
	_In_ int		nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

    // Create application window
    ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    
	// Creat Window
	RECT rcWindow = { 0,0,Tool::g_iWinSizeX,Tool::g_iWinSizeY };
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = ::CreateWindow(wc.lpszClassName, _T("SpongeBob Tool"), WS_OVERLAPPEDWINDOW, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, NULL, NULL, wc.hInstance, NULL);
	g_hInst = hInstance;
	// Init MainTool
	CMainTool* pMainTool = CMainTool::Create();
	if (nullptr == pMainTool) {
		Safe_Release(pMainTool);
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
	}

	// Create Timer
	if (FAILED(CGameInstance::Get_Instance()->Add_Timer(L"Timer_Default")))
		return E_FAIL;
	if (FAILED(CGameInstance::Get_Instance()->Add_Timer(L"Timer_60")))
		return E_FAIL;

	_float fTimerAcc = 0.f;

    // Show the window
    ::ShowWindow(g_hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(g_hWnd);

 
    // Main loop
    bool done = false;
	while (!done)
	{
		// Poll and handle messages (inputs, window resize, etc.)
		// See the WndProc() function below for our to dispatch events to the Win32 backend.
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				done = true;
		}
		if (done)
			break;

		CGameInstance::Get_Instance()->Update_Timer(L"Timer_Default");
		fTimerAcc += CGameInstance::Get_Instance()->Get_TimeDelta(L"Timer_Default");

		if (fTimerAcc >= 1.f / 60.f) {
			CGameInstance::Get_Instance()->Update_Timer(L"Timer_60");
			fTimerAcc = 0.f;
			pMainTool->Tick(CGameInstance::Get_Instance()->Get_TimeDelta(L"Timer_60"));
			if (FAILED(pMainTool->Render()))
				return E_FAIL;
		}
	}
	Safe_Release(pMainTool);
    ::DestroyWindow(g_hWnd );
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions


#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
       //if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
       //{
       //    g_d3dpp.BackBufferWidth = LOWORD(lParam);
       //    g_d3dpp.BackBufferHeight = HIWORD(lParam);
       //    ResetDevice();
       //}
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
	case WM_DPICHANGED:
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports) {
			//const int dpi = HIWORD(wParam);
			//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
			const RECT* suggested_rect = (RECT*)lParam;
			::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
