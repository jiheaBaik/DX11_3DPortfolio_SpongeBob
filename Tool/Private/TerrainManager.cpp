#include "..\Public\TerrainManager.h"
#include "GameInstance.h"
#include "string.h"
#include<iostream>
#include <sstream>
#include <tchar.h>
#include <comdef.h> 
#include "Cube.h"

using namespace std;
IMPLEMENT_SINGLETON(CTerrainManager)

CTerrainManager::CTerrainManager()
{
}

CTerrainManager::CTerrainManager(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, sqlite3* db)
	:m_pDevice(pDevice), m_pContext(pContext), m_db(db)
{
	ZeroMemory(&m_Obj_ClickCount, sizeof(m_Obj_ClickCount));
	ZeroMemory(&vPos, sizeof(vPos));
	m_sTable = "OBJ_Info";
}

HRESULT CTerrainManager::NativeConstruct_Prototype()
{

	return S_OK;
}

HRESULT CTerrainManager::NativeConstruct()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;
	Safe_AddRef(pGameInstance);
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_TERRAIN, TEXT("Layer_Terrain"), TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CTerrainManager::Render()
{
	SelectTable();
	Picking();
	ChangeObjInfo();
	ChangeLevelInfo();
	Delete();
	Create();
	EditObj();
	ObjSelect();
	Load();
	return S_OK;
}

HRESULT CTerrainManager::Get_TerrainComponent()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;
	Safe_AddRef(pGameInstance);

	m_pTerrainVeffer = (CVIBuffer_Terrain*)pGameInstance->Get_Component(LEVEL_TERRAIN, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));
	if (nullptr == m_pTerrainVeffer)
		return E_FAIL;

	m_pTerrainTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TERRAIN, TEXT("Layer_Terrain"), TEXT("Com_Transform"));
	if (nullptr == m_pTerrainTransform)
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

_bool CTerrainManager::CurruntCheck()
{
	int rc;
	sqlite3_stmt *res;

	string sqlTemp = "select * from " + m_sTable + " where Obj = @Obj and Index_Num = @Index_Num";
	const char* sql = sqlTemp.c_str();

	rc = sqlite3_prepare_v2(m_db, sql, -1, &res, 0);
	int index = sqlite3_bind_parameter_index(res, "@Obj");
	_bstr_t temp1(objInfo.pObj);
	const char* pObjCur = temp1;
	sqlite3_bind_text(res, index, pObjCur, -1, SQLITE_TRANSIENT);

	index = sqlite3_bind_parameter_index(res, "@Index_Num");
	sqlite3_bind_int(res, index, m_iObj_current);

	if (sqlite3_step(res) == SQLITE_ROW)
	{
		objInfo.iIndex = sqlite3_column_int(res, 5);
		objInfo.iNumber = sqlite3_column_int(res, 6);
	}
	return true;
}

void CTerrainManager::ChangeObjInfo()
{
	static _int m_iObjRadio = 0;

	if (ImGui::Begin("Obj List"))
	{
		ImGui::RadioButton("Cube", &m_iObjRadio, OBJ_CUBE); ImGui::SameLine();
		ImGui::RadioButton("Rect", &m_iObjRadio, OBJ_RECT); ImGui::SameLine();
		ImGui::RadioButton("Mesh", &m_iObjRadio, OBJ_MESH); ImGui::SameLine();
		ImGui::RadioButton("Collider", &m_iObjRadio, OBJ_COLLIDER);/* ImGui::SameLine();*/



		if (m_iObjRadio == OBJ_CUBE)
		{
			ImGui::Combo("Object Pick", &m_iObj_Listcurrent, "Cube\0");

			objInfo.toolLevel = LEVEL_TERRAIN;
			objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
			objInfo.pObj = TEXT("Cube");
			objInfo.pObj_Kind = TEXT("Cube");

			objInfo.pLayerTag = TEXT("Layer_Cube");
			objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Cube");

		}
		else if (m_iObjRadio == OBJ_RECT)
		{
			ImGui::Combo("Object Pick", &m_iObj_Listcurrent, "Rect\0");

		}
		else
		{
			ImGui::Combo("Object Pick", &m_iObj_Listcurrent, "SpongeBob\0Ground\0Ground2\0Ring_Top\0Rope_Top\0Ring_Pillar\0Light_Tube\0Scoreboard\0Big_Pillar\0Big_Pillar_Attachment\0Big_Cloth\0Pos_Light\0Stairs_02\00Stairs_04\0SM_PD_Banner_01\0Poster\00Poster2\0Fish_A\0Fish_B\0Fish_C\0Fish_D\0Fish_E\0Fish_G\0Fish_H\0Fish_I\0Fish_J\0Fish_K\0Fish_L\0Scoreboard_Ropes\0Light_Cone_Pink\0Light_Cone_Blue\0Light_Cone_Green\0Bubble\0NonAnimSpongeBob\0Camera\0NoneAnimKing\0NoneAnimSandy\0Patrick\0ScoreBoard_Broken\0Light_Cone_Yellow\0Light_Cone_Blue2\0Field\0TaxiStop\0Gate\0Robo\00Robo2\00Robo3\0JellyPink\0JellyBlue\0Grass\0Squidward\0Man\0Tree\0");
			switch (m_iObj_Listcurrent)
			{
			case MESH_SPONGBOB:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("SpongeBob");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_SpongeBob");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_SpongeBob");
				break;
			case MESH_GROUND:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Ground");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Ground");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Ground");
				break;
			case MESH_GROUND2:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Ground2");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Ground2");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Ground2");
				break;
			case MESH_RING_TOP:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Ring_Top");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Ring_Top");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Ring_Top");
				break;
			case MESH_ROPE_TOP:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Rope_Top");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Rope_Top");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Rope_Top");
				break;
			case MESH_RING_PILLAR:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Ring_Pillar");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Ring_Pillar");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Ring_Pillar");
				break;
			case MESH_LIGHT_TUBE:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Light_Tube");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Light_Tube");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Light_Tube");
				break;
			case MESH_SCOREBOARD:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Scoreboard");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Scoreboard");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Scoreboard");
				break;
			case MESH_BIG_PILLAR:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Big_Pillar");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Big_Pillar");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Big_Pillar");
				break;
			case MESH_BIG_PILLAR_ATTACHMENT:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Big_Pillar_Attachment");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Big_Pillar_Attachment");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Big_Pillar_Attachment");
				break;
			case MESH_BIG_CLOTH:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Big_Cloth");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Big_Cloth");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Big_Cloth");
				break;
			case POS_LIGHT:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Pos_Light");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Pos_Light");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Pos_Light");
				break;
			case STAIRS_02:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Stairs_02");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Stairs_02");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Stairs_02");
				break;
			case STAIRS_04:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Stairs_04");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Stairs_04");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Stairs_04");
				break;
			case MESH_SM_PD_BANNER_01:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("SM_PD_Banner_01");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_SM_PD_Banner_01");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_SM_PD_Banner_01");
				break;
			case MESH_SM_PD_POSTER:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Poster");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Poster");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Poster");
				break;
			case MESH_SM_PD_POSTER2:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Poster2");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Poster2");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Poster2");
				break;
			case MESH_FISH_A:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Fish_A");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Fish_A");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Fish_A");
				break;
			case MESH_FISH_B:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Fish_B");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Fish_B");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Fish_B");
				break;
			case MESH_FISH_C:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Fish_C");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Fish_C");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Fish_C");
				break;
			case MESH_FISH_D:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Fish_D");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Fish_D");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Fish_D");
				break;
			case MESH_FISH_E:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Fish_E");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Fish_E");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Fish_E");
				break;
			case MESH_FISH_G:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Fish_G");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Fish_G");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Fish_G");
				break;
			case MESH_FISH_H:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Fish_H");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Fish_H");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Fish_H");
				break;
			case MESH_FISH_I:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Fish_I");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Fish_I");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Fish_I");
				break;
			case MESH_FISH_J:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Fish_J");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Fish_J");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Fish_J");
				break;
			case MESH_FISH_K:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Fish_K");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Fish_K");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Fish_K");
				break;
			case MESH_FISH_L:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Fish_L");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Fish_L");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Fish_L");
				break;
			case MESH_SCOREBOARD_ROPES:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Scoreboard_Ropes");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Scoreboard_Ropes");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Scoreboard_Ropes");
				break;
			case MESH_LIGHT_CONE_PINK:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Light_Cone_Pink");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Light_Cone_Pink");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Light_Cone_Pink");
				break;
			case MESH_LIGHT_CONE_BLUE:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Light_Cone_Blue");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Light_Cone_Blue");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Light_Cone_Blue");
				break;
			case MESH_BUBBLE:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Bubble");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Bubble");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Bubble");
				break;
			case MESH_NOANIMNSPONGEBOB:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_noanimnspongebob");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Mesh_noanimnspongebob");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Mesh_noanimnspongebob");
				break;
			case MESH_CAMERA:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_camera");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Mesh_camera");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Mesh_camera");
				break;
			case MESH_NOANIMNKING:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_noanimnking");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Mesh_noanimnking");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Mesh_noanimnking");
				break;
			case MESH_NOANIMNSANDY:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_noanimnsandy");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Mesh_noanimnsandy");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Mesh_noanimnsandy");
				break;
			case MESH_NOANIMPATRICK:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_noanimpatrick");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Mesh_noanimpatrick");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Mesh_noanimnpatrick");
				break;

			case MESH_SCOREBOARD_BROKEN:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_scoreboard_broken");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Mesh_scoreboard_broken");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Mesh_scoreboard_broken");
				break;

			case MESH_LIGHT_CONE_YELLOW:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_light_cone_yellow");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Mesh_light_cone_yellow");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Light_Cone_Yellow");
				break;

			case MESH_LIGHT_CONE_BLUE2:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_light_cone_blue2");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Mesh_light_cone_blue2");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Light_Cone_Blue2");
				break;

			case MESH_FIELD:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_field");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Mesh_field");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Field");
				break;
			case MESH_TAXI:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_TaxiStop");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_TaxiStop");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_TaxiStop");
				break;
			case MESH_GATE:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_Gate");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Gate");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Gate");
				break;
			case MESH_ROBO:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_Robo");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Robo");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Robo");
				break;
			case MESH_ROBO2:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_Robo2");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Robo2");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Robo2");
				break;
			case MESH_ROBO3:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_Robo3");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Robo3");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Robo3");
				break;
			case MESH_JELLY_PINK:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_Jelly_Pink");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Jelly_Pink");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Jelly_Pink");
				break;
			case MESH_JELLY_BLUE:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_Jelly_Blue");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Jelly_Blue");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Jelly_Blue");
				break;
			case MESH_GRASS:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_Grass");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Grass");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Grass");
				break;
			case MESH_SQUIDWARD:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_Squidward");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Squidward");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Squidward");
				break;
			case MESH_MAN:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_Man");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Man");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Man");
				break;
			case MESH_TREE:
				objInfo.toolLevel = LEVEL_TERRAIN;
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)m_ClientLevel;
				objInfo.pObj = TEXT("Mesh_Tree");
				objInfo.pObj_Kind = TEXT("Mesh");
				objInfo.pLayerTag = TEXT("Layer_Tree");
				objInfo.pPrototypeTag = TEXT("Prototype_GameObject_Tree");
				break;
			}
		}
		if (m_bIsSelect)
		{
			m_vecStrListTemp = ChangeListInfo(objInfo.pObj);
			m_bIsSelect = false;
		}

	}
	ImGui::End();

}

