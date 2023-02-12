#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#include "Loader.h"
#include "GameInstance.h"
#include "Level_Logo.h"
#include "Level_Menu.h"
#include "Level_Field.h"

#include "Level_GamePlay.h"


CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (int i = 0; i < 60; i++)
	{
		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_LOADING, L"Layer_BubbleUI", TEXT("Prototype_GameObject_BubbleUI"))))
			return E_FAIL;
	}


	_uint textuerNum = 1;
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_LOADING, L"Layer_Loding", TEXT("Prototype_GameObject_Loding"),&textuerNum/*, &CGameObject::GAMEOBJECTDESC(5.f, XMConvertToRadians(90.0f))*/)))
		return E_FAIL;

	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;



	pGameInstance->PlayBGM(L"SFX_FD_Amb_BubbleGoo_002.ogg", 0.1f);
	Safe_Release(pGameInstance);

	return S_OK;
}

void CLevel_Loading::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
		
}

void CLevel_Loading::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	

	
	/* 로더에게 로딩(다음레벨을 위한 자원로딩) 이 끝났다면이라는 정보를받아와서.  */

	if(true == m_pLoader->isFinished())
	{ 

		//if (GetKeyState(VK_SPACE) & 0x8000)
		//{
			CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);
			pGameInstance->StopAll();

			CLevel*		pLevel = nullptr;

			switch (m_eNextLevel)
			{
			case LEVEL_LOGO:
				pLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_MENU:
				pLevel = CLevel_Menu::Create(m_pDevice, m_pContext);
				break;
				case LEVEL_FIELD:
				pLevel = CLevel_Field ::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pLevel)
				return;

			

			if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pLevel)))
				return;

			Safe_Release(pGameInstance);

		//}
	}

	
}

HRESULT CLevel_Loading::Render()
{
	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLevel_Loading*		pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
