#include "..\Public\Level_Terrain.h"

#include "GameInstance.h"
//
//#include "BackGround.h"
//#include "Terrain.h"
#include "Camera_Free.h"
//#include "Cube.h"
//#include "SpongeBob.h"
//#include "Ground.h"
//#include "Ground2.h"
//#include "Ring_Top.h"
//#include "Rope_Top.h"
//#include "Ring_Pillar.h"
//#include "Light_Tube.h"
//#include "Scoreboard.h"
//#include "Big_Pillar.h"
//#include "Big_Pillar_Attachment.h"
//#include "Big_Cloth.h"
//#include "Pos_Light.h"
//#include "Stairs_02.h"
//#include "Stairs_04.h"
//#include "SM_PD_Banner_01.h"
//#include "Poster.h"
//#include "Poster2.h"
//#include "SM_Light_Cone_02.h"
//#include "Light_Tube.h"
//#include "Fish_A.h"
//#include "Fish_B.h"
//#include "Fish_C.h"
//#include "Fish_D.h"
//#include "Fish_E.h"
//#include "Fish_G.h"
//#include "Fish_H.h"
//#include "Fish_I.h"
//#include "Fish_J.h"
//#include "Fish_K.h"
//#include "Fish_L.h"
//#include "Scoreboard_Ropes.h"






CLevel_Terrain::CLevel_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext), m_pDevice(pDevice), m_pContext(pContext)
{
	/*Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);*/

	m_sTable = "OBJ_Info";

}

HRESULT CLevel_Terrain::NativeConstruct()
{
	DBConnection();

	m_pTerrainMgrInstance = CTerrainManager::Create(m_pDevice, m_pContext, db);

	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;
	//m_pTerrainMgrInstance->NativeConstruct();
	m_pTerrainMgrInstance->Get_TerrainComponent();
	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Clone_GameObject()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Terrain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CLevel_Terrain::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	
}

HRESULT CLevel_Terrain::Render()
{
	if (ImGui::Begin("Data Management"))
	{
		if (m_IsConnection)
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "DB Connection : Completed");
		else
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "DB Connection : Failed");

	}
	ImGui::End();

	m_pTerrainMgrInstance->Render();

	if (ImGui::Begin("Level_Terrain MousePos"))
	{
		ImVec2  screenMousePos = ImGui::GetMousePosOnOpeningCurrentPopup();
		ImVec2 mainViewPos = ImGui::GetMainViewport()->Pos;
		ImGui::Text("MainViewMousePos_x : %1.f", screenMousePos.x - mainViewPos.x);
		ImGui::Text("MainViewMousePos_y : %1.f", screenMousePos.y - mainViewPos.y - 24);
	}
	ImGui::End();
	
	/*static _bool m_bisFirst = true;
	if (m_bisFirst)
	{
		static _int m_iXAxis = 100;
		static _int m_iZAxis = 100;

		if (ImGui::Begin("Initial Basic Terrain Setting"))
		{
			ImGui::InputInt("XAxis", &m_iXAxis);
			ImGui::InputInt("ZAxis", &m_iZAxis);
			if (ImGui::Button("Create", ImVec2(250, 30)))
			{
				CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
				if (nullptr == pGameInstance)
					return E_FAIL;
				Safe_AddRef(pGameInstance);

				if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
					CVIBuffer_Terrain::Create(m_pDevice, m_pContext, m_iXAxis, m_iZAxis))))
					return E_FAIL;
				if (FAILED(pGameInstance->Clone_GameObject(LEVEL_TERRAIN, TEXT("Layer_Terrain"), TEXT("Prototype_GameObject_Terrain"))))
					return E_FAIL;
				m_pTerrainMgrInstance->Get_TerrainComponent();
				m_bisFirst = false;
				Safe_Release(pGameInstance);
			}
		}
		ImGui::End();
	}*/
	return S_OK;
}


// Terrian 레벨의 컴포넌트 프로토타입
HRESULT CLevel_Terrain::Ready_Prototype_Component()
{
	

	return S_OK;
}