void CTerrainManager::ChangeLevelInfo()
{

	if (ImGui::Begin("Obj List"))
	{
		ImGui::Combo("Client Level Pick", &m_ClientLevel, "LEVELE_BOSS1\0LEVELE_BOSS2\0");

		if (ImGui::Button("select", ImVec2(250, 25)))
			m_bIsSelect = true;

		ImGui::Separator();

		if (ImGui::BeginListBox("Obj List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int i = 0; i < m_vecStrListTemp.size(); i++)
			{
				const bool is_selected = (m_iObj_current == i);
				if (ImGui::Selectable(m_vecStrListTemp[i].c_str(), is_selected))
					m_iObj_current = i;
			}
		}
		ImGui::EndListBox();
	}
	ImGui::End();
}

HRESULT CTerrainManager::Delete()
{
	if (ImGui::Begin("Obj List"))
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		if (nullptr == pGameInstance)
			return E_FAIL;

		Safe_AddRef(pGameInstance);

		if (ImGui::Button("Delete"))
		{
			m_bIsSelect = true;
			CurruntCheck();
			if (FAILED(pGameInstance->Distroy_GameObject(LEVEL_TERRAIN, objInfo.pLayerTag, false, objInfo.iIndex)))
				MSG_BOX("Failed Distroy");
	
			if (isNavigation)
			{
				if (m_vNavigation.size() != 0)
				{
					if (m_vNavigation.size() - 1 % 3)
					{
						for (int i = 0; i < 3; i++)
							m_vNavigation.pop_back();
						m_pObjNavigation->Set_CellsPop();
					}
					else
						m_vNavigation.pop_back();
				}


			}

			m_Obj_ClickCount.isCreate = false;
			ClickCount(m_Obj_ClickCount.isCreate);
			int size = m_vecStrListTemp.size() - 1 * 0.5;

			int rc;
			char *err_msg = 0;
			sqlite3_stmt *res2;

			string sqlTemp = "DELETE FROM " + m_sTable + " where Obj = @_objkind and Index_Num = @_Number";
			const char* sql2 = "";

			if (m_iObj_current > -1)
				sql2 = sqlTemp.c_str();

			rc = sqlite3_prepare_v2(m_db, sql2, -1, &res2, NULL);
			if (SQLITE_OK == rc)
			{
				int a = 0;
			}
			int index = sqlite3_bind_parameter_index(res2, "@_objkind");
			_bstr_t temp1(objInfo.pObj);
			const char* pObj = temp1;
			sqlite3_bind_text(res2, index, pObj, -1, SQLITE_TRANSIENT);
			index = sqlite3_bind_parameter_index(res2, "@_Number");
			sqlite3_bind_int(res2, index, m_iObj_current);

			int step = sqlite3_step(res2);
			m_vecStrListTemp = ChangeListInfo(objInfo.pObj);

			sqlTemp = "Update " + m_sTable + " set Index_Num = Index_Num -1 where Obj = @_objkind and Index_Num > @Count";
			sql2 = sqlTemp.c_str();
			rc = sqlite3_prepare_v2(m_db, sql2, -1, &res2, NULL);
			if (SQLITE_OK == rc)
			{
				int a = 0;
			}
			index = sqlite3_bind_parameter_index(res2, "@Count");
			sqlite3_bind_int(res2, index, m_iObj_current);

			index = sqlite3_bind_parameter_index(res2, "@_objkind");
			sqlite3_bind_text(res2, index, pObj, -1, SQLITE_TRANSIENT);

			step = sqlite3_step(res2);
		}
		Safe_Release(pGameInstance);
	}

	ImGui::End();
	return S_OK;

}

