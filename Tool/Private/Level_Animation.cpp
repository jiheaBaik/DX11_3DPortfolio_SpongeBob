#include "..\Public\Level_Animation.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "SpongeBob.h"
#include "Boss_Sandy.h"
#include "Boss_Sandy_Body.h"
#include "Patrick.h"

#include<iostream>
#include <sstream>
#include <tchar.h>
#include <comdef.h>
#include <algorithm>

CLevel_Animation::CLevel_Animation(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext), m_pDevice(pDevice), m_pContext(pContext)
{
}

HRESULT CLevel_Animation::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Clone_GameObject()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Animation::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CLevel_Animation::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CLevel_Animation::Render()
{

	if (ImGui::Begin("Level_Navigation MousePos"))
	{
		ImVec2  screenMousePos = ImGui::GetMousePosOnOpeningCurrentPopup();
		ImVec2 mainViewPos = ImGui::GetMainViewport()->Pos;
		ImGui::Text("MainViewMousePos_x : %1.f", screenMousePos.x - mainViewPos.x);
		ImGui::Text("MainViewMousePos_y : %1.f", screenMousePos.y - mainViewPos.y - 24);
	}
	ImGui::End();

	SelectMesh();

	return S_OK;
}

HRESULT CLevel_Animation::Ready_Prototype_Component()
{
	return S_OK;
}

HRESULT CLevel_Animation::Ready_Clone_GameObject()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 0.f, -5.0f);
	CameraDesc.vAt = _float3(0.f, 1.f, 0.f);
	CameraDesc.TransformDesc.SpeedPerSec = 15.f;
	CameraDesc.TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = _float(g_iWinSizeX) / g_iWinSizeY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 500.f;

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float3(1.f, -1.f, 1.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_ANIMATION, TEXT("Layer_Camera"), TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
		return E_FAIL;


	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_ANIMATION, L"Layer_SpongeBob", L"Prototype_GameObject_SpongeBob")))
		return E_FAIL;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_ANIMATION, L"Layer_Boss_Sandy_Head", L"Prototype_GameObject_Boss_Sandy_Head")))
		return E_FAIL;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_ANIMATION, L"Layer_Boss_Sandy_Body", L"Prototype_GameObject_Boss_Sandy_Body")))
		return E_FAIL;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_ANIMATION, L"Layer_Patrick", L"Prototype_GameObject_Patrick")))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Animation::DBConnection()
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

