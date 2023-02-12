#include "stdafx.h"
#include "..\Public\Level_Field.h"
#include "GameInstance.h"

#include "Camera_Free.h"
#include "SpongeBob_Foot_L.h"
#include "SpongeBob_Wand.h"
#include "Level_Loading.h"
#include "Player.h"
#include "Gate.h"
#include "Taxi.h"

#include "Menu_SpongeBob.h"

CLevel_Field::CLevel_Field(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Field::NativeConstruct()
{
	//CGameInstance::Get_Instance()->PlayMySound(L"Intro.ogg", CSoundMgr::EFFECT, 0.3f);


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
	

	if (FAILED(Ready_LightDesc()))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;	


	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER);
	//CGameInstance::Get_Instance()->PlayMySound(L"IntroStart2.mp3", CSoundMgr::PLAYER, 0.3f);
	//pGameInstance->PlayBGM(L"Intro_Jelly.mp3", 0.1f);
	pGameInstance->PlayBGM(L"MUS_JellyfishFieldsTheme.ogg", 0.1f);

	
	Safe_Release(pGameInstance);
	return S_OK;
}

void CLevel_Field::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
		
}

void CLevel_Field::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CTaxi* pTaxi = static_cast<CTaxi*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Taxi")));
	if (pTaxi->Get_Count() > 120)
	{
		pGameInstance->StopAll();
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;
	}


	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		pGameInstance->StopAll();

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;


	}
	Safe_Release(pGameInstance);



	SetWindowText(g_hWnd, TEXT("게임플레이레벨임"));
}

HRESULT CLevel_Field::Render()
{
	return S_OK;
}

HRESULT CLevel_Field::Ready_LightDesc()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Light_Remove();
	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float3(1.f, -1.f, 1.f);
	LightDesc.vDiffuse = _float4(1.f, 0.7f, 0.7f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 0.85f, 0.7f, 1.f);
//	LightDesc.vSpecular = _float4(0,0,0,1);
	LightDesc.vSpecular = _float4(1, 0.87, 0.73, 1);
	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

//
//	//BSSE
//	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	LightDesc.vPosition = _float3(20.8441982269287, 50, 27.6317520141602);
//	LightDesc.fRange = 800;
//	LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	LightDesc.vAmbient = _float4(1, 0.87, 0.73, 1);
////	LightDesc.vSpecular = _float4(0.3, 0.3, 0.3, 1);
//	LightDesc.vSpecular = _float4(0, 0, 0, 1);
//	LightDesc.isCulling = false;
//	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//		return E_FAIL;
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Field::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/*if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_King_Neptune"), TEXT("Prototype_GameObject_King_Neptune"))))
		return E_FAIL;*/
	_uint level = 0;
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Player"),&level)))
		return E_FAIL;
	CPlayer* pplayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_Player")));
	pplayer->Set_Navigation();
	/*if(FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_PatrickPlayer"), TEXT("Prototype_GameObject_Patrick_Player"))))
		return E_FAIL;*/

	CSpongeBob_Foot_L::FOOTDESC		FootDesc;
	ZeroMemory(&FootDesc, sizeof(CSpongeBob_Foot_L::FOOTDESC));

	FootDesc.eLevel = LEVEL_STATIC;
	FootDesc.pLayerTag = pLayerTag;
	FootDesc.iIndex = 0;
	FootDesc.pModelComTag = TEXT("Com_VIBuffer");
	FootDesc.pBoneName = "jt_bubbleFoot_L";

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_STATIC, TEXT("Layer_SpongeBob_Foot_L"), TEXT("Prototype_GameObject_SpongeBob_Foot_L"), &FootDesc)))
		return E_FAIL;

	CSpongeBob_Wand::WANDDESC		WandDesc;
	ZeroMemory(&WandDesc, sizeof(CSpongeBob_Wand::WANDDESC));

	WandDesc.eLevel = LEVEL_STATIC;
	WandDesc.pLayerTag = pLayerTag;
	WandDesc.iIndex = 0;
	WandDesc.pModelComTag = TEXT("Com_VIBuffer");
	WandDesc.pBoneName = "IndexFinger1_R";

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_STATIC, TEXT("Layer_SpongeBob_Wand"), TEXT("Prototype_GameObject_SpongeBob_Wand"), &WandDesc)))
		return E_FAIL;


	

	

	
	Safe_Release(pGameInstance);

	return S_OK;
}