void CTerrainManager::ClickCount(_bool _isCreate)
{
	if (_isCreate)
	{
		if (objInfo.pObj == L"Cube")
		{
			objInfo.iNumber = m_Obj_ClickCount.Cube;
			objInfo.iIndex = m_Obj_ClickCount.Cube;
			m_Obj_ClickCount.Cube++;

		}
		else if (objInfo.pObj == L"Rect")
		{
			objInfo.iNumber = m_Obj_ClickCount.Rect;
			objInfo.iIndex = m_Obj_ClickCount.Rect;
			m_Obj_ClickCount.Rect++;

		}
		else if (objInfo.pObj == L"SpongeBob")
		{
			objInfo.iNumber = m_Obj_ClickCount.SpongeBob;
			objInfo.iIndex = m_Obj_ClickCount.SpongeBob;
			m_Obj_ClickCount.SpongeBob++;
		}
		else if (objInfo.pObj == L"Ground")
		{
			objInfo.iNumber = m_Obj_ClickCount.Ground;
			objInfo.iIndex = m_Obj_ClickCount.Ground;
			m_Obj_ClickCount.Ground++;
		}
		else if (objInfo.pObj == L"Ground2")
		{
			objInfo.iNumber = m_Obj_ClickCount.Ground2;
			objInfo.iIndex = m_Obj_ClickCount.Ground2;
			m_Obj_ClickCount.Ground2++;
		}
		else if (objInfo.pObj == L"Ring_Top")
		{
			objInfo.iNumber = m_Obj_ClickCount.Ring_Top;
			objInfo.iIndex = m_Obj_ClickCount.Ring_Top;
			m_Obj_ClickCount.Ring_Top++;
		}
		else if (objInfo.pObj == L"Rope_Top")
		{
			objInfo.iNumber = m_Obj_ClickCount.Rope_Top;
			objInfo.iIndex = m_Obj_ClickCount.Rope_Top;
			m_Obj_ClickCount.Rope_Top++;
		}
		else if (objInfo.pObj == L"Ring_Pillar")
		{
			objInfo.iNumber = m_Obj_ClickCount.Ring_Pillar;
			objInfo.iIndex = m_Obj_ClickCount.Ring_Pillar;
			m_Obj_ClickCount.Ring_Pillar++;
		}
		else if (objInfo.pObj == L"Light_Tube")
		{
			objInfo.iNumber = m_Obj_ClickCount.Light_Tube;
			objInfo.iIndex = m_Obj_ClickCount.Light_Tube;
			m_Obj_ClickCount.Light_Tube++;
		}
		else if (objInfo.pObj == L"Scoreboard")
		{
			objInfo.iNumber = m_Obj_ClickCount.Scoreboard;
			objInfo.iIndex = m_Obj_ClickCount.Scoreboard;
			m_Obj_ClickCount.Scoreboard++;
		}
		else if (objInfo.pObj == L"Big_Pillar")
		{
			objInfo.iNumber = m_Obj_ClickCount.Big_Pillar;
			objInfo.iIndex = m_Obj_ClickCount.Big_Pillar;
			m_Obj_ClickCount.Big_Pillar++;
		}
		else if (objInfo.pObj == L"Big_Pillar_Attachment")
		{
			objInfo.iNumber = m_Obj_ClickCount.Big_Pillar_Attachment;
			objInfo.iIndex = m_Obj_ClickCount.Big_Pillar_Attachment;
			m_Obj_ClickCount.Big_Pillar_Attachment++;
		}
		else if (objInfo.pObj == L"Big_Cloth")
		{
			objInfo.iNumber = m_Obj_ClickCount.Big_Cloth;
			objInfo.iIndex = m_Obj_ClickCount.Big_Cloth;
			m_Obj_ClickCount.Big_Cloth++;
		}
		else if (objInfo.pObj == L"Pos_Light")
		{
			objInfo.iNumber = m_Obj_ClickCount.Pos_Light;
			objInfo.iIndex = m_Obj_ClickCount.Pos_Light;
			m_Obj_ClickCount.Pos_Light++;
		}
		else if (objInfo.pObj == L"Stairs_02")
		{
			objInfo.iNumber = m_Obj_ClickCount.Stairs_02;
			objInfo.iIndex = m_Obj_ClickCount.Stairs_02;
			m_Obj_ClickCount.Stairs_02++;
		}
		else if (objInfo.pObj == L"Stairs_04")
		{
			objInfo.iNumber = m_Obj_ClickCount.Stairs_04;
			objInfo.iIndex = m_Obj_ClickCount.Stairs_04;
			m_Obj_ClickCount.Stairs_04++;
		}
		else if (objInfo.pObj == L"SM_PD_Banner_01")
		{
			objInfo.iNumber = m_Obj_ClickCount.SM_PD_Banner_01;
			objInfo.iIndex = m_Obj_ClickCount.SM_PD_Banner_01;
			m_Obj_ClickCount.SM_PD_Banner_01++;
		}
		else if (objInfo.pObj == L"Poster")
		{
			objInfo.iNumber = m_Obj_ClickCount.Poster;
			objInfo.iIndex = m_Obj_ClickCount.Poster;
			m_Obj_ClickCount.Poster++;
		}
		else if (objInfo.pObj == L"Poster2")
		{
			objInfo.iNumber = m_Obj_ClickCount.Poster2;
			objInfo.iIndex = m_Obj_ClickCount.Poster2;
			m_Obj_ClickCount.Poster2++;
		}
		else if (objInfo.pObj == L"Fish_A")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_A;
			objInfo.iIndex = m_Obj_ClickCount.Fish_A;
			m_Obj_ClickCount.Fish_A++;
		}
		else if (objInfo.pObj == L"Fish_B")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_B;
			objInfo.iIndex = m_Obj_ClickCount.Fish_B;
			m_Obj_ClickCount.Fish_B++;
		}
		else if (objInfo.pObj == L"Fish_C")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_C;
			objInfo.iIndex = m_Obj_ClickCount.Fish_C;
			m_Obj_ClickCount.Fish_C++;
		}
		else if (objInfo.pObj == L"Fish_D")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_D;
			objInfo.iIndex = m_Obj_ClickCount.Fish_D;
			m_Obj_ClickCount.Fish_D++;
		}
		else if (objInfo.pObj == L"Fish_E")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_E;
			objInfo.iIndex = m_Obj_ClickCount.Fish_E;
			m_Obj_ClickCount.Fish_E++;
		}
		else if (objInfo.pObj == L"Fish_G")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_G;
			objInfo.iIndex = m_Obj_ClickCount.Fish_G;
			m_Obj_ClickCount.Fish_G++;
		}
		else if (objInfo.pObj == L"Fish_H")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_H;
			objInfo.iIndex = m_Obj_ClickCount.Fish_H;
			m_Obj_ClickCount.Fish_H++;
		}
		else if (objInfo.pObj == L"Fish_I")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_I;
			objInfo.iIndex = m_Obj_ClickCount.Fish_I;
			m_Obj_ClickCount.Fish_I++;
		}
		else if (objInfo.pObj == L"Fish_J")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_J;
			objInfo.iIndex = m_Obj_ClickCount.Fish_J;
			m_Obj_ClickCount.Fish_J++;
		}
		else if (objInfo.pObj == L"Fish_K")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_K;
			objInfo.iIndex = m_Obj_ClickCount.Fish_K;
			m_Obj_ClickCount.Fish_K++;
		}
		else if (objInfo.pObj == L"Fish_L")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_L;
			objInfo.iIndex = m_Obj_ClickCount.Fish_L;
			m_Obj_ClickCount.Fish_L++;
		}
		else if (objInfo.pObj == L"Scoreboard_Ropes")
		{
			objInfo.iNumber = m_Obj_ClickCount.Scoreboard_Ropes;
			objInfo.iIndex = m_Obj_ClickCount.Scoreboard_Ropes;
			m_Obj_ClickCount.Scoreboard_Ropes++;
		}
		else if (objInfo.pObj == L"Light_Cone_Pink")
		{
			objInfo.iNumber = m_Obj_ClickCount.Light_Cone_Pink;
			objInfo.iIndex = m_Obj_ClickCount.Light_Cone_Pink;
			m_Obj_ClickCount.Light_Cone_Pink++;
		}
		else if (objInfo.pObj == L"Light_Cone_Blue")
		{
			objInfo.iNumber = m_Obj_ClickCount.Light_Cone_Blue;
			objInfo.iIndex = m_Obj_ClickCount.Light_Cone_Blue;
			m_Obj_ClickCount.Light_Cone_Blue++;
		}
		else if (objInfo.pObj == L"Light_Cone_Green")
		{
			objInfo.iNumber = m_Obj_ClickCount.Light_Cone_Green;
			objInfo.iIndex = m_Obj_ClickCount.Light_Cone_Green;
			m_Obj_ClickCount.Light_Cone_Green++;
		}
		else if (objInfo.pObj == L"Bubble")
		{
			objInfo.iNumber = m_Obj_ClickCount.Bubble;
			objInfo.iIndex = m_Obj_ClickCount.Bubble;
			m_Obj_ClickCount.Bubble++;
		}
		else if (objInfo.pObj == L"Mesh_noanimnspongebob")
		{
			objInfo.iNumber = m_Obj_ClickCount.NonAnimSpongeBob;
			objInfo.iIndex = m_Obj_ClickCount.NonAnimSpongeBob;
			m_Obj_ClickCount.NonAnimSpongeBob++;
		}
		else if (objInfo.pObj == L"Mesh_camera")
		{
			objInfo.iNumber = m_Obj_ClickCount.Camera;
			objInfo.iIndex = m_Obj_ClickCount.Camera;
			m_Obj_ClickCount.Camera++;
		}
		else if (objInfo.pObj == L"Mesh_noanimnking")
		{
			objInfo.iNumber = m_Obj_ClickCount.NoneAnimKing;
			objInfo.iIndex = m_Obj_ClickCount.NoneAnimKing;
			m_Obj_ClickCount.NoneAnimKing++;
		}
		else if (objInfo.pObj == L"Mesh_noanimnsandy")
		{
			objInfo.iNumber = m_Obj_ClickCount.NoneAnimSandy;
			objInfo.iIndex = m_Obj_ClickCount.NoneAnimSandy;
			m_Obj_ClickCount.NoneAnimSandy++;
		}
		else if (objInfo.pObj == L"Mesh_noanimpatrick")
		{
			objInfo.iNumber = m_Obj_ClickCount.NoneAnimPatrick;
			objInfo.iIndex = m_Obj_ClickCount.NoneAnimPatrick;
			m_Obj_ClickCount.NoneAnimPatrick++;
		}
		else if (objInfo.pObj == L"Mesh_scoreboard_broken")
		{
			objInfo.iNumber = m_Obj_ClickCount.Scoreboard_Broken;
			objInfo.iIndex = m_Obj_ClickCount.Scoreboard_Broken;
			m_Obj_ClickCount.Scoreboard_Broken++;
		}
		else if (objInfo.pObj == L"Mesh_light_cone_yellow")
		{
			objInfo.iNumber = m_Obj_ClickCount.Light_Cone_Yellow;
			objInfo.iIndex = m_Obj_ClickCount.Light_Cone_Yellow;
			m_Obj_ClickCount.Light_Cone_Yellow++;
		}
		else if (objInfo.pObj == L"Mesh_light_cone_blue2")
		{
			objInfo.iNumber = m_Obj_ClickCount.Light_Cone_Blue2;
			objInfo.iIndex = m_Obj_ClickCount.Light_Cone_Blue2;
			m_Obj_ClickCount.Light_Cone_Blue2++;
		}
		else if (objInfo.pObj == L"Mesh_TaxiStop")
		{
			objInfo.iNumber = m_Obj_ClickCount.Taxi;
			objInfo.iIndex = m_Obj_ClickCount.Taxi;
			m_Obj_ClickCount.Taxi++;
		}
		else if (objInfo.pObj == L"Mesh_Gate")
		{
			objInfo.iNumber = m_Obj_ClickCount.Gate;
			objInfo.iIndex = m_Obj_ClickCount.Gate;
			m_Obj_ClickCount.Gate++;
		}
		else if (objInfo.pObj == L"Mesh_Robo")
		{
			objInfo.iNumber = m_Obj_ClickCount.Robo;
			objInfo.iIndex = m_Obj_ClickCount.Robo;
			m_Obj_ClickCount.Robo++;
		}
		else if (objInfo.pObj == L"Mesh_Robo2")
		{
			objInfo.iNumber = m_Obj_ClickCount.Robo2;
			objInfo.iIndex = m_Obj_ClickCount.Robo2;
			m_Obj_ClickCount.Robo2++;
		}
		else if (objInfo.pObj == L"Mesh_Robo3")
		{
			objInfo.iNumber = m_Obj_ClickCount.Robo3;
			objInfo.iIndex = m_Obj_ClickCount.Robo3;
			m_Obj_ClickCount.Robo3++;
		}
		else if (objInfo.pObj == L"Mesh_Jelly_Pink")
		{
			objInfo.iNumber = m_Obj_ClickCount.JellyPink;
			objInfo.iIndex = m_Obj_ClickCount.JellyPink;
			m_Obj_ClickCount.JellyPink++;
		}
		else if (objInfo.pObj == L"Mesh_Jelly_Blue")
		{
			objInfo.iNumber = m_Obj_ClickCount.JellyBlue;
			objInfo.iIndex = m_Obj_ClickCount.JellyBlue;
			m_Obj_ClickCount.JellyBlue++;
		}
		else if (objInfo.pObj == L"Mesh_Grass")
		{
			objInfo.iNumber = m_Obj_ClickCount.Grass;
			objInfo.iIndex = m_Obj_ClickCount.Grass;
			m_Obj_ClickCount.Grass++;
		}
		else if (objInfo.pObj == L"Mesh_Squidward")
		{
			objInfo.iNumber = m_Obj_ClickCount.Squidward;
			objInfo.iIndex = m_Obj_ClickCount.Squidward;
			m_Obj_ClickCount.Squidward++;
		}
		else if (objInfo.pObj == L"Mesh_Man")
		{
			objInfo.iNumber = m_Obj_ClickCount.Man;
			objInfo.iIndex = m_Obj_ClickCount.Man;
			m_Obj_ClickCount.Man++;
		}
		else if (objInfo.pObj == L"Mesh_Tree")
		{
			objInfo.iNumber = m_Obj_ClickCount.Tree;
			objInfo.iIndex = m_Obj_ClickCount.Tree;
			m_Obj_ClickCount.Tree++;
		}
	}
	else
	{
		if (objInfo.pObj == L"Cube")
		{
			objInfo.iNumber = m_Obj_ClickCount.Cube;
			if (m_Obj_ClickCount.Cube > 0)
				m_Obj_ClickCount.Cube--;
		}
		else if (objInfo.pObj == L"Rect")
		{
			objInfo.iNumber = m_Obj_ClickCount.Rect;
			if (m_Obj_ClickCount.Rect > 0)
				m_Obj_ClickCount.Rect--;
		}
		else if (objInfo.pObj == L"SpongeBob")
		{
			objInfo.iNumber = m_Obj_ClickCount.SpongeBob;
			if (m_Obj_ClickCount.SpongeBob > 0)
				m_Obj_ClickCount.SpongeBob--;

		}
		else if (objInfo.pObj == L"Ground")
		{
			objInfo.iNumber = m_Obj_ClickCount.Ground;
			if (m_Obj_ClickCount.Ground > 0)
				m_Obj_ClickCount.Ground--;
		}
		else if (objInfo.pObj == L"Ground2")
		{
			objInfo.iNumber = m_Obj_ClickCount.Ground2;
			if (m_Obj_ClickCount.Ground2 > 0)
				m_Obj_ClickCount.Ground2--;
		}
		else if (objInfo.pObj == L"Ring_Top")
		{
			objInfo.iNumber = m_Obj_ClickCount.Ring_Top;
			if (m_Obj_ClickCount.Ring_Top > 0)
				m_Obj_ClickCount.Ring_Top--;
		}
		else if (objInfo.pObj == L"Rope_Top")
		{
			objInfo.iNumber = m_Obj_ClickCount.Rope_Top;
			if (m_Obj_ClickCount.Rope_Top > 0)
				m_Obj_ClickCount.Rope_Top--;
		}
		else if (objInfo.pObj == L"Ring_Pillar")
		{
			objInfo.iNumber = m_Obj_ClickCount.Ring_Pillar;
			if (m_Obj_ClickCount.Ring_Pillar > 0)
				m_Obj_ClickCount.Ring_Pillar--;
		}
		else if (objInfo.pObj == L"Light_Tube")
		{
			objInfo.iNumber = m_Obj_ClickCount.Light_Tube;
			if (m_Obj_ClickCount.Light_Tube > 0)
				m_Obj_ClickCount.Light_Tube--;
		}
		else if (objInfo.pObj == L"Scoreboard")
		{
			objInfo.iNumber = m_Obj_ClickCount.Scoreboard;
			if (m_Obj_ClickCount.Scoreboard > 0)
				m_Obj_ClickCount.Scoreboard--;
		}
		else if (objInfo.pObj == L"Big_Pillar")
		{
			objInfo.iNumber = m_Obj_ClickCount.Big_Pillar;
			if (m_Obj_ClickCount.Big_Pillar > 0)
				m_Obj_ClickCount.Big_Pillar--;
		}
		else if (objInfo.pObj == L"Big_Pillar_Attachment")
		{
			objInfo.iNumber = m_Obj_ClickCount.Big_Pillar_Attachment;
			if (m_Obj_ClickCount.Big_Pillar_Attachment > 0)
				m_Obj_ClickCount.Big_Pillar_Attachment--;
		}
		else if (objInfo.pObj == L"Big_Cloth")
		{
			objInfo.iNumber = m_Obj_ClickCount.Big_Cloth;
			if (m_Obj_ClickCount.Big_Cloth > 0)
				m_Obj_ClickCount.Big_Cloth--;
		}
		else if (objInfo.pObj == L"Pos_Light")
		{
			objInfo.iNumber = m_Obj_ClickCount.Pos_Light;
			if (m_Obj_ClickCount.Pos_Light > 0)
				m_Obj_ClickCount.Pos_Light--;
		}
		else if (objInfo.pObj == L"Stairs_02")
		{
			objInfo.iNumber = m_Obj_ClickCount.Stairs_02;
			if (m_Obj_ClickCount.Stairs_02 > 0)
				m_Obj_ClickCount.Stairs_02--;
		}
		else if (objInfo.pObj == L"Stairs_04")
		{
			objInfo.iNumber = m_Obj_ClickCount.Stairs_04;
			if (m_Obj_ClickCount.Stairs_04 > 0)
				m_Obj_ClickCount.Stairs_04--;
		}
		else if (objInfo.pObj == L"SM_PD_Banner_01")
		{
			objInfo.iNumber = m_Obj_ClickCount.SM_PD_Banner_01;
			if (m_Obj_ClickCount.SM_PD_Banner_01 > 0)
				m_Obj_ClickCount.SM_PD_Banner_01--;
		}
		else if (objInfo.pObj == L"Poster")
		{
			objInfo.iNumber = m_Obj_ClickCount.Poster;
			if (m_Obj_ClickCount.Poster > 0)
				m_Obj_ClickCount.Poster--;
		}
		else if (objInfo.pObj == L"Poster2")
		{
			objInfo.iNumber = m_Obj_ClickCount.Poster2;
			if (m_Obj_ClickCount.Poster2 > 0)
				m_Obj_ClickCount.Poster2--;
		}
		else if (objInfo.pObj == L"Fish_A")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_A;
			if (m_Obj_ClickCount.Fish_A > 0)
				m_Obj_ClickCount.Fish_A--;
		}
		else if (objInfo.pObj == L"Fish_B")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_B;
			if (m_Obj_ClickCount.Fish_B > 0)
				m_Obj_ClickCount.Fish_B--;
		}
		else if (objInfo.pObj == L"Fish_C")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_C;
			if (m_Obj_ClickCount.Fish_C > 0)
				m_Obj_ClickCount.Fish_C--;
		}
		else if (objInfo.pObj == L"Fish_D")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_D;
			if (m_Obj_ClickCount.Fish_D > 0)
				m_Obj_ClickCount.Fish_D--;
		}
		else if (objInfo.pObj == L"Fish_E")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_E;
			if (m_Obj_ClickCount.Fish_E > 0)
				m_Obj_ClickCount.Fish_E--;
		}
		else if (objInfo.pObj == L"Fish_G")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_G;
			if (m_Obj_ClickCount.Fish_G > 0)
				m_Obj_ClickCount.Fish_G--;
		}
		else if (objInfo.pObj == L"Fish_H")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_H;
			if (m_Obj_ClickCount.Fish_H > 0)
				m_Obj_ClickCount.Fish_H--;
		}
		else if (objInfo.pObj == L"Fish_I")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_I;
			if (m_Obj_ClickCount.Fish_I > 0)
				m_Obj_ClickCount.Fish_I--;
		}
		else if (objInfo.pObj == L"Fish_J")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_J;
			if (m_Obj_ClickCount.Fish_J > 0)
				m_Obj_ClickCount.Fish_J--;
		}
		else if (objInfo.pObj == L"Fish_K")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_K;
			if (m_Obj_ClickCount.Fish_K > 0)
				m_Obj_ClickCount.Fish_K--;
		}
		else if (objInfo.pObj == L"Fish_L")
		{
			objInfo.iNumber = m_Obj_ClickCount.Fish_L;
			if (m_Obj_ClickCount.Fish_L > 0)
				m_Obj_ClickCount.Fish_L--;
		}
		else if (objInfo.pObj == L"Scoreboard_Ropes")
		{
			objInfo.iNumber = m_Obj_ClickCount.Scoreboard_Ropes;
			if (m_Obj_ClickCount.Scoreboard_Ropes > 0)
				m_Obj_ClickCount.Scoreboard_Ropes--;
		}
		else if (objInfo.pObj == L"Light_Cone_Pink")
		{
			objInfo.iNumber = m_Obj_ClickCount.Light_Cone_Pink;
			if (m_Obj_ClickCount.Light_Cone_Pink > 0)
				m_Obj_ClickCount.Light_Cone_Pink--;
		}
		else if (objInfo.pObj == L"Light_Cone_Blue")
		{
			objInfo.iNumber = m_Obj_ClickCount.Light_Cone_Blue;
			if (m_Obj_ClickCount.Light_Cone_Blue > 0)
				m_Obj_ClickCount.Light_Cone_Blue--;
		}
		else if (objInfo.pObj == L"Light_Cone_Green")
		{
			objInfo.iNumber = m_Obj_ClickCount.Light_Cone_Green;
			if (m_Obj_ClickCount.Light_Cone_Green > 0)
				m_Obj_ClickCount.Light_Cone_Green--;
		}
		else if (objInfo.pObj == L"Bubble")
		{
			objInfo.iNumber = m_Obj_ClickCount.Bubble;
			if (m_Obj_ClickCount.Bubble > 0)
				m_Obj_ClickCount.Bubble--;
		}
		else if (objInfo.pObj == L"Mesh_noanimnspongebob")
		{
			objInfo.iNumber = m_Obj_ClickCount.NonAnimSpongeBob;
			if (m_Obj_ClickCount.NonAnimSpongeBob > 0)
				m_Obj_ClickCount.NonAnimSpongeBob--;
		}
		else if (objInfo.pObj == L"Mesh_camera")
		{
			objInfo.iNumber = m_Obj_ClickCount.Camera;
			if (m_Obj_ClickCount.Camera > 0)
				m_Obj_ClickCount.Camera--;
		}
		else if (objInfo.pObj == L"Mesh_noanimnking")
		{
			objInfo.iNumber = m_Obj_ClickCount.NoneAnimKing;
			if (m_Obj_ClickCount.NoneAnimKing > 0)
				m_Obj_ClickCount.NoneAnimKing--;
		}
		else if (objInfo.pObj == L"Mesh_noanimnsandy")
		{
			objInfo.iNumber = m_Obj_ClickCount.NoneAnimSandy;
			if (m_Obj_ClickCount.NoneAnimSandy > 0)
				m_Obj_ClickCount.NoneAnimSandy--;
		}
		else if (objInfo.pObj == L"Mesh_noanimpatrick")
		{
			objInfo.iNumber = m_Obj_ClickCount.NoneAnimPatrick;
			if (m_Obj_ClickCount.NoneAnimPatrick > 0)
				m_Obj_ClickCount.NoneAnimPatrick--;
		}
		else if (objInfo.pObj == L"Mesh_scoreboard_broken")
		{
			objInfo.iNumber = m_Obj_ClickCount.Scoreboard_Broken;
			if (m_Obj_ClickCount.Scoreboard_Broken > 0)
				m_Obj_ClickCount.Scoreboard_Broken--;
		}
		else if (objInfo.pObj == L"Mesh_light_cone_yellow")
		{
			objInfo.iNumber = m_Obj_ClickCount.Light_Cone_Yellow;
			if (m_Obj_ClickCount.Light_Cone_Yellow > 0)
				m_Obj_ClickCount.Light_Cone_Yellow--;
		}
		else if (objInfo.pObj == L"Mesh_light_cone_blue2")
		{
			objInfo.iNumber = m_Obj_ClickCount.Light_Cone_Blue2;
			if (m_Obj_ClickCount.Light_Cone_Blue2 > 0)
				m_Obj_ClickCount.Light_Cone_Blue2--;
		}
		else if (objInfo.pObj == L"Mesh_TaxiStop")
		{
			objInfo.iNumber = m_Obj_ClickCount.Taxi;
			if (m_Obj_ClickCount.Taxi > 0)
				m_Obj_ClickCount.Taxi--;
		}
		else if (objInfo.pObj == L"Mesh_Gate")
		{
			objInfo.iNumber = m_Obj_ClickCount.Gate;
			if (m_Obj_ClickCount.Gate > 0)
				m_Obj_ClickCount.Gate--;
		}
		else if (objInfo.pObj == L"Mesh_Robo")
		{
			objInfo.iNumber = m_Obj_ClickCount.Robo;
			if (m_Obj_ClickCount.Robo > 0)
				m_Obj_ClickCount.Robo--;
		}
		else if (objInfo.pObj == L"Mesh_Robo2")
		{
			objInfo.iNumber = m_Obj_ClickCount.Robo2;
			if (m_Obj_ClickCount.Robo2 > 0)
				m_Obj_ClickCount.Robo2--;
		}
		else if (objInfo.pObj == L"Mesh_Robo3")
		{
			objInfo.iNumber = m_Obj_ClickCount.Robo3;
			if (m_Obj_ClickCount.Robo3 > 0)
				m_Obj_ClickCount.Robo3--;
		}
		else if (objInfo.pObj == L"Mesh_Jelly_Pink")
		{
			objInfo.iNumber = m_Obj_ClickCount.JellyPink;
			if (m_Obj_ClickCount.JellyPink > 0)
				m_Obj_ClickCount.JellyPink--;
		}
		else if (objInfo.pObj == L"Mesh_Jelly_Blue")
		{
			objInfo.iNumber = m_Obj_ClickCount.JellyBlue;
			if (m_Obj_ClickCount.JellyBlue > 0)
				m_Obj_ClickCount.JellyBlue--;
		}
		else if (objInfo.pObj == L"Mesh_Grass")
		{
			objInfo.iNumber = m_Obj_ClickCount.Grass;
			if (m_Obj_ClickCount.Grass > 0)
				m_Obj_ClickCount.Grass--;
		}
		else if (objInfo.pObj == L"Mesh_Squidward")
		{
			objInfo.iNumber = m_Obj_ClickCount.Squidward;
			if (m_Obj_ClickCount.Squidward > 0)
				m_Obj_ClickCount.Squidward--;
		}
		else if (objInfo.pObj == L"Mesh_Man")
		{
			objInfo.iNumber = m_Obj_ClickCount.Man;
			if (m_Obj_ClickCount.Man > 0)
				m_Obj_ClickCount.Man--;
		}
		else if (objInfo.pObj == L"Mesh_Tree")
		{
			objInfo.iNumber = m_Obj_ClickCount.Tree;
			if (m_Obj_ClickCount.Tree > 0)
				m_Obj_ClickCount.Tree--;
		}
	}
}

