#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
// #include "Camera_Free.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Logo::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	pGameInstance->PlayBGM(L"MUS_WavesSeagullsBg.ogg", 0.1f);
	Safe_Release(pGameInstance);

	return S_OK;
}

void CLevel_Logo::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
		
}

void CLevel_Logo::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	
	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);	
		pGameInstance->StopAll();
		Safe_Release(pGameInstance);
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_MENU))))
			return ;

		

	}

	SetWindowText(g_hWnd, TEXT("로고레벨임"));
}

HRESULT CLevel_Logo::Render()
{
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_LOGO, L"Layer_Logo", TEXT("Prototype_GameObject_Logo")/*, &CGameObject::GAMEOBJECTDESC(5.f, XMConvertToRadians(90.0f))*/)))
		return E_FAIL;

	Safe_Release(pGameInstance);	
	
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//CCamera::CAMERADESC			CameraDesc;
	//ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	//CameraDesc.vEye = _float3(0.f, 2.f, -2.0f);
	//CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	//CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	//CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	//CameraDesc.fFovy = D3DXToRadian(60.0f);
	//CameraDesc.fAspect = _float(g_iWinSizeX) / g_iWinSizeY;
	//CameraDesc.fNear = 0.2f;
	//CameraDesc.fFar = 300.f;

	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);	

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Monster(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_Monster"))))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Logo*		pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Logo::Free()
{
	

	

	__super::Free();
}