HRESULT CLevel_Field::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/*if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;*/

	/*if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Ring_Top"))))
		return E_FAIL;*/



	int rc;
	sqlite3_stmt *res3;

	string sqlTemp = "select * from Level_Boss2_Map";
	const char* sql = sqlTemp.c_str();


	rc = sqlite3_prepare_v2(db, sql, -1, &res3, 0);

	while (sqlite3_step(res3) == SQLITE_ROW)
	{
//		objInfo.toolLevel = (LEVEL)sqlite3_column_int(res3, 0);
		//objInfo.clientLevel = (OBJ_CLIENTLEVELE)sqlite3_column_int(res3, 1);

		char			temp[MAX_PATH] = "";
		strcpy_s(temp, (char*)sqlite3_column_text(res3, 2));
		
if (strcmp(temp, "Mesh_field") == 0)
		{
			objInfo.pLayerTag = L"Layer_Mesh_field";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Field";
		}
		
		else if (strcmp(temp, "Mesh_TaxiStop") == 0)
		{
			objInfo.pLayerTag = L"Layer_TaxiStop";
			objInfo.pPrototypeTag = L"Prototype_GameObject_TaxiStop";
		}
		else if (strcmp(temp, "Mesh_Gate") == 0)
		{
			objInfo.pLayerTag = L"Layer_Gate";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Gate";
		}
		else if (strcmp(temp, "Bubble") == 0)
		{
			objInfo.pLayerTag = L"Layer_Bubble";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Bubble";
		}
		else if (strcmp(temp, "Mesh_Jelly_Pink") == 0)
		{
			objInfo.pLayerTag = L"Layer_Jelly_Pink";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Jelly_Pink";
		}
		else if (strcmp(temp, "Mesh_Jelly_Blue") == 0)
		{
			objInfo.pLayerTag = L"Layer_Jelly_Blue";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Jelly_Blue";
		}
		else if (strcmp(temp, "Mesh_Robo") == 0)
		{
			objInfo.pLayerTag = L"Layer_Robo";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Robo";
		}
		else if (strcmp(temp, "Mesh_Robo2") == 0)
		{
			objInfo.pLayerTag = L"Layer_Robo2";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Robo2";
		}
		else if (strcmp(temp, "Mesh_Robo3") == 0)
		{
			objInfo.pLayerTag = L"Layer_Robo3";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Robo3";
		}
		else if (strcmp(temp, "Mesh_Squidward") == 0)
		{
			objInfo.pLayerTag = L"Layer_Squidward";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Squidward";
		}
		else if (strcmp(temp, "Mesh_Man") == 0)
		{
			objInfo.pLayerTag = L"Layer_Man";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Man";
		}
		else if (strcmp(temp, "Mesh_Tree") == 0)
		{
			objInfo.pLayerTag = L"Layer_Tree";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Tree";
		}
		else
		{
			continue;
		}
		objInfo.iIndex = sqlite3_column_int(res3, 5);
		objInfo.iNumber = sqlite3_column_int(res3, 6);
		objInfo.fPos.x = (_float)sqlite3_column_double(res3, 7);
		objInfo.fPos.y = (_float)sqlite3_column_double(res3, 8);
		objInfo.fPos.z = (_float)sqlite3_column_double(res3, 9);
		objInfo.fScale.x = (_float)sqlite3_column_double(res3, 10);
		objInfo.fScale.y = (_float)sqlite3_column_double(res3, 11);
		objInfo.fScale.z = (_float)sqlite3_column_double(res3, 12);
		objInfo.fAxist.x = (_float)sqlite3_column_int(res3, 21);
		objInfo.fAxist.y = (_float)sqlite3_column_int(res3, 22);
		objInfo.fAxist.z = (_float)sqlite3_column_int(res3, 23);
		objInfo.fRotation = (_float)sqlite3_column_double(res3, 24);

		char * temp1 = (char*)sqlite3_column_text(res3, 4);
		MultiByteToWideChar(CP_ACP, 0, temp1, strlen(temp1) + 1, pTextureInfo, 256);
		objInfo.pTextureInfo = pTextureInfo;
	
		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, (_tchar*)objInfo.pLayerTag, objInfo.pPrototypeTag, &objInfo)))
			return E_FAIL;
		

	}
	_vector pos;
	pos = XMVectorSet(20.8441982269287, -7, 27.6317520141602, 1);
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_JellyGround"), TEXT("Prototype_GameObject_JellyGround"), &pos)))
		return E_FAIL;

	//_float3 Info = _float3(200,150,200);
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_Effect_EndPang"), TEXT("Prototype_GameObject_EndPangEffect"))))
		return E_FAIL; 

	

	
	pos = XMVectorSet(70.4577026367187, 13., -5.59131622314453, 1);
		//pos = XMVectorSet(71.6623764038086, 13., -4.18260955810547, 1);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_Taxi"), TEXT("Prototype_GameObject_Taxi"), &pos)))
		return E_FAIL;
	

		pos = XMVectorSet(54.2292633056641, 13., -8.47719955444336, 1);
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_TaxiStop"), TEXT("Prototype_GameObject_TaxiStop"), &pos)))
		return E_FAIL;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_Sky"), TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;
	

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_FowerEffect"), TEXT("Prototype_GameObject_FlowerEffect"))))
		return E_FAIL;
	
		
		CMenu_SpongeBob::PATRICKDESC		m_Info;
		ZeroMemory(&m_Info, sizeof(CMenu_SpongeBob::PATRICKDESC));

		m_Info.pos = XMVectorSet(49.9300003051758, 13.f, -11, 1);
		m_Info.vAxit = _float3(0, 1, 0);
		m_Info.fAngle = -3.2f;
		m_Info.fScale = _float3(1.5, 1.5, 1.5);
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, L"Layer_Menu_SpongeBob", TEXT("Prototype_GameObject_Menu_SpongeBob"), &m_Info)))
		return E_FAIL;




	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_STATIC, TEXT("Layer_RainBow"), TEXT("Prototype_GameObject_RainBow"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_Info"), TEXT("Prototype_GameObject_Info"))))
		return E_FAIL;

	

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Field::Ready_Layer_Effect(const _tchar * pLayerTag)
{
//	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
//	Safe_AddRef(pGameInstance);	
//
//	/*for (_uint i = 0; i < 30; ++i)
//	{
//		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Effect"))))
//			return E_FAIL;
//	}
//*/
//
//
//	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Field::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	int rc;
	sqlite3_stmt *res3;

	string sqlTemp = "select * from Level_Boss1_UI";
	const char* sql = sqlTemp.c_str();


	rc = sqlite3_prepare_v2(db, sql, -1, &res3, 0);

	while (sqlite3_step(res3) == SQLITE_ROW)
	{
		//		obj_uiInfo.toolLevel = (LEVEL)sqlite3_column_int(res3, 0);
		//	obj_uiInfo.clientLevel = (OBJ_CLIENTLEVELE)sqlite3_column_int(res3, 1);

		char			temp[MAX_PATH] = "";
		strcpy_s(temp, (char*)sqlite3_column_text(res3, 1));

		obj_uiInfo.fPos.x = (_float)sqlite3_column_double(res3, 4);
		obj_uiInfo.fPos.y = (_float)sqlite3_column_double(res3, 5);
		obj_uiInfo.pTextureInfo = sqlite3_column_int(res3, 6);

		if (strcmp(temp, "Underpants") == 0)
		{
			obj_uiInfo.pLayerTag = L"Layer_Underpants";
			obj_uiInfo.pPrototypeTag = L"Prototype_GameObject_Underpants";

		}
		else
		{
			continue;
		}
		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, (_tchar*)obj_uiInfo.pLayerTag, obj_uiInfo.pPrototypeTag, &obj_uiInfo)))
			return E_FAIL;
	}

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_FlowerUI"), TEXT("Prototype_GameObject_Flower_UI"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_JellyCut"), TEXT("Prototype_GameObject_JellyCut"))))
		return E_FAIL;
	

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Field::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	//35.f, 0.f, 45.f, 1
	CameraDesc.vEye = _float3(35.f, 13.f, 25.f);
	CameraDesc.vAt = _float3(35.f, 0.f, 45.f);
	CameraDesc.TransformDesc.SpeedPerSec = 5.f;
	CameraDesc.TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = _float(g_iWinSizeX) / g_iWinSizeY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 500.f;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
		return E_FAIL;

	CCamera_Free* pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_FIELD, pLayerTag));
	pCamera->Set_IsGamePlay(true);
	pCamera->Set_m_bIsDirector_End(false);
	pCamera->Set_Level(0);
	
	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Field * CLevel_Field::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Field*		pInstance = new CLevel_Field(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Field::Free()
{

//	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
//	Safe_AddRef(pGameInstance);
//
//	/*CCamera_Free* pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Camera")));
//	pCamera->Set_IsGamePlay(false);
//*/
//	Safe_Release(pGameInstance);


	__super::Free();
	sqlite3_close(db);
}