HRESULT CTerrainManager::Picking()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;
	Safe_AddRef(pGameInstance);

	if (GetAsyncKeyState('C') & 0x0001)
	{
		_float3 vtrColPostest;

		if (pGameInstance->Picking(m_pTerrainTransform, m_pTerrainVeffer, &vtrColPostest))
		{
			vPos = vtrColPostest;
			isPicking = true;
		}

	}	enum { LAYER_TAG = 3, INDEX = 6 };

	if (GetAsyncKeyState('P') & 0x0001)
	{
		int iRc = 0;
		char *err_msg = 0;
		sqlite3_stmt *res = nullptr;
		_float3 vtrGetPos = {};
		string sqlResult = "select * from " + m_sTable + " where LayerTag = @objLayer";
		const char* pSql = sqlResult.c_str();
		iRc = sqlite3_prepare_v2(m_db, pSql, -1, &res, NULL);

		int index = sqlite3_bind_parameter_index(res, "@objLayer");
		_bstr_t temp1(objInfo.pLayerTag);
		const char* pObjLayer = temp1;
		sqlite3_bind_text(res, index, pObjLayer, -1, SQLITE_TRANSIENT);

		while (sqlite3_step(res) == SQLITE_ROW){
			_bstr_t bstrLayerTag((char*)sqlite3_column_text(res, LAYER_TAG));
			_int iIndex = sqlite3_column_int(res, INDEX);

			CModel* m_pObjnVeffer = (CModel*)pGameInstance->Get_Component(LEVEL_TERRAIN, bstrLayerTag, TEXT("Com_VIBuffer"), iIndex);
			if (nullptr == m_pObjnVeffer)
				return E_FAIL;
			
			CTransform* m_pObjTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TERRAIN, bstrLayerTag, TEXT("Com_Transform"), iIndex);
			if (nullptr == m_pObjTransform)
				return E_FAIL;
			if (pGameInstance->Picking(m_pObjTransform, (CVIBuffer*)m_pObjnVeffer->GetMeshContainers().front(), &vtrGetPos)){
				m_iObj_current = iIndex;
				break;
			}
		}


	}

	if (GetAsyncKeyState('E') & 0x0001)
	{
		_float3 vtrColPos;
		CVIBuffer* m_pObjCubeVeffer;
		CTransform* m_pObjCubeTransform;
		CCube* pObj = nullptr;

		for (int i = 0; i < m_Obj_ClickCount.Cube; i++)
		{
			m_pObjCubeVeffer = (CVIBuffer*)pGameInstance->Get_Component(LEVEL_TERRAIN, L"Layer_Cube", TEXT("Com_VIBuffer"), i);
			m_pObjCubeTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TERRAIN, L"Layer_Cube", TEXT("Com_Transform"), i);
			if (pGameInstance->Picking(m_pObjCubeTransform, m_pObjCubeVeffer, &vtrColPos))
			{
				pObj = static_cast<CCube*>(pGameInstance->Get_GameObject(LEVEL_TERRAIN, TEXT("Layer_Cube"), i));
				pObj->Set_ShaderColor(11);
				_vector Pos = m_pObjCubeTransform->Get_State(CTransform::STATE_POSITION);
				_float3 tempPos;
				XMStoreFloat3(&tempPos, Pos);
				m_vNavigation.push_back(tempPos);

				ImGui::Text("Ok");
			}
		}
	}

	if (ImGui::Begin("Navigation"))
	{
		ImGui::Checkbox("Navigation", &isNavigation);

		if (ImGui::Button("save"))
		{

			_ulong		dwByte = 0;
			HANDLE		hFile = nullptr;
			if (m_sTable == "Level_Boss1_Map")
				hFile = CreateFile(TEXT("../../Client/Bin/Data/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			else if (m_sTable == "Level_Boss2_Map")
				hFile = CreateFile(TEXT("../../Client/Bin/Data/Navigation2.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			if (0 == hFile)
				return E_FAIL;
			_float3			vPoints[3];
			for (int i = 0; i < m_vNavigation.size(); i += 3)
			{

				vPoints[0] = m_vNavigation[i];
				vPoints[1] = m_vNavigation[i + 1];
				vPoints[2] = m_vNavigation[i + 2];

				WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
			}


			CloseHandle(hFile);
		}

	}
	ImGui::End();
	Safe_Release(pGameInstance);
}

void CTerrainManager::CCWCheck(_float3 _MeshPos)
{
	_float3 _sour = m_vNavigation[m_vNavigation.size() - 2];
	_float3 _dest = m_vNavigation[m_vNavigation.size() - 1];

	_float3 _temp;
	int iIscheck = (_dest.x - _sour.x)*(_MeshPos.z - _sour.z) - (_MeshPos.x - _sour.x)*(_dest.z - _sour.z);

	if (iIscheck > 0)
	{
		_temp = m_vNavigation[m_vNavigation.size() - 1];
		m_vNavigation[m_vNavigation.size() - 1] = m_vNavigation[m_vNavigation.size() - 2];
		m_vNavigation[m_vNavigation.size() - 2] = _temp;
	}

}

HRESULT CTerrainManager::DBConnection()
{
	int rc;
	char *err_msg = 0;

	rc = sqlite3_open("../../Reference/DataBase/SpongeBob_jhBaik_DB.db", &m_db);
	if (rc)
		sqlite3_close(m_db);
	else
	{
		string sqlTemp = "create table if not exists " + m_sTable + "(ToolLevel integer, ClientLevel integer, Obj text, LayerTag text, PrototypeTag text, Index_Num integer,Number integer,\
				 Pos_x integer, Pos_y integer, Pos_z integer, Scale_x integer, Scale_y integer, Scale_z integer, , Col_Pos_x, Col_Pos_y, Col_Pos_z, Col_Scale_x, Col_Scale_y,@Col_Scale_z,\
				 TextureInfo text, pObj_Kind text, RotationAxist_x integer, RotationAxist_y integer, RotationAxist_z integer, Angle integer)";//일단 프라이머리키 없애고하기

		const char* sql = sqlTemp.c_str();

		rc = sqlite3_exec(m_db, sql, 0, 0, &err_msg);
	}

	if (rc != SQLITE_OK)
	{
		MSG_BOX("DB error");
		sqlite3_free(err_msg);
		sqlite3_close(m_db);

		return E_FAIL;
	}
	else
		m_IsConnection = true;
	return S_OK;
}

HRESULT CTerrainManager::SelectTable()
{
	static _int m_Table = 0;

	if (ImGui::Begin("Data Management"))
	{
		int rc = 0;
		char *err_msg = 0;
		sqlite3_stmt *res = nullptr;

		static vector<string> m_vecTableListTemp;

		static _int m_TableCount = 0;
		static _bool isEnd = false;


		if (!isEnd)
		{
			const char* sql = "SELECT name FROM sqlite_master WHERE type IN ('table', 'view') AND name NOT LIKE 'sqlite_%' UNION ALL SELECT name FROM sqlite_temp_master WHERE type IN ('table', 'view') ORDER BY 1";
			rc = sqlite3_prepare_v2(m_db, sql, -1, &res, NULL);

			while (sqlite3_step(res) == SQLITE_ROW)
			{
				string pTableName = (char*)sqlite3_column_text(res, 0);
				m_vecTableListTemp.push_back(pTableName);
			}
			isEnd = true;
		}

		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Select Table");
		if (ImGui::BeginListBox("Table List", ImVec2(-FLT_MIN, 4 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int i = 0; i < m_vecTableListTemp.size(); i++)
			{
				const bool is_selected = (m_TableCount == i);
				if (ImGui::Selectable(m_vecTableListTemp[i].c_str(), is_selected))
					m_TableCount = i;
			}
		}
		ImGui::EndListBox();

		if (ImGui::Button("Select", ImVec2(70, 25)))
		{

			if (m_vecTableListTemp.size() == 0)
				m_vecTableListTemp.push_back("Level_Boss1_Map");

			m_sTable = m_vecTableListTemp[m_TableCount];

			string sqlTemp = "create table if not exists " + m_sTable + "(ToolLevel integer, ClientLevel integer, Obj text, LayerTag text, PrototypeTag text, Index_Num integer,Number integer,\
				 Pos_x integer, Pos_y integer, Pos_z integer, Scale_x integer, Scale_y integer, Scale_z integer, Col_Pos_x, Col_Pos_y, Col_Pos_z, Col_Scale_x, Col_Scale_y,Col_Scale_z,\
				 TextureInfo text, pObj_Kind text, RotationAxist_x integer, RotationAxist_y integer, RotationAxist_z integer, Angle integer)";//일단 프라이머리키 없애고하기
			const char* sql = sqlTemp.c_str();

			rc = sqlite3_exec(m_db, sql, 0, 0, &err_msg);
		}
		ImGui::SameLine();
	}
	ImGui::End();

	return S_OK;
}

HRESULT CTerrainManager::EditObj()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;
	Safe_AddRef(pGameInstance);

	if (m_IsEdit)
	{
		static _float3 m_fCntlPos = _float3(0.f, 0.f, 0.f);
		static _float m_Position[4] = { m_fCntlPos.x, m_fCntlPos.y, m_fCntlPos.z, 1 };
		static _float m_Scale[4] = { 1.f, 1.f, 1.f, 1 };

		static _float m_Rotation = 0.f;
		static _int m_iSelectRotationAxist = 0;
		static _vector m_iRotationAxist = {};
		static _bool m_ApplyRotation = false;
		static _float m_TotalAngle = 0.f;

		int rc;
		char *err_msg = 0;
		sqlite3_stmt *res;

		string sqlTemp = "select Index_Num from " + m_sTable + " where Obj = @_objkind and  Index_Num = @Index_Num";
		const char* sql = sqlTemp.c_str();

		rc = sqlite3_prepare_v2(m_db, sql, -1, &res, 0);
		int index = sqlite3_bind_parameter_index(res, "@_objkind");//몇번째 칼럼인지 찾아서
		_bstr_t temp1(objInfo.pObj);
		const char* pObj = temp1;
		sqlite3_bind_text(res, index, pObj, -1, SQLITE_TRANSIENT);//몇번쨰 칼럼안에있는녀석  obj검색


		index = sqlite3_bind_parameter_index(res, "@Index_Num");
		sqlite3_bind_int(res, index, m_iObj_current);

		_uint SelecteNum = 0;
		if (sqlite3_step(res) == SQLITE_ROW)
			SelecteNum = sqlite3_column_int(res, 0);
		else
			MSG_BOX("select Select");
		_tchar* get_obj;

		get_obj = (_tchar*)objInfo.pLayerTag;

		CTransform*	 pTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TERRAIN, get_obj, TEXT("Com_Transform"), SelecteNum);
		if (m_bIsFirst)
		{
			_fvector m_vInitialPos = pTransform->Get_State(CTransform::STATE_POSITION);
			XMStoreFloat3(&m_fCntlPos, m_vInitialPos);
			m_Position[0] = m_fCntlPos.x;
			m_Position[1] = m_fCntlPos.y;
			m_Position[2] = m_fCntlPos.z;
			_float3 m_fInitialScale = pTransform->Get_Scaled();
			m_Scale[0] = m_fInitialScale.x;
			m_Scale[1] = m_fInitialScale.y;
			m_Scale[2] = m_fInitialScale.z;
			_float m_fInitialm_fAngle = pTransform->Get_Angle();
			m_iRotationAxist = pTransform->Get_RotationAxist();

			m_Rotation = m_fInitialm_fAngle;
			m_TotalAngle = m_Rotation;
			m_bIsFirst = false;
		}

		if (ImGui::Begin("OBJ Edit"))
		{	//Position
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Position");
			ImGui::SliderFloat3("pos : X Y Z", m_Position, 0.0f, 100.0f);
			ImGui::InputFloat("pos XAxis", &m_Position[0]);
			ImGui::InputFloat("pos YAxis", &m_Position[1]);
			ImGui::InputFloat("pos ZAxis", &m_Position[2]);
			ImGui::Separator();
			//Scale
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Scale");
			ImGui::SliderFloat3("scale : X Y Z", m_Scale, 1.0f, 10.0f);
			ImGui::InputFloat("scale XAxis", &m_Scale[0]);
			ImGui::InputFloat("scale YAxis", &m_Scale[1]);
			ImGui::InputFloat("scale ZAxis", &m_Scale[2]);
			ImGui::Separator();
			//Rotation
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Rotation");
			ImGui::Checkbox("Appply Rotation", &m_ApplyRotation);
			if (m_ApplyRotation) {
				ImGui::RadioButton("ROLL", &m_iSelectRotationAxist, ROLL); ImGui::SameLine();
				ImGui::RadioButton("PITCH", &m_iSelectRotationAxist, PITCH); ImGui::SameLine();
				ImGui::RadioButton("YAW", &m_iSelectRotationAxist, YAW);
				ImGui::SliderFloat("Rotation", &m_Rotation, -0.5f, 0.5f);
				ImGui::InputFloat("Rotation Value", &m_Rotation);

				if (m_iSelectRotationAxist == ROLL)
					m_iRotationAxist = XMLoadFloat3(&_float3(1.f, 0.f, 0.f));
				else if (m_iSelectRotationAxist == PITCH)
					m_iRotationAxist = XMLoadFloat3(&_float3(0.f, 1.f, 0.f));
				else //YAW
					m_iRotationAxist = XMLoadFloat3(&_float3(0.f, 0.f, 1.f));

				if (ImGui::Button("Angle OK", ImVec2(338, 30))) {
					m_TotalAngle += m_Rotation;
					pTransform->Rotation(m_iRotationAxist, m_Rotation);
				}

				if (ImGui::Button("Angle -0.1f", ImVec2(165, 30)))
				{
					m_TotalAngle += -0.1;
					pTransform->Rotation(m_iRotationAxist, -0.1);
				}
				ImGui::SameLine();
				if (ImGui::Button("Angle +0.1f", ImVec2(165, 30)))
				{
					m_TotalAngle += 0.1;
					pTransform->Rotation(m_iRotationAxist, 0.1);
				}

				if (ImGui::Button("Angle Reset", ImVec2(338, 30)))
				{
					pTransform->Rotation(m_iRotationAxist, m_TotalAngle * -1);
					m_TotalAngle = 0.f;
				}
			}

			pTransform->Set_Scaled(_float3(m_Scale[0], m_Scale[1], m_Scale[2]));
			pTransform->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_Position[0], m_Position[1], m_Position[2], 0.5f));



			ImGui::Separator();
			if (ImGui::Button("OK", ImVec2(338, 30)))
			{
				m_ApplyRotation = false;
				m_IsEdit = false;

				int rc;
				char *err_msg = 0;
				sqlite3_stmt *res;
				string sqlTemp = "Update " + m_sTable + " set Pos_x = @Pos_x, Pos_y = @Pos_y, Pos_z = @Pos_z,\
                                                 Scale_x = @Scale_x, Scale_y = @Scale_y, Scale_z = @Scale_z,\
								              	RotationAxist_x = @RotationAxist_x,  RotationAxist_y = @RotationAxist_y, RotationAxist_z = @RotationAxist_z, \
                                                Angle = @Angle where Obj = @_objkind and Index_Num = @Index_Num";
				const char* sql = sqlTemp.c_str();

				rc = sqlite3_prepare_v2(m_db, sql, -1, &res, 0);

				int index = sqlite3_bind_parameter_index(res, "@_objkind");
				_bstr_t temp1(objInfo.pObj);
				const char* pObj = temp1;
				sqlite3_bind_text(res, index, pObj, -1, SQLITE_TRANSIENT);

				index = sqlite3_bind_parameter_index(res, "@Pos_x");
				sqlite3_bind_double(res, index, m_Position[0]);
				index = sqlite3_bind_parameter_index(res, "@Pos_y");
				sqlite3_bind_double(res, index, m_Position[1]);
				index = sqlite3_bind_parameter_index(res, "@Pos_z");
				sqlite3_bind_double(res, index, m_Position[2]);

				index = sqlite3_bind_parameter_index(res, "@Scale_x");
				sqlite3_bind_double(res, index, m_Scale[0]);
				index = sqlite3_bind_parameter_index(res, "@Scale_y");
				sqlite3_bind_double(res, index, m_Scale[1]);
				index = sqlite3_bind_parameter_index(res, "@Scale_z");
				sqlite3_bind_double(res, index, m_Scale[2]);

				_float3 AxistTemp{};
				XMStoreFloat3(&AxistTemp, m_iRotationAxist);
				index = sqlite3_bind_parameter_index(res, "@RotationAxist_x");
				sqlite3_bind_double(res, index, AxistTemp.x);
				index = sqlite3_bind_parameter_index(res, "@RotationAxist_y");
				sqlite3_bind_double(res, index, AxistTemp.y);
				index = sqlite3_bind_parameter_index(res, "@RotationAxist_z");
				sqlite3_bind_double(res, index, AxistTemp.z);

				index = sqlite3_bind_parameter_index(res, "@Angle");
				sqlite3_bind_double(res, index, m_TotalAngle);

				index = sqlite3_bind_parameter_index(res, "@Index_Num");
				sqlite3_bind_int(res, index, m_iObj_current);

				int step = sqlite3_step(res);

				if (rc != SQLITE_OK)
				{
					MSG_BOX("Update error");
					sqlite3_free(err_msg);
					return E_FAIL;
				}

				for (int i = 0; i <= 2; i++)
				{
					m_Position[i] = 0;
					m_Scale[i] = 1;
					m_iRotationAxist = {};
				}
				m_Rotation = 0;
			}
		}
		ImGui::End();
	}

	if (ImGui::Begin("TerrainControl"))
	{
		static _float3 m_fCntlPos = _float3(0.f, 0.f, 0.f);

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "floor : %d ", (_uint)m_fCntlPos.y);



		if (ImGui::Button("Y - ", ImVec2(50, 25)))
		{
			_fvector m_fpos = m_pTerrainTransform->Get_State(CTransform::STATE_POSITION);
			XMStoreFloat3(&m_fCntlPos, m_fpos);
			m_fCntlPos.y--;
			m_pTerrainTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fCntlPos.x, m_fCntlPos.y, m_fCntlPos.z, 1.f));
		}
		ImGui::SameLine();
		if (ImGui::Button("Y + ", ImVec2(50, 25)))
		{
			_fvector m_fpos = m_pTerrainTransform->Get_State(CTransform::STATE_POSITION);
			XMStoreFloat3(&m_fCntlPos, m_fpos);
			m_fCntlPos.y++;
			m_pTerrainTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fCntlPos.x, m_fCntlPos.y, m_fCntlPos.z, 1.f));
		}
		if (ImGui::Button("X - ", ImVec2(50, 25)))
		{
			_fvector m_fpos = m_pTerrainTransform->Get_State(CTransform::STATE_POSITION);
			XMStoreFloat3(&m_fCntlPos, m_fpos);
			m_fCntlPos.x--;
			m_pTerrainTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fCntlPos.x, m_fCntlPos.y, m_fCntlPos.z, 1.f));
		}
		ImGui::SameLine();
		if (ImGui::Button("X + ", ImVec2(50, 25)))
		{
			_fvector m_fpos = m_pTerrainTransform->Get_State(CTransform::STATE_POSITION);
			XMStoreFloat3(&m_fCntlPos, m_fpos);
			m_fCntlPos.x++;
			m_pTerrainTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fCntlPos.x, m_fCntlPos.y, m_fCntlPos.z, 1.f));
		}
		if (ImGui::Button("Z - ", ImVec2(50, 25)))
		{
			_fvector m_fpos = m_pTerrainTransform->Get_State(CTransform::STATE_POSITION);
			XMStoreFloat3(&m_fCntlPos, m_fpos);
			m_fCntlPos.z--;
			m_pTerrainTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fCntlPos.x, m_fCntlPos.y, m_fCntlPos.z, 1.f));
		}
		ImGui::SameLine();
		if (ImGui::Button("Z + ", ImVec2(50, 25)))
		{
			_fvector m_fpos = m_pTerrainTransform->Get_State(CTransform::STATE_POSITION);
			XMStoreFloat3(&m_fCntlPos, m_fpos);
			m_fCntlPos.z++;
			m_pTerrainTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fCntlPos.x, m_fCntlPos.y, m_fCntlPos.z, 1.f));
		}
	}
	ImGui::End();

	Safe_Release(pGameInstance);
	return S_OK;
}

