#include "..\Public\Level_UI.h"
#include "GameInstance.h"
#include "Underpants.h"
#include <comdef.h> 

CLevel_UI::CLevel_UI(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext), m_pDevice(pDevice), m_pContext(pContext)
{
}

HRESULT CLevel_UI::NativeConstruct()
{
	m_sTable = "Level_Boss1_UI";

	DBConnection();


	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Clone_GameObject()))
		return E_FAIL;

	return S_OK;
}

void CLevel_UI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CLevel_UI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CLevel_UI::Render()
{
	if (ImGui::Begin("Data Management"))
	{
		if (m_IsConnection)
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "DB Connection : Completed");
		else
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "DB Connection : Failed");

	}
	ImGui::End();

	if (ImGui::Begin("Level_UI MousePos"))
	{
		ImVec2  screenMousePos = ImGui::GetMousePosOnOpeningCurrentPopup();
		ImVec2 mainViewPos = ImGui::GetMainViewport()->Pos;
		ImGui::Text("MainViewMousePos_x : %1.f", screenMousePos.x - mainViewPos.x);
		ImGui::Text("MainViewMousePos_y : %1.f", screenMousePos.y - mainViewPos.y - 24);
	}
	ImGui::End();

	SelectTable();
	Create();
	Load();
	return S_OK;
}

HRESULT CLevel_UI::Ready_Prototype_Component()
{
	return S_OK;
}

HRESULT CLevel_UI::Ready_Clone_GameObject()
{
	return S_OK;
}

