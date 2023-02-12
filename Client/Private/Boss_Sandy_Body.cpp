#include "stdafx.h"
#include "..\Public\Boss_Sandy_Body.h"
#include "GameInstance.h"
#include "Player.h"
#include "Rope_Top.h"
#include "Rope_Top_Broekn.h"

#include "Camera_Free.h"

#include "BubblePangEffect.h"


#include <iostream>
#include "Electric.h"
#include "GlowBlue2.h"
#include "Boss_Sandy_Head2.h"
#include "PangEffect.h"

#include "Patrick_Player.h"
#include "Fish.h"
#include "UnderPangEffect.h"



CBoss_Sandy_Body::CBoss_Sandy_Body(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CBoss_Sandy_Body::CBoss_Sandy_Body(const CBoss_Sandy_Body & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CBoss_Sandy_Body::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Sandy_Body::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC TransformDesc(5.f, XMConvertToRadians(90.f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	

	////액션 이넘값
	m_AnimationInfo.clear();
	_tchar		pGetPath_NaimNum[MAX_PATH] = L"../../Reference/Data/Boss_Sandy.dat";
	HANDLE	hFile_AnimNum = CreateFile(pGetPath_NaimNum, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile_AnimNum)
		return E_FAIL;

	DWORD	dwByte = 0;
	ACTIONINFODESC animationInfo;

	while (true)
	{
		ReadFile(hFile_AnimNum, &animationInfo, sizeof(ACTIONINFODESC), &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_AnimationInfo.push_back(animationInfo);
	}

	CloseHandle(hFile_AnimNum);


	//페턴 안맞는 이유 찾기


	//패턴
	m_pattern.m_vPatternList1_1_Attack.clear();
	m_pattern.m_vPatternList1_1_Hit.clear();
	m_pattern.m_vPatternList1_2_Hit.clear();
	m_pattern.m_vPatternList1_3_Hit.clear();
	m_pattern.m_vPatternList1_4_Hit.clear();
	m_pattern.m_vPatternList1_5_Hit.clear();

	m_pattern.m_vPatternList1_2_Attack.clear();
	m_pattern.m_vPatternList1_3_Attack.clear();
	m_pattern.m_vPatternList1_4_Attack.clear();


	//점프Attack,Hit
	_tchar		pGetPath[MAX_PATH] = L"../../Reference/Data/Pattern1_jumpAttack2_Boss_Sandy.dat";
	_tchar		pGetPath2[MAX_PATH] = L"../../Reference/Data/Pattern1_jumpAttack2_Hit_Boss_Sandy.dat";
	_tchar		pGetPath6[MAX_PATH] = L"../../Reference/Data/Pattern1_jumpAttack2_Hit2_Boss_Sandy.dat";
	_tchar		pGetPath7[MAX_PATH] = L"../../Reference/Data/Pattern1_jumpAttack2_Hit4_Boss_Sandy.dat";
	_tchar		pGetPath8[MAX_PATH] = L"../../Reference/Data/Pattern1_jumpAttack2_Hit5_Boss_Sandy.dat";
	_tchar		pGetPath9[MAX_PATH] = L"../../Reference/Data/Pattern1_jumpAttack2_Hit3_Boss_Sandy.dat";

	//엎어지는공격
	_tchar		pGetPath3[MAX_PATH] = L"../../Reference/Data/Pattern1_jumpAttack1_Boss_Sandy.dat";

	// 손 공격
	_tchar		pGetPath4[MAX_PATH] = L"../../Reference/Data/Pattern1_HandAttack1_Boss_Sandy.dat";
	//스프링공격1
	_tchar		pGetPath5[MAX_PATH] = L"../../Reference/Data/Pattern2_SpringAttack1_Boss_Sandy.dat";

	HANDLE	hFile = CreateFile(pGetPath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	dwByte = 0;
	_uint patternInfo;

	while (true)
	{
		ReadFile(hFile, &patternInfo, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_pattern.m_vPatternList1_1_Attack.push_back(patternInfo);
	}


	//CloseHandle(hFile);

	hFile = CreateFile(pGetPath2, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &patternInfo, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_pattern.m_vPatternList1_1_Hit.push_back(patternInfo);
	}

	hFile = CreateFile(pGetPath6, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &patternInfo, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_pattern.m_vPatternList1_2_Hit.push_back(patternInfo);
	}

	hFile = CreateFile(pGetPath7, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &patternInfo, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_pattern.m_vPatternList1_3_Hit.push_back(patternInfo);
	}

	hFile = CreateFile(pGetPath8, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &patternInfo, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_pattern.m_vPatternList1_4_Hit.push_back(patternInfo);
	}


	hFile = CreateFile(pGetPath9, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &patternInfo, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_pattern.m_vPatternList1_5_Hit.push_back(patternInfo);
	}


	hFile = CreateFile(pGetPath3, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &patternInfo, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_pattern.m_vPatternList1_2_Attack.push_back(patternInfo);
	}

	hFile = CreateFile(pGetPath4, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &patternInfo, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_pattern.m_vPatternList1_3_Attack.push_back(patternInfo);
	}

	hFile = CreateFile(pGetPath5, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &patternInfo, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_pattern.m_vPatternList1_4_Attack.push_back(patternInfo);
	}


	CloseHandle(hFile);



	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);
	
	


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(65.f, 0.f, 41.f, 1));
	m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[SANDY_PROVOKE_HAND].ActionIndex);
	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector PlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->LookAt(PlayerPos);

	Safe_Release(pGameInstance);
	return S_OK;
}

void CBoss_Sandy_Body::Tick(_double TimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);




	////카메라 정보 얻기
	CCamera_Free* pCamera = nullptr;
	pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera")));
	if (pCamera != nullptr)
	{
		if (pCamera->Get_m_bIsDirector_End())
		{
			if (!_isIntroEnd)
			{

				IntroShake();

			}
			//플레이어 정보 얻기 

				


			CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_Player")));
			CPatrick_Player* pPlayer2 = static_cast<CPatrick_Player*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer")));
			CTransform* pPlayerTransform = nullptr;

			if (pGameInstance->Get_Player_Info() == 0)
			{
				//pPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_Player")));
				pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
			}
			else
			{
				//pPlayer2 = static_cast<CPatrick_Player*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer")));
				pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer"), TEXT("Com_Transform"));
			}

		//	pCamera->Set_Shake(false);
			_vector PlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
			_float3 fplayerPos;
			XMStoreFloat3(&fplayerPos, PlayerPos);
			_vector m_vPos5 = m_pTransformCom->Get_State(CTransform::STATE_POSITION);




			static _uint AnimPattern = 0;

			IsJumpMotion2 = false;
			IsSpringAttack = false;
			IsSpringAttacking = false;
			IsHandAttack = false;
			IsTouchHead = false;

			AnimPattern = rand() % 3;
			//AnimPattern = 0;
		
			
			m_IsWhite = false;
			//if (AnimPattern == 1)//Hit라면
			//	return;

		/*	if (GetAsyncKeyState('U') & 0x0001)
			{
				AnimPattern = 0;

			}
			if (GetAsyncKeyState('I') & 0x0001)
			{
				AnimPattern = 1;

			}
			if (GetAsyncKeyState('O') & 0x0001)
			{
				AnimPattern = 2;

			}
			if (GetAsyncKeyState('P') & 0x0001)
			{
				AnimPattern = 3;

			}

			if (GetAsyncKeyState('H') & 0x0001)
			{
				CRope_Top* pRope_Top = static_cast<CRope_Top*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Rope_Top"),6));
				pRope_Top->Set_Dead();
				CRope_Top_Broekn* pRope_Top_Brken = static_cast<CRope_Top_Broekn*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Rope_Top_Broken")));
				pRope_Top_Brken->Set_IsBroken(true);
				
			}*/

		


			if (tempPattern.size() == 0)
				tempPattern = m_pattern.m_vPatternList1_1_Attack;


			m_iDeletaTimeEdit = 1.f;
			m_iCurrentAnimIndex = tempPattern[i];



			//if (pPlayer->Get_IsJump() == false && (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_WALK].ActionIndex))
			//{



			//	/*if (fAttack_WalkTime == 0.2f)
			//	{
			//		pCamera->Set_Shake(true);

			//	}
			//	else if (fAttack_WalkTime > 0.3f && fAttack_WalkTime < 0.5f)
			//	{
			//		pCamera->Set_Shake(true);

			//	}
			//	else*/ if (fAttack_WalkTime > 0.8f && fAttack_WalkTime < 0.9f)
			//	{
			//		pCamera->Set_Shake(true);

			//	}
			//	else if (fAttack_WalkTime > 11.f && fAttack_WalkTime < 13.f)
			//	{
			//		pCamera->Set_Shake(true);

			//	}




			//	fAttack_WalkTime += 0.1f;
			//	_vector SandyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			//	m_pTransformCom->Chase(PlayerPos, TimeDelta, 0.f);

			//}


			if (m_pVIBufferCom->Get_NextFinish())
			{
				IsSlamAttack = false;//다음 애니메이션 넘어갈때까지 점프슬램프 공격 이미지 띄우기
									 //여기서 현재애님과 넥스트애님을 정해준다
				m_pVIBufferCom->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
				m_iReal_Time_tAnimIndex = m_iCurrentAnimIndex;

				if (i == tempPattern.size() - 1)
				{
					/************** 애니메이션이 끝난 후 강제로 애니메이션 바꿔야 할 때 ***************************/
					//만약 가운데와 충돌이 났다면 스프링 패턴으로 강제 변경해라.//스프링공격 위함
					CCollider*	pGround2Collider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Ground2"), TEXT("Com_Collider_OBB"));
					if (nullptr == pGround2Collider)
						return;

					if (m_pColliderCom[TYPE_OBB]->Collision_OBB(pGround2Collider))
					{
						tempPattern = m_pattern.m_vPatternList1_4_Attack;
					}
					else
						ChangePattern(AnimPattern);//패턴
					//}/*********************************************************************/


					nextAnim = tempPattern.front();
					i = 0;
				}
				else
				{
					nextAnim = tempPattern[i + 1];
					i++;
				}

				m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta * m_iDeletaTimeEdit, nextAnim, false, i);

			}
			else
			{
				//넥스트를 정해준후임 알아서 선혀보간하든 현재 프레임을 하던m_iCurrentAnimIndex 가 결정
				//m_iCurrentAnimIndex 와 nextAnim가 같다면 현재 프레임을 돌린다는거고 
				//다르다면 선형보간중인것임.
				/*------------------ <*** 패턴 1Page ***> -----------------------------*/
				//만약 현제 애님을 돌린다면(선형보간중이 아니라면)
				if (m_iCurrentAnimIndex == nextAnim)
				{

					//*************애니메이션이 다 끝나지 않은 상태에서 애니메이션이 바뀌어야 한다면*****//
					cout << pPlayer->Get_IsAttack2() << endl;
					
					//if( (pGameInstance->Get_Player_Info() == 0))
					if (pPlayer->Get_IsSlam() && pPlayer->Get_IsAttack2() || pPlayer2->Get_IsSlam() && pPlayer2->Get_IsAttack2() /*pCamera->Get_m_bIsDirectorCount() == 0*//*pPlayer->Get_IsSlam() && pPlayer->Get_IsFoot()*/)
					{//Hit 애니메이션이 돌아야 한다면
						if(pCamera->Get_m_bIsDirectorCount() == 0 || pCamera->Get_m_bIsDirectorCount() == 2)
							tempPattern = m_pattern.m_vPatternList1_1_Hit;
						else if(pCamera->Get_m_bIsDirectorCount() == 1)
							tempPattern = m_pattern.m_vPatternList1_2_Hit;
					//	tempPattern = m_pattern.m_vPatternList1_2_Hit;
					//	tempPattern = m_pattern.m_vPatternList1_3_Hit;
						//tempPattern = m_pattern.m_vPatternList1_4_Hit;
						//tempPattern = m_pattern.m_vPatternList1_5_Hit;

						nextAnim = tempPattern.front();
						i = 0;
					}
					/*********************************************************************/
					else
					{
						if ((m_iReal_Time_tAnimIndex != m_AnimationInfo[SANDY_WALK].ActionIndex))
						{
							fAttack_WalkTime = 0.f;
						}
						else if (m_iReal_Time_tAnimIndex != m_AnimationInfo[SANDY_RUN].ActionIndex)
						{
							fAttack_RunTime = 0.f;
						}
						//현재 동작하고 있는 애님이 조건문일 경우 플레이어를 향해 움직이기 (플레이이어가 점프 중 일땐 제외)
						if (/*pPlayer->Get_IsJump() == false &&*/ (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_WALK].ActionIndex))
						{


							if (fAttack_WalkTime > 0.8f && fAttack_WalkTime < 0.9f)
							{
								pCamera->Set_Shake(true);
								CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_ftstp_Toe_006.ogg", CSoundMgr::MONSTER_FOOT, 0.2f);

							}
							else if (fAttack_WalkTime > 4.f && fAttack_WalkTime < 6.f)
							{
								pCamera->Set_Shake(true);
								CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_ftstp_Toe_006.ogg", CSoundMgr::MONSTER_FOOT, 0.2f);

							}
							else if (fAttack_WalkTime > 11.f && fAttack_WalkTime < 13.f)
							{
								pCamera->Set_Shake(true);
								CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_ftstp_Toe_006.ogg", CSoundMgr::MONSTER_FOOT, 0.2f);

							}


							//if (fAttack_WalkTime < 0.1f)
							//{
							//	pCamera->Set_Shake(true);

							//}
							//else if (fAttack_WalkTime > 0.3f && fAttack_WalkTime < 0.5f)
							//{
							//	pCamera->Set_Shake(true);

							//}
							//else if (fAttack_WalkTime > 0.7f && fAttack_WalkTime < 0.9f)
							//{
							//	pCamera->Set_Shake(true);

							//}
							//else if (fAttack_WalkTime > 11.f && fAttack_WalkTime < 13.f)
							//{
							//	pCamera->Set_Shake(true);

							//}




							fAttack_WalkTime += 0.1f;
							_vector SandyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
							m_pTransformCom->Chase(PlayerPos, TimeDelta, 0.f);

						}
						else if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_RUN].ActionIndex)
						{
							cout << "size : " << tempPattern.size() << endl;
							//CGameInstance::Get_Instance()->StopSound(CSoundMgr::MONSTER_FOOT);
							/*if(AnimPattern == 1)
								CGameInstance::Get_Instance()->PlayMySound(L"Sandy_Run2.mp3", CSoundMgr::MONSTER_FOOT, 0.5f);
							else
								CGameInstance::Get_Instance()->PlayMySound(L"Sandy_Run.mp3", CSoundMgr::MONSTER_FOOT, 0.5f);*/
							
							if(tempPattern.size() >3)
								CGameInstance::Get_Instance()->PlayMySound(L"Sandy_Run.mp3", CSoundMgr::MONSTER_FOOT, 0.2f);
							else
								CGameInstance::Get_Instance()->PlayMySound(L"Sandy_Run2.mp3", CSoundMgr::MONSTER_FOOT, 0.2f);

							if (fAttack_RunTime < 0.1f)
							{
								pCamera->Set_Shake(true);
							

							}
							else if (fAttack_RunTime > 0.2f && fAttack_RunTime < 0.3f)
							{
								pCamera->Set_Shake(true);
							//	CGameInstance::Get_Instance()->StopSound(CSoundMgr::MONSTER_FOOT);


							}
							else if (fAttack_WalkTime > 0.4f && fAttack_WalkTime < 0.5f && AnimPattern != 1)
							{
								pCamera->Set_Shake(true);
			//					CGameInstance::Get_Instance()->StopSound(CSoundMgr::MONSTER_FOOT);


							}
							else if (fAttack_WalkTime > 0.6f && fAttack_WalkTime < 0.7f  && AnimPattern != 1)
							{
								pCamera->Set_Shake(true);
							//	CGameInstance::Get_Instance()->StopSound(CSoundMgr::MONSTER_FOOT);


							}
							else if (fAttack_WalkTime > 0.8f && fAttack_WalkTime < 0.9f  && AnimPattern != 1)
							{
								pCamera->Set_Shake(true);
							//	CGameInstance::Get_Instance()->StopSound(CSoundMgr::MONSTER_FOOT);


							}
							else if (fAttack_WalkTime > 10.f && fAttack_WalkTime < 11.f  && AnimPattern != 1)
							{
								pCamera->Set_Shake(true);
								//CGameInstance::Get_Instance()->StopSound(CSoundMgr::MONSTER_FOOT);


							}

							if (tempPattern == m_pattern.m_vPatternList1_4_Attack)
							{//스프링패턴
								IsSpringAttack = true;
								CTransform* m_pRope_TopTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Rope_Top"), TEXT("Com_Transform"));
								_vector m_vRope_ToPos = m_pRope_TopTransform->Get_State(CTransform::STATE_POSITION);

								_vector SandyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
								m_pTransformCom->Chase(m_vRope_ToPos, TimeDelta, 0.f);
							}
							else
							{
								_vector SandyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
								m_pTransformCom->Chase(PlayerPos, TimeDelta, 0.f);
							}


						}
						else if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_HIT_SIT_ELECTIVICITY].ActionIndex)
						{
							
							CFish* pFish = static_cast<CFish*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Fish")));
							pFish->Set_FishRender(true);




							/*if (getHp == 5)
								
							if (getHp == 2)
								pFish->Set_FishRender(false);
							if (getHp == -1)
								pFish->Set_FishRender(false);*/

							/*CPangEffect* pPang = static_cast<CPangEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Pang")));
							pPang->IsRender(true);*/

							m_iDeletaTimeEdit = 0.5f;
							m_IsWhite = true;

							if (m_iColorChage % 5 == 0)
								m_IsWhite = true;
							else
							{
								m_IsWhite = false;

							}

							if (m_iColorChage == 10)
								m_iColorChage = 0;
							else
								m_iColorChage++;



						}
						else if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_ATTACK_JUMP_START].ActionIndex)
						{//점프 공격시 바라보는 방향으로 뛰기

						 /*	static _uint _count = 0;

						 _count += 1;

							if (_count <3)
							{
								_count = 0;
							pCamera->Set_Shake(true);
								}*/


							_vector		vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
							_vector SandyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
							m_pTransformCom->Set_State(CTransform::STATE_POSITION, SandyPos + (vLook * 0.2));
							IsSlamAttack = true;
						}
						else if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_ATTACK_SPRING1].ActionIndex)
						{//만약 스프링 공격이라면 앞으로전진
							
							



							IsSpringAttacking = true;
							CTransform* m_pRope_TopTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Rope_Top"), TEXT("Com_Transform"), 4);
							_vector m_vRope_ToPos = m_pRope_TopTransform->Get_State(CTransform::STATE_POSITION);
							m_pTransformCom->Chase(m_vRope_ToPos, TimeDelta, 0.f);
						}

						//만약 SANDY_ATTACK_FALLDOWN라면 조금 있다 뛰기
						static _float fAttack_FalldownTime = 0.f;
						static _float fAttack_FalldownTotalTime = 0.f;

						if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_ATTACK_FALLDOWN].ActionIndex)
						{
							IsSlamAttack = true;
							IsSlamAttackCollision = false;

							if (fAttack_FalldownTime <= 0.9f)
								fAttack_FalldownTime += 0.01;
							else//SANDY_ATTACK_FALLDOWN 애니메이션에서 손 탁탁 과 넘어지고나서 일어날땐 이동x 
							{
								_vector		vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
								_vector SandyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
								m_pTransformCom->Set_State(CTransform::STATE_POSITION, SandyPos + (vLook * 0.2));

								IsSlamAttackCollision = true;
								//CGameInstance::Get_Instance()->StopSound(CSoundMgr::MONSTER_FOOT);
								if (fAttack_FalldownTotalTime > 1.3f)
								{
									//CGameInstance::Get_Instance()->StopSound(CSoundMgr::MONSTER_FOOT);
									CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_Karate.ogg", CSoundMgr::MONSTER_FOOT, 0.5f);

									pCamera->Set_Shake(true);
								}

							}

							if (fAttack_FalldownTotalTime <= 1.3f)
							{
								//pCamera->Set_Shake(true);
								fAttack_FalldownTotalTime += 0.01;

							}
							else
								fAttack_FalldownTime = 0.f;
						}
						else
						{
							fAttack_FalldownTotalTime = 0.f;
							fAttack_FalldownTime = 0.f;
						}

						//if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_ATTACK_JUMP_START].ActionIndex)//만약 slame이 필요한 공격이라면


						if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_HIT_SIT_TOUCH_HEAD2].ActionIndex)
						{
							IsTouchHead = true;
						}

						if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_ATTACK_HAND].ActionIndex)
						{
							IsHandAttack = true;
						}

						if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_PROVOKE_HAND].ActionIndex)
						{
							//IsHandAttack = true;
							m_pTransformCom->LookAt(PlayerPos);
							static _uint  count = 0;

							count++;
							if(count > 50)
								CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_ftstp_Toe_006.ogg", CSoundMgr::MONSTER_FOOT, 0.2f);
							if (count > 51)
								count = 0;

						}
					



						if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_ATTACK_SPRING_START].ActionIndex)
						{
							CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_SpringLaunch_001.ogg", CSoundMgr::MONSTER, 0.5f);

							CRope_Top* pRope_Top = static_cast<CRope_Top*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Rope_Top")));

							pRope_Top->Set_IsAction(true);
						}


						if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_PROVOKE_WALK_THRUST_HEAD].ActionIndex)
						{
							CTransform* m_pHeadTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head2"), TEXT("Com_Transform"));
							_vector m_vheadCurrentPos = m_pHeadTransform->Get_State(CTransform::STATE_POSITION);

							if(!m_IsScoreBoxCollision)
								m_pTransformCom->LookAt(PlayerPos);
							else
								m_pTransformCom->LookAt(m_vheadCurrentPos);
						}

						if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_FIXING_HELMET].ActionIndex)
						{
							m_IsScoreBoxCollision = false;
						}

					}

				}
				//여기서의 m_iCurrentAnimIndex는 아무 의미 없음 m_pVIBufferCom->Set_CurrentAnimIndex(()은 위의 함수에서 하기 떄문
				m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta *m_iDeletaTimeEdit, m_iCurrentAnimIndex, false, i);

			}

			if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_ATTACK_JUMP2].ActionIndex)//만약 점프어택중 앉았다면 플레이어가 붕띄워야 한다는 정보 셋팅
			{
				static _uint _count = 0;

				_count += 1;
				if (_count>= 20)
				{
					_count = 0;
					
				}
				else
				{
					CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_Karate.ogg", CSoundMgr::MONSTER_FOOT, 0.2f);
					CUnderPangEffect* pUnderPangEffect = static_cast<CUnderPangEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect_UnderPang")));
					pUnderPangEffect->IsRender(true);
					pCamera->Set_Shake(true);
				}
				
				IsJumpMotion2 = true;
			}
			CBubblePangEffect* pBubblePangEffect = static_cast<CBubblePangEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_BubblePangEffect")));
			CBubblePangEffect* pBubblePangEffect2 = static_cast<CBubblePangEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_BubblePangEffect"),1));

			if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_HIT_SIT_ELECTIVICITY_90AGREE].ActionIndex || m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_HIT_SIT_ELECTIVICITY].ActionIndex  )
			{

				CFish* pFish = static_cast<CFish*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Fish")));
				pFish->Set_FishRender(true);

				/*CFish* pFish = static_cast<CFish*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Fish")));
				pFish->Set_FishRender(true);
				_int getHp = pGameInstance->Get_Hp();
				if (getHp == 5)
					pFish->Set_FishRender(false);
				if (getHp == 2)
					pFish->Set_FishRender(false);
				if (getHp == -1)
					pFish->Set_FishRender(false);*/


					pBubblePangEffect->IsRender(true);
					 pBubblePangEffect2->IsRender(true);
					 m_IsWhite = true;

					 if (m_iColorChage % 5 == 0)
						 m_IsWhite = true;
					 else
					 {
						 m_IsWhite = false;

					 }

					 if (m_iColorChage == 10)
						 m_iColorChage = 0;
					 else
						 m_iColorChage++;

					 CElectric* pElectric = static_cast<CElectric*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ElectircEffect"),2));
					 pElectric->IsRender(true);
					 CElectric* pElectric2 = static_cast<CElectric*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ElectircEffect"), 3));
					 pElectric2->IsRender(true);


					 CGlowBlue2* pGlowBlue = static_cast<CGlowBlue2*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrowBlue2")));
					 pGlowBlue->IsRender(true);
					 CGlowBlue2* pGlowBlue2 = static_cast<CGlowBlue2*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrowBlue2"),1));
					 pGlowBlue2->IsRender(true);


					/* CPangEffect* pPang = static_cast<CPangEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Pang")));
					 pPang->IsRender(true);*/

			}
			else
			{
				/*CPangEffect* pPang = static_cast<CPangEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Pang")));
				pPang->IsRender(false);*/



				pBubblePangEffect->IsRender(false);
				pBubblePangEffect2->IsRender(false);
				m_IsWhite = false;


				CElectric* pElectric = static_cast<CElectric*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ElectircEffect"), 2));
				pElectric->IsRender(false);
				CElectric* pElectric2 = static_cast<CElectric*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ElectircEffect"), 3));
				pElectric2->IsRender(false);

				CGlowBlue2* pGlowBlue = static_cast<CGlowBlue2*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrowBlue2")));
				pGlowBlue->IsRender(false);
				CGlowBlue2* pGlowBlue2 = static_cast<CGlowBlue2*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrowBlue2"),1));
				pGlowBlue2->IsRender(false);
			}



			//항상 y는 0
			_vector m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 fPos;
			XMStoreFloat3(&fPos, m_vPos);
			if (m_iReal_Time_tAnimIndex != m_AnimationInfo[SANDY_HIT_SIT_ELECTIVICITY].ActionIndex)//전기 피격당하기만 빼고 
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x, 0, fPos.z, 1));


			//Collider
			m_pColliderCom[TYPE_OBB]->Update(m_pTransformCom->Get_WorldMatrix());
			m_pColliderComSpring[TYPE_OBB]->Update(m_pTransformCom->Get_WorldMatrix());
			m_pColliderComSpring2[TYPE_OBB]->Update(m_pTransformCom->Get_WorldMatrix());
			m_pColliderComFallDown[TYPE_OBB]->Update(m_pTransformCom->Get_WorldMatrix());
		}
		else//연출
		{
			if (pCamera->Get_m_bIsDirectorCount() == 0)
			{
				m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[SANDY_INTRO].ActionIndex);
				m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta, m_AnimationInfo[SANDY_INTRO].ActionIndex, false, m_AnimationInfo[SANDY_PROVOKE_HAND].ActionIndex);
			}
			if (pCamera->Get_m_bIsDirectorCount() == 3)
			{
				m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[SANDY_DEATH].ActionIndex);
				m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta, m_AnimationInfo[SANDY_DEATH].ActionIndex, false, m_AnimationInfo[SANDY_DEATH].ActionIndex);
			}
			if (pCamera->Get_m_bIsDirectorCount() == 4)
			{
				m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[SANDY_DEATH_STOP].ActionIndex);
				m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta, m_AnimationInfo[SANDY_DEATH_STOP].ActionIndex, false, m_AnimationInfo[SANDY_DEATH_STOP].ActionIndex);
			}
			
		}
	}

		
		Safe_Release(pGameInstance);

	
}