vector<string> CTerrainManager::ChangeListInfo(const _tchar * _obj)
{
	enum {OBJ = 2, INDEX = 5};
	vector<string> vSetInfo;
	int rc;
	char *pErr_msg = 0;
	sqlite3_stmt *pRes;

	_bstr_t bstrObj(_obj);
	const char* pObj = bstrObj;
	string strSql = "select * from " + m_sTable + "  where Obj like  @ObjName||'%'";
	const char* pSql = strSql.c_str();

	rc = sqlite3_prepare_v2(m_db, pSql, -1, &pRes, 0);
	int iIndex = sqlite3_bind_parameter_index(pRes, "@ObjName");
	sqlite3_bind_text(pRes, iIndex, pObj, -1, SQLITE_TRANSIENT);

	while (sqlite3_step(pRes) == SQLITE_ROW){
		pObj = (char*)sqlite3_column_text(pRes, OBJ);
		_uint iIndex= sqlite3_column_int(pRes, INDEX);

		string strObj = (string)pObj;
		string strIndex = to_string(iIndex);

		strObj.append(strIndex, 0, strIndex.size());
		vSetInfo.push_back(strObj);
	}

	if (rc != SQLITE_OK){
		MSG_BOX("select error");
		sqlite3_free(pErr_msg);
	}
	sqlite3_finalize(pRes);
	return vSetInfo;
}


