#include "stdafx.h"
#include "..\Public\Player.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "SpongeBob_Foot_L.h"
#include "SpongeBob_Wand.h"
#include "Boss_Sandy_Body.h"
//#include "Ground2.h"
//#include "Bubble.h"
#include "Underpants.h"

#include <iostream>
#include "PoofEffect.h"
#include "Cut.h"
#include "RainBow.h"
#include "Gate.h"





CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CPlayer::CPlayer(const CPlayer & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC TransformDesc(8.f, XMConvertToRadians(90.f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (static_cast<_uint*>(pArg) != nullptr)
		m_Info = *(_uint*)pArg;



	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_bIsRender = true;
	/*CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);*/

	m_AnimationInfo.clear();
	_tchar		pGetPath[MAX_PATH] = L"../../Reference/Data/SpongeBob.dat";
	HANDLE	hFile = CreateFile(pGetPath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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
	}

	//CloseHandle(hFile);


	//pattern
	m_pattern.m_vPatternList1_1_Intro.clear();

	_tchar		pGetPath2[MAX_PATH] = L"../../Reference/Data/Pattern1_Intro_SpongeBob.dat";
	hFile = CreateFile(pGetPath2, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	dwByte = 0;
	_uint patternInfo;

	while (true)
	{
		ReadFile(hFile, &patternInfo, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_pattern.m_vPatternList1_1_Intro.push_back(patternInfo);
	}

//	Safe_Release(pGameInstance);

	CloseHandle(hFile);


	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(45.5f, 0.f, 36.5f, 1));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(22.4, 2.f, -4, 1));
	m_floaGetY = 1.3f;
	m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_IDLE].ActionIndex);
	
	m_iHP = 3;
	return S_OK;
}