HRESULT CLevel_Animation::SelectMesh()
{
	
	static _int NumberPick = 0;
	static _int m_ActionListIndex = 0;
	if (ImGui::Begin("Select Character"))
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		if (nullptr == pGameInstance)
			return E_FAIL;

		Safe_AddRef(pGameInstance);

		ImGui::Combo("Character Pick", &CharacterPick, "SpongeBob\0Boss_Sandy\0Patrick\0");
		ImGui::InputInt("Number", &NumberPick);

		ImGui::SameLine();

		if (ImGui::Button("View"))
		{
			CSpongeBob* pSpongeBob = static_cast<CSpongeBob*>(pGameInstance->Get_GameObject(LEVEL_ANIMATION, TEXT("Layer_SpongeBob")));
			CBoss_Sandy* pBoss_Sandy_Head = static_cast<CBoss_Sandy*>(pGameInstance->Get_GameObject(LEVEL_ANIMATION, TEXT("Layer_Boss_Sandy_Head")));
			CBoss_Sandy_Body* pBoss_Sandy_Body = static_cast<CBoss_Sandy_Body*>(pGameInstance->Get_GameObject(LEVEL_ANIMATION, TEXT("Layer_Boss_Sandy_Body")));
			CPatrick* pPatrick = static_cast<CPatrick*>(pGameInstance->Get_GameObject(LEVEL_ANIMATION, TEXT("Layer_Patrick")));

			switch (CharacterPick)
			{
			case CHARACTER_SPONGBOB:
				
				pSpongeBob->SetActionIndex(NumberPick);
				pSpongeBob->Set_Render(true);
				pBoss_Sandy_Head->Set_Render(false);
				pBoss_Sandy_Body->Set_Render(false);
				pBoss_Sandy_Body->Set_Render(false);
				pPatrick->Set_Render(false);
				break;
				
			case CHARACTER_BOSS_SANADY:
				
				pBoss_Sandy_Head->SetActionIndex(NumberPick);
				pBoss_Sandy_Head->Set_Render(true);
				pBoss_Sandy_Body->SetActionIndex(NumberPick);
				pBoss_Sandy_Body->Set_Render(true);
				pSpongeBob->Set_Render(false);
				pPatrick->Set_Render(false);
				break;
			case CHARACTER_PATRICK:

				pSpongeBob->Set_Render(false);
				pBoss_Sandy_Head->Set_Render(false);
				pBoss_Sandy_Body->Set_Render(false);
				pBoss_Sandy_Body->Set_Render(false);
				pPatrick->Set_Render(true);
				pPatrick->SetActionIndex(NumberPick);
				break;
			default:
				break;
			}
		}
		ImGui::Separator();

		

		State_Info(AnimationPick);
	

		if (ImGui::Button("Customize Animation"))
		{
			ACTIONINFODESC animationInfo;
			animationInfo.ActionLevel = AnimationPick;
			animationInfo.ActionIndex = NumberPick;

			m_AnimationInfo.push_back(animationInfo);

			string string_temp = to_string(NumberPick);

			m_actionDesc.Action.append(string_temp, 0, string_temp.size());
			std::wstring widestr = std::wstring(m_actionDesc.Action.begin(), m_actionDesc.Action.end());
			const wchar_t* widecstr = widestr.c_str();
			m_vActionList.push_back(m_actionDesc.Action);

		}
		Safe_Release(pGameInstance);

	}
	ImGui::End();

	if (ImGui::Begin("Action List"))
	{
		if (ImGui::BeginListBox("Action List", ImVec2(-FLT_MIN, 15 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int i = 0; i < m_vActionList.size(); i++)
			{
				const bool is_selected = (m_ActionListIndex == i);
				if (ImGui::Selectable(m_vActionList[i].c_str(), is_selected))
					m_ActionListIndex = i;
			}
		}
		ImGui::EndListBox();

		if (ImGui::Button("Delete"))
		{
			m_vActionList.erase(m_vActionList.begin() + m_ActionListIndex);
			m_AnimationInfo.erase(m_AnimationInfo.begin() + m_ActionListIndex);

		}
		
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{

			std::sort(m_AnimationInfo.begin(), m_AnimationInfo.end(), compare);

			TCHAR PathFileName[MAX_PATH] = TEXT("");

			OPENFILENAME ofn;
			::memset(&ofn, 0, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.nFilterIndex = 2;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFile = PathFileName;
			ofn.Flags = OFN_FILEMUSTEXIST;
			if (::GetSaveFileName(&ofn) == true)
			{
				HANDLE	hFile = CreateFile(PathFileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

				if (INVALID_HANDLE_VALUE == hFile)
					return E_FAIL;

				DWORD	dwByte = 0;

				for (_uint i = 0; i < m_AnimationInfo.size(); i++)
					WriteFile(hFile, &m_AnimationInfo[i], sizeof(ACTIONINFODESC), &dwByte, nullptr);


				CloseHandle(hFile);
			}
			m_vActionList.clear();
			m_AnimationInfo.clear();
		}

		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			m_vActionList.clear();
			m_AnimationInfo.clear();
			TCHAR PathFileName[MAX_PATH] = TEXT("");

			OPENFILENAME ofn;
			::memset(&ofn, 0, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.nFilterIndex = 2;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFile = PathFileName;
			ofn.Flags = OFN_FILEMUSTEXIST;
			if (::GetOpenFileName(&ofn) == true)
			{
				HANDLE	hFile = CreateFile(PathFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

				if (INVALID_HANDLE_VALUE == hFile)
					return E_FAIL;

				DWORD	dwByte = 0;
				ACTIONINFODESC animationInfo;
				while (true)
				{
					ReadFile(hFile, &animationInfo, sizeof(ACTIONINFODESC), &dwByte, nullptr);

					if (0 == dwByte)
						break;
					m_AnimationInfo.push_back(animationInfo);

					string string_temp = to_string(animationInfo.ActionIndex);

					State_Info((SPONGEBOB_ACTION)animationInfo.ActionLevel);
					
					m_actionDesc.Action.append(string_temp, 0, string_temp.size());
					std::wstring widestr = std::wstring(m_actionDesc.Action.begin(), m_actionDesc.Action.end());
					const wchar_t* widecstr = widestr.c_str();
					m_vActionList.push_back(m_actionDesc.Action);
				}

				CloseHandle(hFile);
			}
		}

		
	}
	ImGui::End();



	if(ImGui::Begin("Set_Pattern"))
	{
		static _int PatternNum = 0;
		static _int patternList = 0;
		static _uint SelectpatternList = 0;

			if (ImGui::Button("Select Motion"))
			{
				m_vPatternList.push_back((SANDY_ACTION)NumberPick);

				patternList++;
				m_vPatternNameList.push_back(to_string(NumberPick));


			}
			if (ImGui::BeginListBox("Pattern List", ImVec2(-FLT_MIN, 15 * ImGui::GetTextLineHeightWithSpacing())))
			{
				for (int i = 0; i < m_vPatternNameList.size(); i++)
				{
					const bool is_selected = (SelectpatternList == i);
					if (ImGui::Selectable(m_vPatternNameList[i].c_str(), is_selected))
						SelectpatternList = i;
				}
			}
			ImGui::EndListBox();

			if (ImGui::Button("Delete"))
			{
				m_vPatternNameList.erase(m_vPatternNameList.begin() + SelectpatternList);
				m_vPatternList.erase(m_vPatternList.begin() + SelectpatternList);
				patternList--;

			}
			ImGui::SameLine();
			if (ImGui::Button("Insert"))
			{
				m_vPatternNameList.insert(m_vPatternNameList.begin() + SelectpatternList, to_string(NumberPick) );
				m_vPatternList.insert(m_vPatternList.begin() + SelectpatternList, (SANDY_ACTION)NumberPick);
				patternList++;

			}
			ImGui::SameLine();
			if (ImGui::Button("Save"))
			{

				TCHAR PathFileName[MAX_PATH] = TEXT("");

				OPENFILENAME ofn;
				::memset(&ofn, 0, sizeof(ofn));

				ofn.lStructSize = sizeof(ofn);
				ofn.nFilterIndex = 2;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrFile = PathFileName;
				ofn.Flags = OFN_FILEMUSTEXIST;
				if (::GetSaveFileName(&ofn) == true)
				{
					HANDLE	hFile = CreateFile(PathFileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

					if (INVALID_HANDLE_VALUE == hFile)
						return E_FAIL;

					DWORD	dwByte = 0;

					for (_uint i = 0; i < m_vPatternList.size(); i++)
					{
						
							WriteFile(hFile, &m_vPatternList[i], sizeof(_uint), &dwByte, nullptr);
					}
					CloseHandle(hFile);
				}
				m_vPatternNameList.clear();
				m_vPatternList.clear();
			}
			ImGui::SameLine();
			if (ImGui::Button("Load"))
			{

				m_vPatternList.clear();
				m_vPatternNameList.clear();
				TCHAR PathFileName[MAX_PATH] = TEXT("");

				OPENFILENAME ofn;
				::memset(&ofn, 0, sizeof(ofn));

				ofn.lStructSize = sizeof(ofn);
				ofn.nFilterIndex = 2;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrFile = PathFileName;
				ofn.Flags = OFN_FILEMUSTEXIST;
				if (::GetOpenFileName(&ofn) == true)
				{
					HANDLE	hFile = CreateFile(PathFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

					if (INVALID_HANDLE_VALUE == hFile)
						return E_FAIL;

					DWORD	dwByte = 0;
					_uint PatternInfo;
					while (true)
					{
						
						ReadFile(hFile, &PatternInfo, sizeof(_uint), &dwByte, nullptr);

						if (0 == dwByte)
							break;
						m_vPatternList.push_back(PatternInfo);

						string string_temp = to_string(PatternInfo);


						m_vPatternNameList.push_back(to_string(PatternInfo));
					}

					CloseHandle(hFile);
				}
			}
	}
	ImGui::End();


		return S_OK;
	}


	

	void CLevel_Animation::State_Info(_int _sateInfo)
	{
		switch (CharacterPick)
		{
		case CHARACTER_SPONGBOB:
			ImGui::Combo("Animation Pick", &AnimationPick, "SPONGEBOB_IDLE\0SPONGEBOB_WALK\0SPONGEBOB_JUMPUP\0SPONGEBOB_RUN\0SPONGEBOB_DEATH\0SPONGEBOB_ATTACK\0SPONGEBOB_JUMPDW\0SPONGEBOB_JUMP_LANDING_EYE\0SPONGEBOB_BUBLE_START\0SPONGEBOB_BUBLE_LANDING\00SPONGEBOB_BUBLE_LOOP\0SPONGEBOB_BUBLE_SPINE\0SPONGEBOB_BASH_LOOP\0SPONGEBOB_POSEIDOME_INTRO_0\0SPONGEBOB_POSEIDOME_INTRO_1\0SPONGEBOB_POSEIDOME_INTRO_2\0SPONGEBOB_POSEIDOME_INTRO_3\0SPONGEBOB_POSEIDOME_INTRO_4\0SPONGEBOB_POSEIDOME_OUTRO0\0SPONGEBOB_POSEIDOME_OUTRO1\0SPONGEBOB_POSEIDOME_OUTRO2\0SPONGEBOB_POSEIDOME_ROUND2_0\0SPONGEBOB_POSEIDOME_ROUND2_1\0SPONGEBOB_POSEIDOME_ROUND2_2\0SPONGEBOB_POSEIDOME_ROUND2_3\0SPONGEBOB_POSEIDOME_ROUND3_0\0SPONGEBOB_POSEIDOME_ROUND3_1\0SPONGEBOB_HIT\0");
			if (_sateInfo == SPONGEBOB_IDLE)
				m_actionDesc.Action = "SPONGEBOB_IDLE : ";
			else if (_sateInfo == SPONGEBOB_WALK)
				m_actionDesc.Action = "SPONGEBOB_WALK : ";
			else if (_sateInfo == SPONGEBOB_JUMPUP)
				m_actionDesc.Action = "SPONGEBOB_JUMPUP : ";
			else if (_sateInfo == SPONGEBOB_RUN)
				m_actionDesc.Action = "SPONGEBOB_RUN : ";
			else if (_sateInfo == SPONGEBOB_DEATH)
				m_actionDesc.Action = "SPONGEBOB_DEATH : ";
			else if (_sateInfo == SPONGEBOB_ATTACK)
				m_actionDesc.Action = "SPONGEBOB_ATTACK : ";
			else if (_sateInfo == SPONGEBOB_JUMPDW)
				m_actionDesc.Action = "SPONGEBOB_JUMPDW : ";
			else if (_sateInfo == SPONGEBOB_JUMP_LANDING_EYE)
				m_actionDesc.Action = "SPONGEBOB_JUMP_LANDING_EYE : ";
			else if (_sateInfo == SPONGEBOB_BUBLE_START)
				m_actionDesc.Action = "SPONGEBOB_BUBLE_START : ";
			else if (_sateInfo == SPONGEBOB_BUBLE_LANDING)
				m_actionDesc.Action = "SPONGEBOB_BUBLE_LANDING : ";
			else if (_sateInfo == SPONGEBOB_BUBLE_LOOP)
				m_actionDesc.Action = "SPONGEBOB_BUBLE_LOOP : ";
			else if (_sateInfo == SPONGEBOB_BUBLE_SPINE)
				m_actionDesc.Action = "SPONGEBOB_BUBLE_SPINE : ";
			else if (_sateInfo == SPONGEBOB_BASH_LOOP)
				m_actionDesc.Action = "SPONGEBOB_BASH_LOOP : ";
			else if (_sateInfo == SPONGEBOB_POSEIDOME_INTRO_0)
				m_actionDesc.Action = "SPONGEBOB_POSEIDOME_INTRO_0 : ";
			else if (_sateInfo == SPONGEBOB_POSEIDOME_INTRO_1)
				m_actionDesc.Action = "SPONGEBOB_POSEIDOME_INTRO_1 : ";
			else if (_sateInfo == SPONGEBOB_POSEIDOME_INTRO_2)
				m_actionDesc.Action = "SPONGEBOB_POSEIDOME_INTRO_2 : ";
			else if (_sateInfo == SPONGEBOB_POSEIDOME_INTRO_3)
				m_actionDesc.Action = "SPONGEBOB_POSEIDOME_INTRO_3 : ";
			else if (_sateInfo == SPONGEBOB_POSEIDOME_INTRO_4)
				m_actionDesc.Action = "SPONGEBOB_POSEIDOME_INTRO_4 : ";
			else if (_sateInfo == SPONGEBOB_POSEIDOME_OUTRO0)
				m_actionDesc.Action = "SPONGEBOB_POSEIDOME_OUTRO0 : ";
			else if (_sateInfo == SPONGEBOB_POSEIDOME_OUTRO1)
				m_actionDesc.Action = "SPONGEBOB_POSEIDOME_OUTRO1 : ";
			else if (_sateInfo == SPONGEBOB_POSEIDOME_OUTRO2)
				m_actionDesc.Action = "SPONGEBOB_POSEIDOME_OUTRO2 : ";
			else if (_sateInfo == SPONGEBOB_POSEIDOME_ROUND2_0)
				m_actionDesc.Action = "SPONGEBOB_POSEIDOME_ROUND2_0 : ";
			else if (_sateInfo == SPONGEBOB_POSEIDOME_ROUND2_1)
				m_actionDesc.Action = "SPONGEBOB_POSEIDOME_ROUND2_1 : ";
			else if (_sateInfo == SPONGEBOB_POSEIDOME_ROUND2_2)
				m_actionDesc.Action = "SPONGEBOB_POSEIDOME_ROUND2_2 : ";
			else if (_sateInfo == SPONGEBOB_POSEIDOME_ROUND2_3)
				m_actionDesc.Action = "SPONGEBOB_POSEIDOME_ROUND2_3 : ";
			else if (_sateInfo == SPONGEBOB_POSEIDOME_ROUND3_0)
				m_actionDesc.Action = "SPONGEBOB_POSEIDOME_ROUND3_0 : ";
			else if (_sateInfo == SPONGEBOB_POSEIDOME_ROUND3_1)
				m_actionDesc.Action = "SPONGEBOB_POSEIDOME_ROUND3_1 : ";
			else if (_sateInfo == SPONGEBOB_HIT)
				m_actionDesc.Action = "SPONGEBOB_HIT : ";
			break;
		case CHARACTER_BOSS_SANADY:
			ImGui::Combo("Animation Pick", &AnimationPick, "SANDY_INTRO\0SANDY_IDLE\0SANDY_WALK\0SANDY_RUN\0SANDY_DEATH\0SANDY_DEATH_STOP\0SANDY_ATTACK_SPRING_START\0SANDY_ATTACK_SPRING1\0SANDY_ATTACK_SPRING2,\0SANDY_ATTACK_FALLDOWN\0SANDY_ATTACK_JUMP_START\0SANDY_ATTACK_JUMP1\0SANDY_ATTACK_JUMP2\0SANDY_ATTACK_HAND\0SANDY_HIT_SIT_TOUCH_HEAD1\0SANDY_HIT_SIT_TOUCH_HEAD2\0SANDY_HIT_SIT_ELECTIVICITY\0SANDY_HIT_UP_ELECTIVICITY\0SANDY_HIT_SIT_ELECTIVICITY_90AGREE\0SANDY_PROVOKE_SIT_THRUST_HEAD\0SANDY_PROVOKE_SIT_THRUST_HEAD_SHAKE\0SANDY_SIT_THRUST_HAND\0SANDY_PROVOKE_WALK_THRUST_HEAD\0SANDY_PROVOKE_HAND\0SANDY_GET_HELMET\0SANDY_FIXING_HELMET\0SANDY_ATTACK_SPRING3\0SANDY_ATTACK_SPRING4\0");
			if (_sateInfo == SANDY_INTRO)
				m_actionDesc.Action = "SANDY_INTRO : ";
			else if (_sateInfo == SANDY_IDLE)
				m_actionDesc.Action = "SANDY_IDLE : ";
			else if (_sateInfo == SANDY_WALK)
				m_actionDesc.Action = "SANDY_WALK : ";
			else if (_sateInfo == SANDY_RUN)
				m_actionDesc.Action = "SANDY_RUN : ";
			else if (_sateInfo == SANDY_DEATH)
				m_actionDesc.Action = "SANDY_DEATH : ";
			else if (_sateInfo == SANDY_DEATH_STOP)
				m_actionDesc.Action = "SANDY_DEATH_STOP : ";
			else if (_sateInfo == SANDY_ATTACK_SPRING_START)
				m_actionDesc.Action = "SANDY_ATTACK_SPRING_START : ";
			else if (_sateInfo == SANDY_ATTACK_SPRING1)
				m_actionDesc.Action = "SANDY_ATTACK_SPRING1 : ";
			else if (_sateInfo == SANDY_ATTACK_SPRING2)
				m_actionDesc.Action = "SANDY_ATTACK_SPRING2 : ";
			else if (_sateInfo == SANDY_ATTACK_FALLDOWN)
				m_actionDesc.Action = "SANDY_ATTACK_FALLDOWN : ";
			else if (_sateInfo == SANDY_ATTACK_JUMP_START)
				m_actionDesc.Action = "SANDY_ATTACK_JUMP_START : ";
			else if (_sateInfo == SANDY_ATTACK_JUMP1)
				m_actionDesc.Action = "SANDY_ATTACK_JUMP1 : ";
			else if (_sateInfo == SANDY_ATTACK_JUMP2)
				m_actionDesc.Action = "SANDY_ATTACK_JUMP2 : ";
			else if (_sateInfo == SANDY_ATTACK_HAND)
				m_actionDesc.Action = "SANDY_ATTACK_HAND : ";
			else if (_sateInfo == SANDY_HIT_SIT_TOUCH_HEAD1)
				m_actionDesc.Action = "SANDY_HIT_SIT_TOUCH_HEAD1 : ";
			else if (_sateInfo == SANDY_HIT_SIT_TOUCH_HEAD2)
				m_actionDesc.Action = "SANDY_HIT_SIT_TOUCH_HEAD2 : ";
			else if (_sateInfo == SANDY_HIT_SIT_ELECTIVICITY)
				m_actionDesc.Action = "SANDY_HIT_SIT_ELECTIVICITY : ";
			else if (_sateInfo == SANDY_HIT_UP_ELECTIVICITY)
				m_actionDesc.Action = "SANDY_HIT_UP_ELECTIVICITY : ";
			else if (_sateInfo == SANDY_HIT_SIT_ELECTIVICITY_90AGREE)
				m_actionDesc.Action = "SANDY_HIT_SIT_ELECTIVICITY_90AGREE : ";
			else if (_sateInfo == SANDY_PROVOKE_SIT_THRUST_HEAD)
				m_actionDesc.Action = "SANDY_PROVOKE_SIT_THRUST_HEAD : ";
			else if (_sateInfo == SANDY_PROVOKE_SIT_THRUST_HEAD_SHAKE)
				m_actionDesc.Action = "SANDY_PROVOKE_SIT_THRUST_HEAD_SHAKE : ";
			else if (_sateInfo == SANDY_SIT_THRUST_HAND)
				m_actionDesc.Action = "SANDY_SIT_THRUST_HAND : ";
			else if (_sateInfo == SANDY_PROVOKE_WALK_THRUST_HEAD)
				m_actionDesc.Action = "SANDY_PROVOKE_WALK_THRUST_HEAD : ";
			else if (_sateInfo == SANDY_PROVOKE_HAND)
				m_actionDesc.Action = "SANDY_PROVOKE_HAND : ";
			else if (_sateInfo == SANDY_GET_HELMET)
				m_actionDesc.Action = "SANDY_GET_HELMET : ";
			else if (_sateInfo == SANDY_FIXING_HELMET)
				m_actionDesc.Action = "SANDY_FIXING_HELMET : ";
			else if (_sateInfo == SANDY_ATTACK_SPRING3)
				m_actionDesc.Action = "SANDY_ATTACK_SPRING3 : ";
			else if (_sateInfo == SANDY_ATTACK_SPRING4)
				m_actionDesc.Action = "SANDY_ATTACK_SPRING4 : ";
			break;

		case CHARACTER_PATRICK:
			ImGui::Combo("Animation Pick", &AnimationPick, "PATRICK_ATTACK\0PATRICK_DEATH\0PATRICK_HIT\0PATRICK_IDLE\0PATRICK_JUMP_DW\0PATRICK_JUMP_UP\0PATRICK_INTRO_2\0PATRICK_INTRO_3\0PATRICK_INTRO_7\0PATRICK_OUTTRO_0\0PATRICK_ROUND2_1\0PATRICK_ROUND2_2\0PATRICK_ROUND2_4\0PATRICK_ROUND3_1\0PATRICK_ROUND3_2\0PATRICK_PICKUP\0PATRICK_PICKUP_IDLE\0PATRICK_PICKUP_JUMP_DW\0PATRICK_PICKUP_JUMP_UP\0PATRICK_PICKUP_WALK\0PATRICK_PICKUP_RUN\0PATRICK_SLAM_END\0PATRICK_SLAM_LOOP\00PATRICK_SLAM_START\0PATRICK_TALK\0PATRICK_THROW\0PATRICK_VICTORY\0PATRICK_WALK\0PATRICK_WATCHING_DOME\0");
			if (_sateInfo == PATRICK_ATTACK)
				m_actionDesc.Action = "PATRICK_ATTACK : ";
			else if (_sateInfo == PATRICK_DEATH)
				m_actionDesc.Action = "PATRICK_DEATH : ";
			else if (_sateInfo == PATRICK_HIT)
				m_actionDesc.Action = "PATRICK_HIT : ";
			else if (_sateInfo == PATRICK_IDLE)
				m_actionDesc.Action = "PATRICK_IDLE : ";
			else if (_sateInfo == PATRICK_JUMP_DW)
				m_actionDesc.Action = "PATRICK_JUMP_DW : ";
			else if (_sateInfo == PATRICK_JUMP_UP)
				m_actionDesc.Action = "PATRICK_JUMP_UP : ";
			else if (_sateInfo == PATRICK_INTRO_2)
				m_actionDesc.Action = "PATRICK_INTRO_2 : ";
			else if (_sateInfo == PATRICK_INTRO_3)
				m_actionDesc.Action = "PATRICK_INTRO_3 : ";
			else if (_sateInfo == PATRICK_INTRO_7)
				m_actionDesc.Action = "PATRICK_INTRO_7 : ";
			else if (_sateInfo == PATRICK_OUTTRO_0)
				m_actionDesc.Action = "PATRICK_OUTTRO_0 : ";
			else if (_sateInfo == PATRICK_ROUND2_1)
				m_actionDesc.Action = "PATRICK_ROUND2_1 : ";
			else if (_sateInfo == PATRICK_ROUND2_2)
				m_actionDesc.Action = "PATRICK_ROUND2_2 : ";
			else if (_sateInfo == PATRICK_ROUND2_4)
				m_actionDesc.Action = "PATRICK_ROUND2_4 : ";
			else if (_sateInfo == PATRICK_ROUND3_1)
				m_actionDesc.Action = "PATRICK_ROUND3_1 : ";
			else if (_sateInfo == PATRICK_ROUND3_2)
				m_actionDesc.Action = "PATRICK_ROUND3_2 : ";
			else if (_sateInfo == PATRICK_PICKUP)
				m_actionDesc.Action = "PATRICK_PICKUP : ";
			else if (_sateInfo == PATRICK_PICKUP_IDLE)
				m_actionDesc.Action = "PATRICK_PICKUP_IDLE : ";
			else if (_sateInfo == PATRICK_PICKUP_JUMP_DW)
				m_actionDesc.Action = "PATRICK_PICKUP_JUMP_DW : ";
			else if (_sateInfo == PATRICK_PICKUP_JUMP_UP)
				m_actionDesc.Action = "PATRICK_PICKUP_JUMP_UP : ";
			else if (_sateInfo == PATRICK_PICKUP_WALK)
				m_actionDesc.Action = "PATRICK_PICKUP_WALK : ";
			else if (_sateInfo == PATRICK_PICKUP_RUN)
				m_actionDesc.Action = "PATRICK_PICKUP_RUN : ";
			else if (_sateInfo == PATRICK_SLAM_END)
				m_actionDesc.Action = "PATRICK_SLAM_START : ";
			else if (_sateInfo == PATRICK_SLAM_LOOP)
				m_actionDesc.Action = "PATRICK_SLAM_LOOP : ";
			else if (_sateInfo == PATRICK_SLAM_START)
				m_actionDesc.Action = "PATRICK_SLAM_END : ";
			else if (_sateInfo == PATRICK_TALK)
				m_actionDesc.Action = "PATRICK_TALK : ";
			else if (_sateInfo == PATRICK_THROW)
				m_actionDesc.Action = "PATRICK_THROW : ";
			else if (_sateInfo == PATRICK_VICTORY)
				m_actionDesc.Action = "PATRICK_VICTORY : ";
			else if (_sateInfo == PATRICK_WALK)
				m_actionDesc.Action = "PATRICK_WALK : ";
			else if (_sateInfo == PATRICK_WATCHING_DOME)
				m_actionDesc.Action = "PATRICK_WATCHING_DOME : ";
			break;
		default:
			break;
		}
		
	}

	
HRESULT CLevel_Animation::SelectTable()
{
	return S_OK;
}


CLevel_Animation * CLevel_Animation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Animation*		pInstance = new CLevel_Animation(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Created : CLevel_UI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Animation::Free()
{
	__super::Free();
}