void CBoss_Sandy_Body::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == pGameInstance->isIn_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 2.f))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}


	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CBoss_Sandy_Body::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pVIBufferCom->Get_NumMeshContainers();


	

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (i == 0)
		{
			if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

			if (m_IsWhite)
			{
				CBoss_Sandy_Head2* pHead2 = static_cast<CBoss_Sandy_Head2*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head2")));
				pHead2->Set_IsShaderNum(2);
				
					m_pVIBufferCom->Render(i, m_pShaderCom, 2);

			}
			else
			{
				CBoss_Sandy_Head2* pHead2 = static_cast<CBoss_Sandy_Head2*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head2")));
				pHead2->Set_IsShaderNum(0);
				m_pVIBufferCom->Render(i, m_pShaderCom, 0);

			}

			RELEASE_INSTANCE(CGameInstance);

		}
		else
		{
			if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_ATTACK_SPRING2].ActionIndex || m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_ATTACK_SPRING1].ActionIndex)
			{
				if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
					return E_FAIL;

				m_pVIBufferCom->Render(i, m_pShaderCom, 0);
			}
				
		}
	}

	
#ifdef _DEBUG

	//m_pRendererCom->Add_DebugGroup(m_pColliderCom[TYPE_AABB]);
	m_pRendererCom->Add_DebugGroup(m_pColliderCom[TYPE_OBB]);
	//m_pRendererCom->Add_DebugGroup(m_pColliderCom[TYPE_SPHERE]);
	


	m_pColliderCom[TYPE_OBB]->Render();
	if (IsSpringAttacking)
	{
		m_pRendererCom->Add_DebugGroup(m_pColliderComSpring[TYPE_OBB]);
		m_pRendererCom->Add_DebugGroup(m_pColliderComSpring2[TYPE_OBB]);
	}
	if(IsSlamAttackCollision)
		m_pRendererCom->Add_DebugGroup(m_pColliderComFallDown[TYPE_OBB]);