void CPlayer::Tick(_double TimeDelta)
{
	//CPlayer_Management*			pPlayer_Management = CPlayer_Management::Get_Instance();

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);                        
	m_IsWhite = false;
	////카메라 정보 얻기
	CCamera_Free* pCamera = nullptr;
	
	if(m_Info != 0)
		pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera")));
	else
		pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_FIELD ,TEXT("Layer_Camera")));

	
	if (pCamera != nullptr)
	{
		if (pCamera->Get_m_bIsDirector_End())
		{
			IsSlam = false;

			if (m_pVIBufferCom->Get_IsFinish())
			{
				IsHit_Field = false;
				IsHit = false;
			}

			IsFoot = false;
			Gravity(TimeDelta);//중력적용
			m_isAttack2 = false;


			if (GetAsyncKeyState('F') & 0x0001)
			{
				CGate* pGate = static_cast<CGate*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Gate"), 2));
				pGate->Set_Open();
				 pGate = static_cast<CGate*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Gate"), 3));
				pGate->Set_Open();
			}

			if (GetAsyncKeyState('M') & 0x0001)
			{
				pGameInstance->Set_Player((_uint)CPlayer_Typedef::PLAYER_KIND::PLAYER_PATRICK);
			}

			if (pGameInstance->Get_Player_Info() == (_uint)CPlayer_Typedef::PLAYER_KIND::PLAYER_SPONGEBOB)
			{

				//**필요한 객체 정보 얻기 ***********************************************************************************//
				_bool bIsWalk = false;
			//XMStoreFloat3(&CameraPos, m_vCameraPos);

				//플레이어 위치 정보 얻기
				_vector m_vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				_float3 playerPos;
				XMStoreFloat3(&playerPos, m_vPlayerPos);





				//보스 정보 얻기
				CBoss_Sandy_Body* pBoss_Sandy_Body = nullptr;
				pBoss_Sandy_Body = static_cast<CBoss_Sandy_Body*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body")));


			


				//************************************************************************************************************//
				if (pBoss_Sandy_Body != nullptr)
				{
					//충돌체크
					if (pBoss_Sandy_Body->Get_IsTouchHead())
					{//피격할 수 있는 애님 일때만 충돌 체크
					 //	충돌체크
						CCollider*	pSlamCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player_Slame"), TEXT("Com_Collider_OBB"));
						if (nullptr == pSlamCollider)
							return;

						if (m_pColliderCom[TYPE_OBB]->Collision_OBB(pSlamCollider))
							IsSlam = true;
					}

					if (pBoss_Sandy_Body->Get_IsSpringAttack_Ing())
					{//피격할 수 있는 애님 일때만 충돌 체크
					 //	충돌체크
						CCollider*	pSlamCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body"), TEXT("Com_ColliderSpring_OBB"));
						if (nullptr == pSlamCollider)
							return;

						if (m_pColliderCom[TYPE_OBB]->Collision_OBB(pSlamCollider))
							IsHit = true;


						CCollider*	pSlamCollider2 = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body"), TEXT("Com_ColliderSpring2_OBB"));
						if (nullptr == pSlamCollider)
							return;

						if (m_pColliderCom[TYPE_OBB]->Collision_OBB(pSlamCollider2))
							IsHit = true;
					}

					if (pBoss_Sandy_Body->Get_IsHandAttack())
					{//피격할 수 있는 애님 일때만 충돌 체크
					 //	충돌체크
						CCollider*	pSlamCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body"), TEXT("Com_Collider_OBB"));
						if (nullptr == pSlamCollider)
							return;

						if (m_pColliderCom[TYPE_OBB]->Collision_OBB(pSlamCollider))
							IsHit = true;
					}

				}
				

			/*	CCollider*	pRoboCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_FIELD, TEXT("Layer_Robo"), TEXT("Com_Collider_OBB"));
				if (nullptr == pRoboCollider)
					return;
				CCollider*	pRoboCollider2 = (CCollider*)pGameInstance->Get_Component(LEVEL_FIELD, TEXT("Layer_Robo"), TEXT("Com_Collider_OBB"),1);
				if (nullptr == pRoboCollider2)
					return;*/


			/*	if (m_pColliderCom[TYPE_OBB]->Collision_OBB(pRoboCollider) || m_pColliderCom[TYPE_OBB]->Collision_OBB(pRoboCollider2))
					IsHit_Field = true;*/


				//**Key에 대한 플레이어 움직임*******************************************************************************//
				static _float m_fAngle = 0.f;//플레이어와 카메라가 움직일 각도

				if (GetKeyState(VK_UP) & 0x8000)
				{
					walkCount++;
					if (walkCount % 10 == 0)
					{
					//	CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER);
						if(!m_Flying)
							CGameInstance::Get_Instance()->PlayMySound(L"SFX_CH_Gary_Ftstp_005.ogg", CSoundMgr::PLAYER, 0.3f);



						if (walkCount >= 20)
							walkCount = 0;
					}
					iHitCount = 0;

					if (m_Info == 0)
						m_floaGetY = m_pNavigationCom2->Get_pos() + 0.2f;


					if (!IsHit)
					{
						if (m_Info == 0)
							m_pTransformCom->Go_Straight(TimeDelta, m_pNavigationCom2);

						else
							m_pTransformCom->Go_Straight(TimeDelta, m_pNavigationCom);

					}

					if (m_bIsJump == false && IsHit == false)
					{
						m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_WALK].ActionIndex);
						bIsWalk = true;
					}

				}

				if (GetKeyState(VK_LEFT) & 0x8000)
				{
					iHitCount = 0;

					if (m_Info == 0)
						m_floaGetY = m_pNavigationCom2->Get_pos() + 0.2f;

					m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.07 * -1.f);


					if (m_bIsJump == false && IsHit == false)
					{
						bIsWalk = true;
					}


				}

				if (GetKeyState(VK_RIGHT) & 0x8000)
				{
					
					
					iHitCount = 0;

					if (m_Info == 0)
						m_floaGetY = m_pNavigationCom2->Get_pos() + 0.2f;

					m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.07);

					if (m_bIsJump == false && IsHit == false)
					{
						bIsWalk = true;
					}

				}

				if (!m_bIsJump)
				{

					if (GetAsyncKeyState(VK_CONTROL) & 0x0001)
					{
						m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_JUMPUP].ActionIndex);
						m_FallSpeed = -20;
						CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER);
						CGameInstance::Get_Instance()->PlayMySound(L"SB_jump_sngl.ogg", CSoundMgr::PLAYER, 0.3f);

					}
				}



				CSpongeBob_Foot_L* pFoot = static_cast<CSpongeBob_Foot_L*>(pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_SpongeBob_Foot_L")));
				CSpongeBob_Wand* pWand = static_cast<CSpongeBob_Wand*>(pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_SpongeBob_Wand")));
				pFoot->Set_IsRender(false);
				pWand->Set_IsRender(false);
				if (GetKeyState(VK_DOWN) & 0x8000)//S키 눌르는 중에 점프하는 중이였다면 하강애니메이션, 발바닥 렌더o
				{
					iHitCount = 0;


					if (m_Info == 0)
						m_floaGetY = m_pNavigationCom2->Get_pos() + 0.2f;


					if (m_bIsJump)
					{
						pWand->Set_IsRender(true);
						pFoot->Set_IsRender(true);
						m_isAttack2 = true;
						m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_BUBLE_LOOP].ActionIndex);

						m_fDown += 0.008;
						IsFoot = true;
					}
					else//아니면 발바닥 렌더x, 걷는 애니메이션
					{
						walkCount++;
						if (walkCount % 10 == 0)
						{
							//	CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER);
							CGameInstance::Get_Instance()->PlayMySound(L"SFX_CH_Gary_Ftstp_005.ogg", CSoundMgr::PLAYER, 0.3f);


							if (walkCount >= 20)
								walkCount = 0;
						}
						
						m_fDown = 0.f;
						m_FallSpeed = 0.f;
						if(m_Info == 0)
							m_pTransformCom->Go_Backward(TimeDelta, m_pNavigationCom2);
						else
							m_pTransformCom->Go_Backward(TimeDelta, m_pNavigationCom);


						if (m_bIsJump == false && IsHit == false)
						{
							m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_WALK].ActionIndex);
							bIsWalk = true;
						}
					}
				}



				if (GetKeyState(VK_SHIFT) & 0x8000)
				{
					pWand->Set_IsRender(true);
					IsAttack = true;
					BubbleCreate();
					CRainBow* pRainBow = static_cast<CRainBow*>(pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_RainBow")));
					pRainBow->set_isRender(true);
					CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER_ATTACK);
					CGameInstance::Get_Instance()->PlayMySound(L"SB_bubble_spin_rework.ogg", CSoundMgr::PLAYER_ATTACK, 0.2f);

			
				}
				//*************************************************************************************************************************//


				//**상태에 따른 애니메이션 설정*******************************************************************************************//
				//Update_SpongeBobAnimation함수의 0은 아무 의미 없음 시간날때 잊지말고 지우기
				if (IsAttack)
				{
					//if (m_pVIBufferCom->Get_IsFinish())
						//pWand->Set_IsRender(false);//완드는 보간전까지 랜더
					
					if (m_pVIBufferCom->Get_NextFinish())
					{
						m_iSlowMotionCount = 0.f;
						IsAttack = false;
						m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_IDLE].ActionIndex, false, 0);
					}
					else
					{
						if (m_iSlowMotionCount < 1)
						{
							pWand->Set_IsRender(true);
							m_iSlowMotionCount += 0.03f;

						}
						m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_BUBLE_SPINE].ActionIndex);
						m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta * m_iSlowMotionCount, m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_IDLE].ActionIndex, false, 0);
					}
				}
				else if (m_bIsJump)
					m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta * 0.5, m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_BUBLE_LOOP].ActionIndex, false, 0);
				else if (bIsWalk)
					m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_WALK].ActionIndex, false, 0);
				else if (IsHit || IsHit_Field)
				{
					if (m_Info == 0 && iHitCount == 0)
					{
						//CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER);
						CGameInstance::Get_Instance()->PlayMySound(L"gspatula_sb.ogg", CSoundMgr::PLAYER_HIT, 0.3f);
						if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_PoofEffect"), TEXT("Prototype_GameObject_Effect_PoofEffect"))));
					}
					
							
					iHitCount++;
					cout << "Hit" << iHitCount << endl;

					m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_HIT].ActionIndex);
					m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta /** 0.2f*/, m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_HIT].ActionIndex, false, 0);

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


						if (m_pVIBufferCom->Get_IsFinish()/*m_pVIBufferCom->Get_NextFinish()*/)
						{
							iHitCount = 0;
						//	m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_IDLE].ActionIndex, false, 0);
						}

						if(m_Info!=0)
							pCamera->Set_Shake(true);

				}
				else
				{
					iHitCount = 0;
					m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_IDLE].ActionIndex, false, 0);

				}
				if (pBoss_Sandy_Body != nullptr)
				{
					if (pBoss_Sandy_Body->Get_IsJumpMotion2())//만약 보스가 앉았다면 붕 뜨기
					{

						m_FallSpeed = -20;
						m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_BUBLE_LOOP].ActionIndex);
						m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta * 0.1, m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_BUBLE_LOOP].ActionIndex, false, 0);
					}
					else
						m_fSlowDown = 0.f;
				}
				
				//********* HP관리 **************************************************************************************************//
				//CHp_Management*			pHp_Management = CHp_Management::Get_Instance();

				if (pGameInstance->Get_Hp() < 4)//Max가 아니라면 
				{
					static _uint count = 0;
					if (IsHit)
					{

						if(count == 0)
							CGameInstance::Get_Instance()->PlayMySound(L"gspatula_sb.ogg", CSoundMgr::PLAYER_HIT, 0.31f);

						if (count == 0 && pGameInstance->Get_Hp() != -1)
						{
							//Underpants정보 얻기
							CUnderpants* pUnderpants = static_cast<CUnderpants*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Underpants"), pGameInstance->Get_Hp()));
							pUnderpants->Set_TexutureNum(1);

							if (pGameInstance->Get_Hp() != -1)// 피 깎기
							{
								//CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER);

								pGameInstance->Set_Hp(-1);

							}
						}
						count++;

						//pCamera->Set_Shake(true);
					}
					else
					{
						if (m_pVIBufferCom->Get_NextFinish())
							count = 0;//한번만 피 깎이도록 
					}
					if (IsHit_Field)
					{
						if (count == 0 && pGameInstance->Get_Hp() != -1)
						{
							//Underpants정보 얻기
							CUnderpants* pUnderpants = static_cast<CUnderpants*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Underpants"), pGameInstance->Get_Hp()));
							pUnderpants->Set_TexutureNum(1);

							if (pGameInstance->Get_Hp() != -1)// 피 깎기
							{
								

								pGameInstance->Set_Hp(-1);

							}
						}
						count++;

						//pCamera->Set_Shake(true);
					}
					else
					{
						if (m_pVIBufferCom->Get_NextFinish())
							count = 0;//한번만 피 깎이도록 
					}
				}

			
				m_pColliderCom[TYPE_OBB]->Update(m_pTransformCom->Get_WorldMatrix());
			

			}
			else
			{
				//스폰지밥이 아니면 뚱이니까 뚱이 위치 스폰지밥으로 계속 옮겨놓기

				//CTransform* m_pPatrickPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer"), TEXT("Com_Transform"));


				//m_pTransformCom = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
				//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(59.39, 0.f, 36, 1));
				//_vector m_vm_pPatrickPlayerTransformPos = m_pPatrickPlayerTransform->Get_State(CTransform::STATE_POSITION);



				//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vm_pPatrickPlayerTransformPos);

				//_vector		vPlayerLook = m_pPatrickPlayerTransform->Get_State(CTransform::STATE_LOOK);
				//m_pPatrickPlayerTransform->Set_State(CTransform::STATE_LOOK, vPlayerLook);
			}

		}
		else//연출
		{

			if (pCamera->Get_m_bIsDirectorCount() == 0)
			{
				
				/*static _uint iDelaycount = 0;
				iDelaycount++;*/
				if (m_Info != 0)
				{
					
					if (tempPattern.size() == 0)
						tempPattern = m_pattern.m_vPatternList1_1_Intro;

					m_iCurrentAnimIndex = tempPattern[next];

					if (m_pVIBufferCom->Get_NextFinish())
					{
						m_pVIBufferCom->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
						//m_iReal_Time_tAnimIndex = m_iCurrentAnimIndex;

						if (next == tempPattern.size() - 1)
						{
							nextAnim = tempPattern.front();
							next = 0;
						}
						else
						{
							//else
							//	{
							nextAnim = tempPattern[next + 1];
							
							next++;
						//	CCut* pCut = static_cast<CCut*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cut")));
							
							//}


						}
						m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta, nextAnim, false, next);
					}
					else
					{
						//여기서의 m_iCurrentAnimIndex는 아무 의미 없음 m_pVIBufferCom->Set_CurrentAnimIndex(()은 위의 함수에서 하기 떄문
						m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta, m_iCurrentAnimIndex, false, nextAnim);
					}
				}

			}

		}
	}
	
	
	Safe_Release(pGameInstance);
}