// Terrian 레벨의 게임오브젝트 사본
HRESULT CLevel_Terrain::Ready_Clone_GameObject()
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);



	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_TERRAIN, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_BackGround")/*, &CGameObject::GAMEOBJECTDESC(5.f, XMConvertToRadians(90.0f))*/)))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_TERRAIN, TEXT("Layer_Terrain"), TEXT("Prototype_GameObject_Terrain")/*, &CGameObject::GAMEOBJECTDESC(5.f, XMConvertToRadians(90.0f))*/)))
	//	return E_FAIL;

	/* Clone_GameObject_Camera_Free */
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 20.f, -20.0f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.TransformDesc.SpeedPerSec = 15.f;
	CameraDesc.TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = _float(g_iWinSizeX) / g_iWinSizeY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 500.f;
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_TERRAIN, TEXT("Layer_Camera"), TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
		return E_FAIL;
	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float3(1.f, -1.f, 1.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
		if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
			return E_FAIL;
//	//base
//	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	LightDesc.vPosition = _float3(47, 18, 46);
//	LightDesc.fRange = 75;
//	LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	LightDesc.vAmbient = _float4(1, 0.87, 0.73, 1);
//	LightDesc.vSpecular = _float4(1, 0.87, 0.73, 1);
//
//	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//		return E_FAIL;
//
//
//
//	//보
//	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	LightDesc.vPosition = _float3(56, 0, 77);
//	LightDesc.fRange = 15;
//	LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	LightDesc.vAmbient = _float4(0, 0, 1, 1);
//	LightDesc.vSpecular = _float4(0, 0, 1, 1);
//
//	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//		return E_FAIL;
//
//	////보
//	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	//LightDesc.vPosition = _float3(17.2, 0, 54.3);
//	//LightDesc.fRange = 15;
//	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	//LightDesc.vAmbient = _float4(1, 0.627, 0.015, 1);
//	//LightDesc.vSpecular = _float4(1, 0.627, 0.015, 1);
//
//	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//	//	return E_FAIL;
//
//
//
//
//	//파
//	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	LightDesc.vPosition = _float3(75, 0, 35.29);
//	LightDesc.fRange = 15;
//	LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	LightDesc.vAmbient = _float4(1, 0.627, 0.015, 1);
//	LightDesc.vSpecular = _float4(1, 0.627, 0.015, 1);
//
//	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//		return E_FAIL;
//
//
//	////보
//	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	//LightDesc.vPosition = _float3(74, 0, 33);
//	//LightDesc.fRange = 40;
//	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	//LightDesc.vAmbient = _float4(0.68, 0, 1, 1);
//	//LightDesc.vSpecular = _float4(0.68,0,1,1);
//
//	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//	//	return E_FAIL;
//
//	////보
//	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	//LightDesc.vPosition = _float3(15.45, 0, 55.1);
//	//LightDesc.fRange = 40;
//	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	//LightDesc.vAmbient = _float4(0.68, 0, 1, 1);
//	//LightDesc.vSpecular = _float4(0.68, 0, 1, 1);
//
//	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//
//	//	return E_FAIL;
//	
//
//	////보
//	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	//LightDesc.vPosition = _float3(56, 0, 77);
//	//LightDesc.fRange = 15;
//	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	//LightDesc.vAmbient = _float4(0, 0, 1, 1);
//	//LightDesc.vSpecular = _float4(0, 0, 1, 1);
//
//	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//	//	return E_FAIL;
//
//	////보
//	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	//LightDesc.vPosition = _float3(17.2, 0, 54.3);
//	//LightDesc.fRange = 15;
//	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	//LightDesc.vAmbient = _float4(0, 0, 1, 1);
//	//LightDesc.vSpecular = _float4(0, 0, 1, 1);
//
//	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//	//	return E_FAIL;
//
//
//
//
//	////파
//	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	//LightDesc.vPosition = _float3(75, 0, 35.29);
//	//LightDesc.fRange = 15;
//	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	//LightDesc.vAmbient = _float4(0, 0, 1, 1);
//	//LightDesc.vSpecular = _float4(0, 0, 1, 1);
//
//	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//	//	return E_FAIL;
//
//	////파
//	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	//LightDesc.vPosition = _float3(33.97, 0, 15);
//	//LightDesc.fRange = 15;
//	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	//LightDesc.vAmbient = _float4(0, 0, 1, 1);
//	//LightDesc.vSpecular = _float4(0, 0, 1, 1);
//	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//	//	return E_FAIL;
//
//
//
//
//
//
//
//
//
//
//	/////////////////////////////////
//	////보
//	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	//LightDesc.vPosition = _float3(59.7, 0, 16.1);
//	//LightDesc.fRange = 15;
//	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	//LightDesc.vAmbient = _float4(0, 0, 1, 1);
//	//LightDesc.vSpecular = _float4(0, 0, 1, 1);
//
//	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//	//	return E_FAIL;
//
//	////보
//	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	//LightDesc.vPosition = _float3(15.65, 0, 30.76);
//	//LightDesc.fRange = 15;
//	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	//LightDesc.vAmbient = _float4(0, 0, 1, 1);
//	//LightDesc.vSpecular = _float4(0, 0, 1, 1);
//
//	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//	//	return E_FAIL;
//
//
//
//
//	////파
//	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	//LightDesc.vPosition = _float3(29.1, 0, 75.8);
//	//LightDesc.fRange = 15;
//	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	//LightDesc.vAmbient = _float4(0, 0, 1, 1);
//	//LightDesc.vSpecular = _float4(0, 0, 1, 1);
//
//	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//	//	return E_FAIL;
//
//	////파
//	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
//	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
//	//LightDesc.vPosition = _float3(79.4, 0, 60);
//	//LightDesc.fRange = 15;
//	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
//	//LightDesc.vAmbient = _float4(0, 0, 1, 1);
//	//LightDesc.vSpecular = _float4(0, 0, 1, 1);
//	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//	//	return E_FAIL;
//
//
//
//
//
//
//
//
//	//////////////////////////////////////////////////////////////////
//
//	/*if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
//		return E_FAIL;*/
//
//	
//
//	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_TERRAIN, TEXT("Layer_SpongeBob"), TEXT("Prototype_GameObject_SpongeBob"))))
//	//	return E_FAIL;
//	///* Clone_GameObject_Terrain */
//	//if (FAILED(pGameInstance->Clone_GameObject(Level_Terrain, L"Layer_Terrain", L"Prototype_GameObject_Terrain")))
//	//	return E_FAIL;
//
////	m_pCubeMgrInstance->Get_TerrainComponent();
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Terrain::DBConnection()
{
	int rc;
	char *err_msg = 0;

	rc = sqlite3_open("../../Reference/DataBase/SpongeBob_jhBaik_DB.db", &db);
	if (rc)
		sqlite3_close(db);
	else
	{

		/*string sqlTemp = "create table if not exists " + m_sTable + "(ToolLevel integer, ClientLevel integer, Obj text, LayerTag text, PrototypeTag text, Index_Num integer,Number integer,\
				 Pos_x integer, Pos_y integer, Pos_z integer, Scale_x integer, Scale_y integer, Scale_z integer, \
				Col_Pos_x integer, Col_Pos_y integer, Col_Pos_z integer, Col_Scale_x integer, Col_Scale_y integer, Col_Scale_z integer, TextureInfo text)";//일단 프라이머리키 없애고하기

		const char* sql = sqlTemp.c_str();

		rc = sqlite3_exec(db, sql, 0, 0, &err_msg);*/
	}

	if (rc != SQLITE_OK)
	{
		MSG_BOX("SQL errory");
		sqlite3_free(err_msg);
		sqlite3_close(db);

		return E_FAIL;
	}
	else
		m_IsConnection = true;
	return S_OK;
}

CLevel_Terrain * CLevel_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Terrain*		pInstance = new CLevel_Terrain(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Created : CLevel_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Terrain::Free()
{
	//아니 왜..?누수,,?
	/*CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Light_Remove();

	Safe_Release(pGameInstance);*/


	

	__super::Free();
	Safe_Release(m_pTerrainMgrInstance);
	sqlite3_close(db);




	//Safe_Release(m_pDevice);
	//Safe_Release(m_pContext);
	//Safe_Release(m_pCubeMgrInstance);
}