#endif // _DEBUG

	return S_OK;
}

void CBoss_Sandy_Body::Set_IsSlamAttackCollision()
{
	tempPattern = m_pattern.m_vPatternList1_5_Hit;
	nextAnim = tempPattern.front();
	i = 0;


	//CTransform* m_pHead2Transform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Mesh_scoreboard_broken"), TEXT("Com_Transform"));
	//m_vScoreBoardPos = m_pHead2Transform->Get_State(CTransform::STATE_POSITION);


	//m_pTransformCom->


	/*IsScorBoxAttack = false;
	IsScorBoxAttack = true;*/
}

void CBoss_Sandy_Body::IntroShake()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera_Free* pCamera = nullptr;
	pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera")));
	_introcount += 1;

	if (_introcount > 8 && _introcount <= 12)
	{
		//_introcount = 0;
		
			CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_ftstp_Toe_006.ogg", CSoundMgr::MONSTER_FOOT, 0.2f);

		pCamera->Set_Shake(true);
	}


	if (_introcount > 130 && _introcount <= 134)
	{
		CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_ftstp_Toe_006.ogg", CSoundMgr::MONSTER_FOOT, 0.2f);

		pCamera->Set_Shake(true);
	}

	if (_introcount > 134)
	{
	//	CGameInstance::Get_Instance()->PlayMySound(L"Poseidome.ogg", CSoundMgr::PLAYER, 0.1f);
		pGameInstance->PlayBGM(L"Poseidome.ogg", 0.1f);
		_isIntroEnd = true;

	}

	Safe_Release(pGameInstance);
}

