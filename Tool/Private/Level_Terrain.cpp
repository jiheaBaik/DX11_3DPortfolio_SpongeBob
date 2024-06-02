#include "..\Public\Level_Terrain.h"

#include "GameInstance.h"
//
#include "Camera_Free.h"
CLevel_Terrain::CLevel_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext), m_pDevice(pDevice), m_pContext(pContext)
{
	m_sTable = "OBJ_Info";
}

HRESULT CLevel_Terrain::NativeConstruct()
{
	DBConnection();

	m_pTerrainMgrInstance = CTerrainManager::Create(m_pDevice, m_pContext, db);

	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;
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

	return S_OK;
}


HRESULT CLevel_Terrain::Ready_Prototype_Component()
{
	return S_OK;
}


HRESULT CLevel_Terrain::Ready_Clone_GameObject()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);
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
	__super::Free();
	Safe_Release(m_pTerrainMgrInstance);
	sqlite3_close(db);
}