void CPlayer::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	////카메라 정보 얻기
	CCamera_Free* pCamera = nullptr;
	pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera")));
	if(pCamera == nullptr)
		pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Camera")));
	if (m_bIsRender)
	{
		if (pCamera != nullptr)
		{
			if (!pCamera->Get_m_bIsDirector_End() && pCamera->Get_m_bIsDirectorCount() == 0)
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
			else
			{
				if (pGameInstance->Get_Player_Info() == (_uint)CPlayer_Typedef::PLAYER_KIND::PLAYER_SPONGEBOB)
					m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
			}
		}

	}

	

	Safe_Release(pGameInstance);

	
}

HRESULT CPlayer::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pVIBufferCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (m_IsWhite)
			m_pVIBufferCom->Render(i, m_pShaderCom, 2);
		else
			m_pVIBufferCom->Render(i, m_pShaderCom, 0);
	}




#ifdef _DEBUG
	//m_pColliderCom[TYPE_AABB]->Render();
	//m_pColliderCom[TYPE_OBB]->Render();
	m_pRendererCom->Add_DebugGroup(m_pColliderCom[TYPE_OBB]);

	//	m_pColliderCom[TYPE_SPHERE]->Render();
	if(m_Info != 0)
		m_pRendererCom->Add_DebugGroup(m_pNavigationCom);
	else
		m_pRendererCom->Add_DebugGroup(m_pNavigationCom2);