HRESULT CBoss_Sandy_Body::SetUp_Components()
{
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Sandy_Body"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


		/* For.Com_Collider_AABB */
	CCollider::COLLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 2.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.0f, 1.0f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pColliderCom[TYPE_AABB], &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Collider_OBB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(5.f,5.f, 5.f);
	ColliderDesc.vTranslation = _float3(0.0f, 3.f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pColliderCom[TYPE_OBB], &ColliderDesc)))
		return E_FAIL;


	/* For.Com_ColliderSpring_OBB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(50.f, 2.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.0f, 1.5f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_ColliderSpring_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pColliderComSpring[TYPE_OBB], &ColliderDesc)))
		return E_FAIL;
	/* For.Com_ColliderSpring2_OBB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(50.f, 2.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.0f, 6.5f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_ColliderSpring2_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pColliderComSpring2[TYPE_OBB], &ColliderDesc)))
		return E_FAIL;

	/* For.Com_ColliderFallDown_OBB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vTranslation = _float3(0.0f, 3.f, 5.f);

	if (FAILED(__super::Add_Component(TEXT("Com_ColliderFallDown_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pColliderComFallDown[TYPE_OBB], &ColliderDesc)))
		return E_FAIL;
	/* For.Com_Collider_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(2.0f, 2.f, 2.f);
	ColliderDesc.vTranslation = _float3(0.0f, 1.0f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pColliderCom[TYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CBoss_Sandy_Body::SetUp_SRV()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_Transformfloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_Transformfloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CBoss_Sandy_Body::ChangePattern(_uint _animPattern)
{
	switch (_animPattern)
	{
	case 0:
		tempPattern = m_pattern.m_vPatternList1_1_Attack;
		break;
	case 1:
		tempPattern = m_pattern.m_vPatternList1_2_Attack;
		break;
	case 2:
		tempPattern = m_pattern.m_vPatternList1_3_Attack;
		break;
	case 3:
		tempPattern = m_pattern.m_vPatternList1_4_Attack;
		break;
	case 4:
		tempPattern = m_pattern.m_vPatternList1_1_Hit;
		break;
	default:
		break;
	}
}

void CBoss_Sandy_Body::Set_Last_Anim(_int _addNum)
{
	m_iCurrentAnimIndex = tempPattern.back() + _addNum;
	i = tempPattern.size()-1 + _addNum;
}

void CBoss_Sandy_Body::Set_First_Anim(_int _addNum)
{
	m_iCurrentAnimIndex = tempPattern.front() + _addNum;
	i = 0;
}

void CBoss_Sandy_Body::Set_Free_Anim(_int select ,_int _addNum)
{
	ChangePattern(select);
	m_iCurrentAnimIndex =  _addNum;
	i = 0;
}

CBoss_Sandy_Body * CBoss_Sandy_Body::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBoss_Sandy_Body*		pInstance = new CBoss_Sandy_Body(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CForkLift");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoss_Sandy_Body::Clone(void * pArg)
{
	CBoss_Sandy_Body*		pInstance = new CBoss_Sandy_Body(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CForkLift");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_Sandy_Body::Free()
{
	__super::Free();
	for (auto& pColliderCom : m_pColliderCom)
		Safe_Release(pColliderCom);

	for (auto& pColliderCom : m_pColliderComSpring)
		Safe_Release(pColliderCom);

	for (auto& pColliderCom : m_pColliderComSpring2)
		Safe_Release(pColliderCom);

	for (auto& pColliderCom : m_pColliderComFallDown)
		Safe_Release(pColliderCom);

	
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

}