HRESULT CLevel_UI::DBConnection()
{
	int rc;
	char *err_msg = 0;

	rc = sqlite3_open("../../Reference/DataBase/SpongeBob_jhBaik_DB.db", &db);
	if (rc)
		sqlite3_close(db);
	else
	{

		string sqlTemp = "create table if not exists " + m_sTable + "(ToolLevel integer, ClientLevel integer, Obj text, LayerTag text, PrototypeTag text, Index_Num integer,Number integer,\
				 Pos_x integer, Pos_y integer, Pos_z integer, Scale_x integer, Scale_y integer, Scale_z integer, Col_Pos_x, Col_Pos_y, Col_Pos_z, Col_Scale_x, Col_Scale_y,Col_Scale_z,\
				 TextureInfo text, pObj_Kind text, RotationAxist_x integer, RotationAxist_y integer, RotationAxist_z integer, Angle integer)";//일단 프라이머리키 없애고하기

		const char* sql = sqlTemp.c_str();

		rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
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

HRESULT CLevel_UI::Create()
{
	static _int iSelecte = 0;
	static _int iLevelSelecte = 0;

	ImGui::Combo("Lecvel Pick", &iLevelSelecte, "LEVELE_BOSS1\0LEVELE_BOSS2\0LEVEL_MENU\0");
	ImGui::Combo("UI Pick", &iSelecte, "Underpants\0Boss_HPBar\0Gane_Start\0Exit\0MenuBar\0Energy\0");
	switch (iSelecte)
	{
	case UNDERPANTS:
		m_objInfo.clientLevel = (OBJ_CLIENTLEVELE)iLevelSelecte;
		m_objInfo.pObj = L"Underpants";
		m_objInfo.pLayerTag = L"Layer_Underpants";
		m_objInfo.pPrototypeTag = L"Prototype_GameObject_Underpants";
		m_objInfo.pTextureInfo = 0;

		break;
	case BOSS_HP_BAR:
		m_objInfo.clientLevel = (OBJ_CLIENTLEVELE)iLevelSelecte;
		m_objInfo.pObj = L"Boss_HPBar";
		m_objInfo.pLayerTag = L"Laye_Boss_HPBar";
		m_objInfo.pPrototypeTag = L"Prototype_GameObject_Boss_HPBar";
		if (iLevelSelecte == 0)
			m_objInfo.pTextureInfo = 0;
		else
			m_objInfo.pTextureInfo = 1;

		break;
	case GAME_START:
		m_objInfo.clientLevel = (OBJ_CLIENTLEVELE)iLevelSelecte;
		m_objInfo.pObj = L"Game_Start";
		m_objInfo.pLayerTag = L"Laye_Game_Start";
		m_objInfo.pPrototypeTag = L"Prototype_GameObject_Disable_Geme_Start";
		m_objInfo.pTextureInfo = 0;
		break;
	case EXIT:
		m_objInfo.clientLevel = (OBJ_CLIENTLEVELE)iLevelSelecte;
		m_objInfo.pObj = L"Exit";
		m_objInfo.pLayerTag = L"Laye_Exit";
		m_objInfo.pPrototypeTag = L"Prototype_GameObject_Disable_Exit";
		m_objInfo.pTextureInfo = 0;
		break;
	case MENU_BAR:
		m_objInfo.clientLevel = (OBJ_CLIENTLEVELE)iLevelSelecte;
		m_objInfo.pObj = L"Menu_Bar";
		m_objInfo.pLayerTag = L"Laye_Menu_Bar";
		m_objInfo.pPrototypeTag = L"Prototype_GameObject_MenuBar";
		m_objInfo.pTextureInfo = 0;
		break;
	case ENERGY:
		m_objInfo.clientLevel = (OBJ_CLIENTLEVELE)iLevelSelecte;
		m_objInfo.pObj = L"Eenergy";
		m_objInfo.pLayerTag = L"Laye_Eenergy";
		m_objInfo.pPrototypeTag = L"Prototype_GameObject_Boss_Energy";
		m_objInfo.pTextureInfo = 0;
		break;
	}
	


	if (GetAsyncKeyState('C') & 0x0001)
	{
		POINT ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(g_hWnd, &ptCursor);

		m_objInfo.fPos.x = ptCursor.x;
		m_objInfo.fPos.y = ptCursor.y;

		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		if (nullptr == pGameInstance)
			return E_FAIL;
		Safe_AddRef(pGameInstance);

		m_objInfo.db = db;
		m_objInfo.m_Table = m_sTable;
		if (m_objInfo.pObj == L"Underpants")
		{
			for (int i = 0; i < 4; i++)
			{
				InsertObj();
				if (FAILED(pGameInstance->Clone_GameObject(LEVEL_UI, m_objInfo.pLayerTag, m_objInfo.pPrototypeTag, &m_objInfo)))
					return E_FAIL;
				m_objInfo.fPos.x += 65;
			}
		}
		else if (m_objInfo.pObj == L"Eenergy")
		{
		//	static _uint count = 0;
			//static _uint textureNum = 0;

			for (int i = 0; i < 9; i++)
			{
				InsertObj();
				
				if (i < 3 && i >= 0)
					m_objInfo.pTextureInfo = 0;
				else if (i >= 3 && i < 6)
					m_objInfo.pTextureInfo = 1;
				else
					m_objInfo.pTextureInfo = 2;

				if (FAILED(pGameInstance->Clone_GameObject(LEVEL_UI, m_objInfo.pLayerTag, m_objInfo.pPrototypeTag, &m_objInfo)))
					return E_FAIL;
				m_objInfo.fPos.x += 15;
			}
		}
		else
		{
			if (FAILED(pGameInstance->Clone_GameObject(LEVEL_UI, m_objInfo.pLayerTag, m_objInfo.pPrototypeTag, &m_objInfo)))
				return E_FAIL;
			InsertObj();
	}
		Safe_Release(pGameInstance);
	}
	
	return S_OK;
}







CLevel_UI * CLevel_UI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_UI*		pInstance = new CLevel_UI(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Created : CLevel_UI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_UI::Free()
{
	__super::Free();
	sqlite3_close(db);
}


HRESULT CLevel_UI::SelectTable()
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
			rc = sqlite3_prepare_v2(db, sql, -1, &res, NULL);

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
				m_vecTableListTemp.push_back("Level_Boss1");

			m_sTable = m_vecTableListTemp[m_TableCount];

			string sqlTemp = "create table if not exists " + m_sTable + "(ClientLevel integer,\
				Obj	text,\
				LayerTag	text,\
				PrototypeTag text, \
				Pos_x integer \
				Pos_y	integer,\
				TextureInfo	integer";
			
			
			const char* sql = sqlTemp.c_str();

			rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
		}
		ImGui::SameLine();
	}
	ImGui::End();

	return S_OK;
}