#endif // _DEBUG

	return S_OK;

}


HRESULT CPlayer::SetUp_Components()
{
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Melody"), (CComponent**)&m_pVIBufferCom)))
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

	ColliderDesc.vScale = _float3(1.2f, 1.f, 1.2f);
	ColliderDesc.vTranslation = _float3(0.0f,1.f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pColliderCom[TYPE_OBB], &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Collider_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(2.0f, 2.f, 2.f);
	ColliderDesc.vTranslation = _float3(0.0f, 1.0f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pColliderCom[TYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::SetUp_SRV()
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

HRESULT CPlayer::Set_Navigation()
{
	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));

	NaviDesc.iCurrentIndex = 0;
	if (m_Info != 0)
	{
		if (FAILED(__super::Add_Component(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Add_Component(TEXT("Com_Navigation2"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation2"), (CComponent**)&m_pNavigationCom2, &NaviDesc)))
			return E_FAIL;
	}
}

HRESULT CPlayer::BubbleCreate()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//m_iBubbleCount++;

	//if (m_iBubbleCount % 2 == 0)
//	{

		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_STATIC, TEXT("Layer_Bubbleffect"), TEXT("Prototype_GameObject_Effect_Bubble"), &m_Info)))
			return E_FAIL;
		
//	}
	
	/*if (m_iBubbleCount > 20)
		m_iBubbleCount = 0;*/

	Safe_Release(pGameInstance);
	return S_OK;
}



void CPlayer::Gravity(_double TimeDelta)
{

	//중력
	m_FallSpeed += TimeDelta * GravityPower;
	//cout << "GravityPower : " << GravityPower << endl;
	if (m_FallSpeed >= MaxSpeedPerSeconds)
	{
		m_FallSpeed = MaxSpeedPerSeconds;
		
	}
	

	_vector pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 fPos;
	XMStoreFloat3(&fPos, pos);
	fPos.y -= m_FallSpeed * TimeDelta + m_fDown;
	//cout << "fPos.y : " << fPos.y << endl;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x, fPos.y, fPos.z, 1.f));

	//평상시엔 IDLE 모션
	m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_IDLE].ActionIndex);


	if (m_Info == 0)
	{
	
		if (m_floaGetY < +0.2f)
			m_floaGetY = 0.2f;

		if (fPos.y < m_floaGetY + 0.2f)//플레이어가 네비메쉬 Y보다 더 아래라면 0으로 맞추기
		{
			m_bIsJump = false;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x, m_floaGetY + 0.2f, fPos.z, 1.f));
		}
		else if(fPos.y > m_floaGetY + 1.2f)//아니라면 공중에 띄워져 있는것 : 점프 모션 취하기
		{
			m_bIsJump = true;
			m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_JUMPUP].ActionIndex);
		}

		if (fPos.y > m_floaGetY + 2.f)
			m_Flying = true;//
		else
			m_Flying = false;


	}
	else
	{
		if (fPos.y < 0)//플레이어가 0보다 더 아래라면 0으로 맞추기
		{
			m_bIsJump = false;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x, 0, fPos.z, 1.f));
		}
		else//아니라면 공중에 띄워져 있는것 : 점프 모션 취하기
		{
			m_bIsJump = true;
			m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_JUMPUP].ActionIndex);
		}

		if (fPos.y >2.f)//
		{
			/*if (m_FlyingCount == 0)
				CGameInstance::Get_Instance()->PlayMySound(L"SBG01001_a.ogg", CSoundMgr::PLAYER, 0.1f);*/

			m_Flying = true;
			m_FlyingCount++;
		}
		else
		{
			if (m_FlyingCount > 0)
			{
				//이팩트
			}

			m_FlyingCount = 0;
			m_Flying = false;

		}
	}
	//if (fPos.y < m_floaGetY + 0.5f)//플레이어가 0보다 더 아래라면 0으로 맞추기
	//{
	//	m_bIsJump = false;
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x, m_floaGetY + 0.5f, fPos.z, 1.f));
	//}
	//else//아니라면 공중에 띄워져 있는것 : 점프 모션 취하기
	//{
	//	m_bIsJump = true;
	//	m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::SPONGEBOB_ACTION::SPONGEBOB_JUMPUP].ActionIndex);
	//}

	//if (fPos.y >2.f)
	//	m_Flying = true;//
	//else
	//	m_Flying = false;
}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayer*		pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	for (auto& pColliderCom : m_pColliderCom)
		Safe_Release(pColliderCom);

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pNavigationCom2);

	//	Safe_Release(m_pColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

}
