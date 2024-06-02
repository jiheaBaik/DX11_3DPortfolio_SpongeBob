#include "..\Public\Imgui_Manager.h"

#include "GameInstance.h"
#include "Level_Terrain.h"
#include "Level_UI.h"
#include "Level_Animation.h"


IMPLEMENT_SINGLETON(CImgui_Manager)

CImgui_Manager::CImgui_Manager()
{
}

HRESULT CImgui_Manager::NativeConstruct(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.f;
		style.Colors[ImGuiCol_WindowBg].w = 1.f;
	}
	
	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	return S_OK;
}

void CImgui_Manager::Frame()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
}

HRESULT CImgui_Manager::Render()
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();

	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	pGameInstance->Render_Engine();

	Render_Menu();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

void CImgui_Manager::Render_Menu()
{
	LEVEL		eLevel;
	CLevel*		pLevel = nullptr;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Scene"))
		{
			for (_uint iLevel = 0; iLevel < LEVEL_END; iLevel++)
			{
				switch (iLevel)
				{
				case LEVEL_TERRAIN:
					if (ImGui::MenuItem("Terrian Tool"))
					{
						eLevel = LEVEL_TERRAIN;
						if (m_eCurrentLevel != eLevel)
							pLevel = CLevel_Terrain::Create(m_pDevice, m_pContext);
					}
					break;

				case LEVEL_ANIMATION:
					if (ImGui::MenuItem("Animation Tool"))
					{
						eLevel = LEVEL_ANIMATION;
						if (m_eCurrentLevel != eLevel)
							pLevel = CLevel_Animation::Create(m_pDevice, m_pContext);

					}
					break;
					case LEVEL_EFFECT:
					if (ImGui::MenuItem("Effect Tool"))
					{
						eLevel = LEVEL_EFFECT;

					}
					break;
					case LEVEL_UI:
						if (ImGui::MenuItem("UI Tool"))
						{
							eLevel = LEVEL_UI;
							if (m_eCurrentLevel != eLevel)
								pLevel = CLevel_UI::Create(m_pDevice, m_pContext);

						}
						break;

				default:
					break;
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	if (nullptr == pLevel)
		return;

	m_eCurrentLevel = eLevel;
	m_pCurrentLevel = pLevel;
	if (FAILED(CGameInstance::Get_Instance()->Open_Level(eLevel, pLevel)))
	{
		MSG_BOX("Open Level Failed");
		return;
	}
}

void CImgui_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