HRESULT CTerrainManager::Load()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;
	Safe_AddRef(pGameInstance);


	if (ImGui::Begin("Create Table"))
	{
		static char strTable[64]{};
		char *err_msg = 0;



		_int iRc;
		ImGui::InputText("TableName", strTable, IM_ARRAYSIZE(strTable));
		if (ImGui::Button("Create", ImVec2(70, 25)))
		{
			string strGetTable = "";
			strGetTable = (string)strTable;
			string sqlTemp = "create table if not exists " + strGetTable + "(ToolLevel integer, ClientLevel integer, Obj text, LayerTag text, PrototypeTag text, Index_Num integer, Number integer,\
				                               Pos_x integer, Pos_y integer, Pos_z integer, Scale_x integer, Scale_y integer, Scale_z integer, Col_Pos_x integer, Col_Pos_y integer, Col_Pos_z integer, \
                                               pObj_Kind text, RotationAxist_x integer, RotationAxist_y integer, RotationAxist_z integer, Angle integer)";
			const char* strSql = sqlTemp.c_str();
			iRc = sqlite3_exec(m_db, strSql, 0, 0, &err_msg);
			if (SQLITE_OK != iRc)
				E_FAIL;
		}
	}
	ImGui::End();

	if (ImGui::Begin("Data Management"))
	{
		if (ImGui::Button("Creat", ImVec2(70, 25)))
		{
			//먼저 싹 지우기
			for (auto iter = m_setObjName.begin(); iter != m_setObjName.end(); iter++)
			{
				if (FAILED(pGameInstance->Distroy_GameObject(LEVEL_TERRAIN, *(iter), true)))
					MSG_BOX("Failed Distroy");
			}

			ZeroMemory(&m_Obj_ClickCount, sizeof(m_Obj_ClickCount));
			m_vecStrListTemp = ChangeListInfo(objInfo.pObj);

			m_bIsSelect = true;

			int rc;
			sqlite3_stmt *res3;

			//string sqlTemp = "select * from " + m_sTable;
			string sqlTemp = "select * from " + m_sTable + " where obj Not like 'fish%'";
			const char* sql = sqlTemp.c_str();


			rc = sqlite3_prepare_v2(m_db, sql, -1, &res3, 0);
			enum{TOOL_LEVEL, CLIENT_LEVEL, INDEX, POS_X, POS_Y,POS_Z, SCALE_X};
			while (sqlite3_step(res3) == SQLITE_ROW)
			{
				objInfo.toolLevel = (LEVEL)sqlite3_column_int(res3, 0);
				objInfo.clientLevel = (OBJ_CLIENTLEVELE)sqlite3_column_int(res3, 1);
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

				char			temp[MAX_PATH] = "";
				strcpy_s(temp, (char*)sqlite3_column_text(res3, 2));

				if (strcmp(temp, "Cube") == 0)
				{
					objInfo.pObj = L"Cube";
					objInfo.pObj_Kind = L"Cube";
					objInfo.pLayerTag = L"Layer_Cube";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Cube";
				}
				else if (strcmp(temp, "SpongeBob") == 0)
				{
					objInfo.pObj = L"SpongeBob";
					objInfo.pObj_Kind = L"Mesh";

					objInfo.pLayerTag = L"Layer_SpongeBob";
					objInfo.pPrototypeTag = L"Prototype_GameObject_SpongeBob";
				}
				else if (strcmp(temp, "Ground") == 0)
				{
					objInfo.pObj = L"Ground";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Ground";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Ground";
				}
				else if (strcmp(temp, "Ground2") == 0)
				{
					objInfo.pObj = L"Ground2";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Ground2";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Ground2";
				}
				else if (strcmp(temp, "Ring_Top") == 0)
				{
					objInfo.pObj = L"Ring_Top";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Ring_Top";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Ring_Top";
				}
				else if (strcmp(temp, "Rope_Top") == 0)
				{
					objInfo.pObj = L"Rope_Top";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Rope_Top";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Rope_Top";
				}
				else if (strcmp(temp, "Ring_Pillar") == 0)
				{
					objInfo.pObj = L"Ring_Pillar";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Ring_Pillar";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Ring_Pillar";
				}
				else if (strcmp(temp, "Light_Tube") == 0)
				{
					objInfo.pObj = L"Light_Tube";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Light_Tube";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Light_Tube";
				}
				else if (strcmp(temp, "Scoreboard") == 0)
				{
					objInfo.pObj = L"Scoreboard";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Scoreboard";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Scoreboard";
				}
				else if (strcmp(temp, "Light_Cone_Pink") == 0)
				{
					objInfo.pObj = L"Light_Cone_Pink";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Light_Cone_Pink";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Light_Cone_Pink";
				}
				else if (strcmp(temp, "Light_Cone_Blue") == 0)
				{
					objInfo.pObj = L"Light_Cone_Blue";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Light_Cone_Blue";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Light_Cone_Blue";
				}
				else if (strcmp(temp, "Light_Cone_Green") == 0)
				{
					objInfo.pObj = L"Light_Cone_Green";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Light_Cone_Green";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Light_Cone_Green";
				}
				else if (strcmp(temp, "Bubble") == 0)
				{
					objInfo.pObj = L"Bubble";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Bubble";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Bubble";
				}
				else if (strcmp(temp, "Mesh_noanimnspongebob") == 0)
				{
					objInfo.pObj = L"Mesh_noanimnspongebob";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Mesh_noanimnspongebob";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Mesh_noanimnspongebob";
				}
				else if (strcmp(temp, "Mesh_camera") == 0)
				{
					objInfo.pObj = L"Mesh_camera";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Mesh_camera";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Mesh_camera";
				}
				else if (strcmp(temp, "Mesh_noanimnking") == 0)
				{
					objInfo.pObj = L"Mesh_noanimnking";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Mesh_noanimnking";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Mesh_noanimnking";
				}
				else if (strcmp(temp, "Mesh_noanimnsandy") == 0)
				{
					objInfo.pObj = L"Mesh_noanimnsandy";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Mesh_noanimnsandy";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Mesh_noanimnsandy";
				}
				else if (strcmp(temp, "Mesh_noanimpatrick") == 0)
				{
					objInfo.pObj = L"Mesh_noanimpatrick";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Mesh_noanimpatrick";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Mesh_noanimnpatrick";
				}
				else if (strcmp(temp, "Big_Pillar") == 0)
				{
					objInfo.pObj = L"Big_Pillar";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Big_Pillar";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Big_Pillar";
				}
				else if (strcmp(temp, "Big_Pillar_Attachment") == 0)
				{
					objInfo.pObj = L"Big_Pillar_Attachment";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Big_Pillar_Attachment";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Big_Pillar_Attachment";
				}
				else if (strcmp(temp, "Big_Cloth") == 0)
				{
					objInfo.pObj = L"Big_Cloth";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Big_Cloth";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Big_Cloth";
				}
				else if (strcmp(temp, "Pos_Light") == 0)
				{
					objInfo.pObj = L"Pos_Light";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Pos_Light";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Pos_Light";
				}
				else if (strcmp(temp, "Stairs_02") == 0)
				{
					objInfo.pObj = L"Stairs_02";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Stairs_02";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Stairs_02";
				}
				else if (strcmp(temp, "Stairs_04") == 0)
				{
					objInfo.pObj = L"Stairs_04";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Stairs_04";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Stairs_04";
				}
				else if (strcmp(temp, "SM_PD_Banner_01") == 0)
				{
					objInfo.pObj = L"SM_PD_Banner_01";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_SM_PD_Banner_01";
					objInfo.pPrototypeTag = L"Prototype_GameObject_SM_PD_Banner_01";
				}
				else if (strcmp(temp, "Poster") == 0)
				{
					objInfo.pObj = L"Poster";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Poster";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Poster";
				}
				else if (strcmp(temp, "Poster2") == 0)
				{
					objInfo.pObj = L"Poster2";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Poster2";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Poster2";
				}
				else if (strcmp(temp, "Mesh_Squidward") == 0)
				{
					objInfo.pObj = L"Mesh_Squidward";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Squidward";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Squidward";
				}
				else if (strcmp(temp, "Mesh_Man") == 0)
				{
					objInfo.pObj = L"Mesh_Man";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Man";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Man";
				}
				else if (strcmp(temp, "Mesh_Tree") == 0)
				{
					objInfo.pObj = L"Mesh_Tree";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Tree";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Tree";
				}
				else if (strcmp(temp, "Scoreboard_Ropes") == 0)
				{
					objInfo.pObj = L"Scoreboard_Ropes";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Scoreboard_Ropes";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Scoreboard_Ropes";
				}
				else if (strcmp(temp, "Mesh_scoreboard_broken") == 0)
				{
					objInfo.pObj = L"Mesh_scoreboard_broken";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Mesh_scoreboard_broken";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Mesh_scoreboard_broken";
				}
				else if (strcmp(temp, "Mesh_light_cone_yellow") == 0)
				{
					objInfo.pObj = L"Mesh_light_cone_yellow";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Mesh_light_cone_yellow";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Light_Cone_Yellow";
				}
				else if (strcmp(temp, "Mesh_light_cone_blue2") == 0)
				{
					objInfo.pObj = L"Mesh_light_cone_blue2";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Mesh_light_cone_blue2";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Light_Cone_Blue2";
				}
				else if (strcmp(temp, "Mesh_field") == 0)
				{
					objInfo.pObj = L"Mesh_field";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Mesh_field";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Field";
				}
				else if (strcmp(temp, "Mesh_TaxiStop") == 0)
				{
					objInfo.pObj = L"Mesh_TaxiStop";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_TaxiStop";
					objInfo.pPrototypeTag = L"Prototype_GameObject_TaxiStop";
				}
				else if (strcmp(temp, "Mesh_Gate") == 0)
				{
					objInfo.pObj = L"Mesh_Gate";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Gate";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Gate";
				}
				else if (strcmp(temp, "Mesh_Robo") == 0)
				{
					objInfo.pObj = L"Mesh_Robo";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Robo";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Robo";
				}
				else if (strcmp(temp, "Mesh_Robo2") == 0)
				{
					objInfo.pObj = L"Mesh_Robo2";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Robo2";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Robo2";
				}
				else if (strcmp(temp, "Mesh_Robo3") == 0)
				{
					objInfo.pObj = L"Mesh_Robo3";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Robo3";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Robo3";
				}
				else if (strcmp(temp, "Mesh_Jelly_Pink") == 0)
				{
					objInfo.pObj = L"Mesh_Jelly_Pink";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Jelly_Pink";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Jelly_Pink";
				}
				else if (strcmp(temp, "Mesh_Jelly_Blue") == 0)
				{
					objInfo.pObj = L"Mesh_Jelly_Blue";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Jelly_Blue";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Jelly_Blue";
				}
				else if (strcmp(temp, "Mesh_Grass") == 0)
				{
					objInfo.pObj = L"Mesh_Grass";
					objInfo.pObj_Kind = L"Mesh";
					objInfo.pLayerTag = L"Layer_Grass";
					objInfo.pPrototypeTag = L"Prototype_GameObject_Grass";
				}
				else
					continue;

				char * temp1 = (char*)sqlite3_column_text(res3, 4);
				MultiByteToWideChar(CP_ACP, 0, temp1, strlen(temp1) + 1, pTextureInfo, 256);
				objInfo.pTextureInfo = pTextureInfo;


				if (FAILED(pGameInstance->Clone_GameObject(LEVEL_TERRAIN, (_tchar*)objInfo.pLayerTag, objInfo.pPrototypeTag, &objInfo)))
					return E_FAIL;

				m_setObjName.emplace(objInfo.pLayerTag);

				ClickCount(true);
				string str = (string)temp;
				string string_temp = to_string(objInfo.iIndex);

				str.append(string_temp, 0, string_temp.size());
				std::wstring widestr = std::wstring(str.begin(), str.end());
				const wchar_t* widecstr = widestr.c_str();
				m_vecStrListTemp.push_back(str);
			}
			if (rc != SQLITE_OK)
			{
				MSG_BOX("SQL error");
				return E_FAIL;
			}

			_ulong		dwByte = 0;
			HANDLE		hFile = nullptr;
			if (m_sTable == "Level_Boss1_Map")
				hFile = CreateFile(TEXT("../../Client/Bin/Data/Navigation.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			else if (m_sTable == "Level_Boss2_Map")
				hFile = CreateFile(TEXT("../../Client/Bin/Data/Navigation2.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

			if (0 == hFile)
				return E_FAIL;

			while (true)
			{
				_float3		vPoints[3];

				ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
				if (0 == dwByte)
					break;

				m_vNavigation.push_back(vPoints[0]);
				m_vNavigation.push_back(vPoints[1]);
				m_vNavigation.push_back(vPoints[2]);
			}
			CloseHandle(hFile);
		}
	}
	ImGui::End();

	Safe_Release(pGameInstance);
	return S_OK;
}



HRESULT CTerrainManager::Create()
{
	if (ImGui::Begin("Obj List"))
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		if (nullptr == pGameInstance)
			return E_FAIL;
		Safe_AddRef(pGameInstance);

		string   pObjTag = "";
		ImGui::SameLine();
		if (ImGui::Button("Create") || isPicking)
		{
			m_bIsSelect = true;
			m_Obj_ClickCount.isCreate = true;

			objInfo.fPos = _float3(0, 0, 0);
			objInfo.fScale = _float3(0, 0, 0);
			objInfo.fAxist = _float3(0, 0, 0);
			objInfo.fRotation = 0.f;

			ClickCount(m_Obj_ClickCount.isCreate);

			string string_temp = "";

			pObjTag.append(string_temp, 0, string_temp.size());
			int rc;
			char *err_msg = 0;
			sqlite3_stmt *res2;

			//quotesql()
			if (isPicking)
			{
				objInfo.fPos = _float3(vPos.x, vPos.y, vPos.z);
				isPicking = false;
			}


			if (isNavigation)
			{
				_float3 vtrColPostest;

				int rc;
				char *err_msg = 0;
				sqlite3_stmt *res;
				string sqlTemp;
				TCHAR* temp_tchar{};

				if (m_sTable == "Level_Boss1_Map")
				{
					sqlTemp = "select * from " + m_sTable + " where LayerTag  = 'Layer_Ring_Top' ";
					temp_tchar = L"Layer_Ring_Top";
				}
				else if (m_sTable == "Level_Boss2_Map")
				{
					sqlTemp = "select * from " + m_sTable + " where LayerTag  = 'Layer_Mesh_field' ";
					temp_tchar = L"Layer_Mesh_field";
				}

				const char* sql = sqlTemp.c_str();

				rc = sqlite3_prepare_v2(m_db, sql, -1, &res, 0);

				while (sqlite3_step(res) == SQLITE_ROW)
				{
					char			temp[MAX_PATH] = "";
					strcpy_s(temp, (char*)sqlite3_column_text(res, 3));

					_int Number = sqlite3_column_int(res, 6);

					CModel* m_pObjnVeffer = (CModel*)pGameInstance->Get_Component(LEVEL_TERRAIN, temp_tchar, TEXT("Com_VIBuffer"), Number);
					if (nullptr == m_pObjnVeffer)
						return E_FAIL;

					CTransform* m_pObjTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TERRAIN, temp_tchar, TEXT("Com_Transform"), Number);
					if (nullptr == m_pObjTransform)
						return E_FAIL;


					if (pGameInstance->Picking(m_pObjTransform, (CVIBuffer*)m_pObjnVeffer->GetMeshContainers().front(), &vtrColPostest))
					{
						_float3 vMeshPos = vtrColPostest;

						objInfo.fPos = _float3(vMeshPos.x, vMeshPos.y, vMeshPos.z);

						if (m_vNavigation.size() > 3)
							CCWCheck(vMeshPos);


						m_vNavigation.push_back(vMeshPos);

						m_pObjNavigation = (CNavigation*)pGameInstance->Get_Component(LEVEL_TERRAIN, temp_tchar, TEXT("Com_Navigation"), Number);
						if (nullptr == m_pObjTransform)
							return E_FAIL;

						if (m_vNavigation.size() % 3 == 0)
						{
							_float3		vPoints[3];

							vPoints[0] = m_vNavigation[m_vNavigation.size() - 1 - 2];
							vPoints[1] = m_vNavigation[m_vNavigation.size() - 1 - 1];
							vPoints[2] = m_vNavigation[m_vNavigation.size() - 1];

							m_pObjNavigation->Set_CellsPush(vPoints);
						}
					}
				}
			}

			if (FAILED(pGameInstance->Clone_GameObject(LEVEL_TERRAIN, objInfo.pLayerTag, objInfo.pPrototypeTag, &objInfo)))
				return E_FAIL;
			string sqlTemp = "INSERT INTO " + m_sTable + "  VALUES (@ToolLevel, @ClientLevel, @Obj, @LayerTag, @PrototypeTag, @Index_Num, @Number,\
								                                     @Pos_x, @Pos_y, @Pos_z, @Scale_x, @Scale_y,@Scale_z, @Col_Pos_x, @Col_Pos_y, @Col_Pos_z, @Col_Scale_x,\
                                                                     @Col_Scale_y,@Col_Scale_z, @TextureInfo, @Obj_Kind, @RotationAxist_x, @RotationAxist_y,@RotationAxist_z, @Angle)";

			const char* sql2 = sqlTemp.c_str();

			rc = sqlite3_prepare_v2(m_db, sql2, -1, &res2, NULL);
			if (SQLITE_OK == rc) {
				int index = sqlite3_bind_parameter_index(res2, "@ToolLevel");
				sqlite3_bind_int(res2, index, (_int)objInfo.toolLevel);

				index = sqlite3_bind_parameter_index(res2, "@ClientLevel");
				sqlite3_bind_int(res2, index, (_int)objInfo.clientLevel);

				index = sqlite3_bind_parameter_index(res2, "@Obj");
				_bstr_t temp1(objInfo.pObj);
				const char* pObj = temp1;
				sqlite3_bind_text(res2, index, pObj, -1, SQLITE_TRANSIENT);

				index = sqlite3_bind_parameter_index(res2, "@Obj_Kind");
				_bstr_t temp4(objInfo.pObj_Kind);
				const char* pObj_Kind = temp4;
				sqlite3_bind_text(res2, index, pObj_Kind, -1, SQLITE_TRANSIENT);

				_bstr_t temp2(objInfo.pLayerTag);
				const char* pLayerTag = temp2;
				index = sqlite3_bind_parameter_index(res2, "@LayerTag");
				sqlite3_bind_text(res2, index, pLayerTag, -1, SQLITE_TRANSIENT);
				m_setObjName.emplace(temp2);

				_bstr_t temp3(objInfo.pPrototypeTag);
				const char* pPrototypeTag = temp3;
				index = sqlite3_bind_parameter_index(res2, "@PrototypeTag");
				sqlite3_bind_text(res2, index, pPrototypeTag, -1, SQLITE_TRANSIENT);

				index = sqlite3_bind_parameter_index(res2, "@Index_Num");
				sqlite3_bind_int(res2, index, objInfo.iIndex);

				index = sqlite3_bind_parameter_index(res2, "@Number");
				sqlite3_bind_int(res2, index, objInfo.iNumber);

				index = sqlite3_bind_parameter_index(res2, "@Pos_x");
				sqlite3_bind_double(res2, index, objInfo.fPos.x);
				index = sqlite3_bind_parameter_index(res2, "@Pos_y");
				sqlite3_bind_double(res2, index, objInfo.fPos.y);
				index = sqlite3_bind_parameter_index(res2, "@Pos_z");
				sqlite3_bind_double(res2, index, objInfo.fPos.z);

				index = sqlite3_bind_parameter_index(res2, "@Scale_x");
				sqlite3_bind_double(res2, index, objInfo.fScale.x);
				index = sqlite3_bind_parameter_index(res2, "@Scale_y");
				sqlite3_bind_double(res2, index, objInfo.fScale.y);
				index = sqlite3_bind_parameter_index(res2, "@Scale_z");
				sqlite3_bind_double(res2, index, objInfo.fScale.z);

				index = sqlite3_bind_parameter_index(res2, "@RotationAxist_x");
				sqlite3_bind_double(res2, index, objInfo.fAxist.x);
				index = sqlite3_bind_parameter_index(res2, "@RotationAxist_y");
				sqlite3_bind_double(res2, index, objInfo.fAxist.y);
				index = sqlite3_bind_parameter_index(res2, "@RotationAxist_z");
				sqlite3_bind_double(res2, index, objInfo.fAxist.z);

				index = sqlite3_bind_parameter_index(res2, "@Angle");
				sqlite3_bind_double(res2, index, objInfo.fRotation);

				int step = sqlite3_step(res2);
			}
			else {
				MSG_BOX("INSERT error");
				sqlite3_free(err_msg);
				return E_FAIL;
			}

			m_vecStrListTemp = ChangeListInfo(objInfo.pObj);

			if (isNavigation)
			{
				CCube* pCube = nullptr;
				for (int i = 0; i < m_Obj_ClickCount.Cube; i++)
				{
					pCube = static_cast<CCube*>(pGameInstance->Get_GameObject(LEVEL_TERRAIN, TEXT("Layer_Cube"), i));
					pCube->Set_ShaderColor(1);
				}
			}
		}
		ImGui::SameLine();
		Safe_Release(pGameInstance);
	}
	ImGui::End();
	return S_OK;
}

void CTerrainManager::ObjSelect()
{
	if (ImGui::Begin("Obj List"))
	{
		if (ImGui::Button("Select"))
		{
			m_IsEdit = true;
			m_bIsFirst = true;
		}
	}
	ImGui::End();
}

CTerrainManager * CTerrainManager::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, sqlite3* db)
{
	CTerrainManager*		pInstance = new CTerrainManager(pDevice, pContext, db);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Created : CLevel_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrainManager::Free()
{
	__super::Free();
	sqlite3_close(m_db);
}
