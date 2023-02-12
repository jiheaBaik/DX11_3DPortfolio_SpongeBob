#include "stdafx.h"
#include "..\Public\Level_Menu.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "MenuBar.h"
#include "Camera_Free.h"
#include "Menu_SpongeBob.h"


CLevel_Menu::CLevel_Menu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Menu::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	int rc;
	char *err_msg = 0;

	rc = sqlite3_open("../../Reference/DataBase/SpongeBob_jhBaik_DB.db", &db);
	if (rc)
		sqlite3_close(db);


	if (rc != SQLITE_OK)
	{
		MSG_BOX("SQL errory");
		sqlite3_free(err_msg);
		sqlite3_close(db);

		return E_FAIL;
	}


	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	
	

	

	if (FAILED(Ready_Layer_SpongeBob(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	pGameInstance->PlayBGM(L"MUS_Spongebob_Theme.ogg", 0.5f);
	Safe_Release(pGameInstance);
	return S_OK;
}
HRESULT CLevel_Menu::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	int rc;
	sqlite3_stmt *res3;

	string sqlTemp = "select * from Level_Menu_UI";
	const char* sql = sqlTemp.c_str();


	rc = sqlite3_prepare_v2(db, sql, -1, &res3, 0);

	while (sqlite3_step(res3) == SQLITE_ROW)
	{
		//		obj_uiInfo.toolLevel = (LEVEL)sqlite3_column_int(res3, 0);
		//	obj_uiInfo.clientLevel = (OBJ_CLIENTLEVELE)sqlite3_column_int(res3, 1);

		char			temp[MAX_PATH] = "";
		strcpy_s(temp, (char*)sqlite3_column_text(res3, 1));

	/*	if (strcmp(temp, "Exit") == 0)
		{
			obj_uiInfo.pLayerTag = L"Layer_Exit";
			obj_uiInfo.pPrototypeTag = L"Prototype_GameObject_Disable_Exit";

		}
		else if (strcmp(temp, "Game_Start") == 0)
		{
			obj_uiInfo.pLayerTag = L"Layer_Geme_Start";
			obj_uiInfo.pPrototypeTag = L"Prototype_GameObject_Disable_Geme_Start";

		}
		else*/
		if (strcmp(temp, "Menu_Bar") == 0)
		{
			obj_uiInfo.pLayerTag = L"Layer_Menu_Bar";
			obj_uiInfo.pPrototypeTag = L"Prototype_GameObject_MenuBar";

		}
		else
		{
			continue;
		}
		obj_uiInfo.fPos.x = (_float)sqlite3_column_double(res3, 4);
		obj_uiInfo.fPos.y = (_float)sqlite3_column_double(res3, 5);
		obj_uiInfo.pTextureInfo = sqlite3_column_int(res3, 6);

		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_MENU, (_tchar*)obj_uiInfo.pLayerTag, obj_uiInfo.pPrototypeTag, &obj_uiInfo)))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}
void CLevel_Menu::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	
	Safe_Release(pGameInstance);
}

void CLevel_Menu::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CMenuBar* pMenuBar = static_cast<CMenuBar*>(pGameInstance->Get_GameObject(LEVEL_MENU, TEXT("Layer_Menu_Bar")));
	//m_bIsClick = ;
	if (pMenuBar->Get_Click())
	{
		pGameInstance->StopAll();
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_FIELD))))
			return;

	}
	Safe_Release(pGameInstance);
	SetWindowText(g_hWnd, TEXT("로고레벨임"));
}

HRESULT CLevel_Menu::Render()
{
	return S_OK;
}

HRESULT CLevel_Menu::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	_uint textuerNum = 2;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_MENU, L"Layer_Menu", TEXT("Prototype_GameObject_Menu"), &textuerNum/*, &CGameObject::GAMEOBJECTDESC(5.f, XMConvertToRadians(90.0f))*/)))
		return E_FAIL;

	CMenu_SpongeBob::PATRICKDESC		m_Info;
	ZeroMemory(&m_Info, sizeof(CMenu_SpongeBob::PATRICKDESC));

	m_Info.pos = XMVectorSet(-1, -1.5f, 0, 1);
	m_Info.vAxit = _float3(0, 1, 0);
	m_Info.fAngle = -0.0f;
	m_Info.fScale = _float3(1, 1, 1);
//	_vector pos = XMVectorSet(-1, -1.5f, 0, 1);
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_MENU, L"Layer_Menu_SpongeBob", TEXT("Prototype_GameObject_Menu_SpongeBob"),&m_Info)))
		return E_FAIL;

	//_uint textuerNum = 2;
	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_MENU, L"Layer_Menu", TEXT("Prototype_GameObject_Menu"), &textuerNum/*, &CGameObject::GAMEOBJECTDESC(5.f, XMConvertToRadians(90.0f))*/)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Menu::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	//35.f, 0.f, 45.f, 1
	CameraDesc.vEye = _float3(0.f, 1.f, -5.0f);
	CameraDesc.vAt = _float3(0.f, 1.f, 0.f);
	CameraDesc.TransformDesc.SpeedPerSec = 15.f;
	CameraDesc.TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = _float(g_iWinSizeX) / g_iWinSizeY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 500.f;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_MENU, pLayerTag, TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
		return E_FAIL;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Menu::Ready_Layer_SpongeBob(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float3(1.f, -1.f, 1.f);
	LightDesc.vDiffuse = _float4(0.9f, 0.6f, 0.5f, 1.f);
	LightDesc.vAmbient = _float4(1,1,1, 1.f);
	LightDesc.vSpecular = _float4(0.1, 0.1, 0.1, 1);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;
	
	/*if (FAILED(pGameInstance->Clone_GameObject(LEVEL_MENU, L"Layer_Menu_SpongeBob", TEXT("Prototype_GameObject_Menu_SpongeBob"))))
		return E_FAIL;*/
	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Menu * CLevel_Menu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Menu*		pInstance = new CLevel_Menu(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Menu::Free()
{
	/*CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Light_Remove();

	Safe_Release(pGameInstance);*/

	__super::Free();
}