HRESULT CLevel_UI::Load()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;
	Safe_AddRef(pGameInstance);

	if (ImGui::Begin("Data Management"))
	{
		_uint count = 0;
		_uint textureNum = 0;

		if (ImGui::Button("Load", ImVec2(70, 25)))
		{
			int rc;
			sqlite3_stmt *res3;

			string sqlTemp = "select * from " + m_sTable;
			const char* sql = sqlTemp.c_str();


			rc = sqlite3_prepare_v2(db, sql, -1, &res3, 0);

			while (sqlite3_step(res3) == SQLITE_ROW)
			{
				m_objInfo.clientLevel = (OBJ_CLIENTLEVELE)sqlite3_column_int(res3, 1);

				char			temp[MAX_PATH] = "";
				strcpy_s(temp, (char*)sqlite3_column_text(res3, 1));

				m_objInfo.fPos.x = (_float)sqlite3_column_double(res3, 4);
				m_objInfo.fPos.y = (_float)sqlite3_column_double(res3, 5);
				m_objInfo.m_Table = m_sTable;
				m_objInfo.db = db;
				m_objInfo.pTextureInfo = (_float)sqlite3_column_int(res3, 6);

				if (strcmp(temp, "Underpants") == 0)
				{
					m_objInfo.pObj = L"Underpants";
					m_objInfo.pLayerTag = L"Layer_Underpants";
					m_objInfo.pPrototypeTag = L"Prototype_GameObject_Underpants";
				}
				else if (strcmp(temp, "Boss_HPBar") == 0)
				{
					m_objInfo.pObj = L"Boss_HPBar";
					m_objInfo.pLayerTag = L"Layer_Boss_HPBar";
					m_objInfo.pPrototypeTag = L"Prototype_GameObject_Boss_HPBar";
				
				}
				else if (strcmp(temp, "Game_Start") == 0)
				{
					m_objInfo.pObj = L"Game_Start";
					m_objInfo.pLayerTag = L"Layer_Game_Start";
					m_objInfo.pPrototypeTag = L"Prototype_GameObject_Disable_Geme_Start";

				}
				else if (strcmp(temp, "Exit") == 0)
				{
					m_objInfo.pObj = L"Boss_HPBar";
					m_objInfo.pLayerTag = L"Layer_Exit";
					m_objInfo.pPrototypeTag = L"Prototype_GameObject_Disable_Exit";

				}
				else if (strcmp(temp, "Menu_Bar") == 0)
				{
					m_objInfo.pObj = L"Menu_Bar";
					m_objInfo.pLayerTag = L"Layer_Menu_Bar";
					m_objInfo.pPrototypeTag = L"Prototype_GameObject_MenuBar";

				}
				else if (strcmp(temp, "Eenergy") == 0)
				{
					if(count < 3 && count >= 0)
						m_objInfo.pTextureInfo = 0;
					else if(count >= 3 && count < 6)
						m_objInfo.pTextureInfo = 1;
					else
						m_objInfo.pTextureInfo = 2;
					
					m_objInfo.pObj = L"Eenergy";
					m_objInfo.pLayerTag = L"Laye_Eenergy";
					m_objInfo.pPrototypeTag = L"Prototype_GameObject_Boss_Energy";

					count++;
				}
			
					if (FAILED(pGameInstance->Clone_GameObject(LEVEL_UI, m_objInfo.pLayerTag, m_objInfo.pPrototypeTag, &m_objInfo)))
						return E_FAIL;

					if (rc != SQLITE_OK)
				{
					MSG_BOX("SQL error");
					return E_FAIL;
				}
			}
		}
		
	}
	Safe_Release(pGameInstance);
	ImGui::End();
}

void CLevel_UI::InsertObj()
{
	int rc;
	char *err_msg = 0;
	sqlite3_stmt *res2 = nullptr;
	string sqlTemp = "INSERT INTO " + m_sTable + "  VALUES (@ClientLevel, @Obj, @LayerTag, @PrototypeTag,@Pos_x, @Pos_y, @TextureInfo)";


	const char* sql2 = sqlTemp.c_str();
	rc = sqlite3_prepare_v2(db, sql2, -1, &res2, NULL);
	if (SQLITE_OK == rc)
	{
		int a = 0;
	}
	int index = sqlite3_bind_parameter_index(res2, "@ClientLevel");
	sqlite3_bind_int(res2, index, (_int)m_objInfo.clientLevel);

	index = sqlite3_bind_parameter_index(res2, "@Obj");
	_bstr_t temp1(m_objInfo.pObj);
	const char* pObj = temp1; 
	sqlite3_bind_text(res2, index, pObj, -1, SQLITE_TRANSIENT);


	_bstr_t temp2(m_objInfo.pLayerTag);
	const char* pLayerTag = temp2;
	index = sqlite3_bind_parameter_index(res2, "@LayerTag");
	sqlite3_bind_text(res2, index, pLayerTag, -1, SQLITE_TRANSIENT);

	_bstr_t temp3(m_objInfo.pPrototypeTag);
	const char* pPrototypeTag = temp3;
	index = sqlite3_bind_parameter_index(res2, "@PrototypeTag");
	sqlite3_bind_text(res2, index, pPrototypeTag, -1, SQLITE_TRANSIENT);

	index = sqlite3_bind_parameter_index(res2, "@Pos_x");
	sqlite3_bind_double(res2, index, m_objInfo.fPos.x);
	index = sqlite3_bind_parameter_index(res2, "@Pos_y");
	sqlite3_bind_double(res2, index, m_objInfo.fPos.y);


	 index = sqlite3_bind_parameter_index(res2, "@TextureInfo");
	sqlite3_bind_int(res2, index, m_objInfo.pTextureInfo);

	int step = sqlite3_step(res2);
}
