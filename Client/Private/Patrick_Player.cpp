#include "stdafx.h"
#include "..\Public\Patrick_Player.h"
#include "GameInstance.h"
#include "Camera_Free.h"
//#include "SpongeBob_Foot_L.h"
//#include "SpongeBob_Wand.h"
#include "Boss_Sandy_Body.h"
//#include "Ground2.h"
//#include "Bubble.h"
#include "Underpants.h"

#include "Boss_Sandy_Head2.h"


#include <iostream>



CPatrick_Player::CPatrick_Player(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CPatrick_Player::CPatrick_Player(const CPatrick_Player & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CPatrick_Player::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPatrick_Player::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC TransformDesc(8.f, XMConvertToRadians(90.f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	

	m_AnimationInfo.clear();
	_tchar		pGetPath[MAX_PATH] = L"../../Reference/Data/Patrick.dat";
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
	_tchar		pGetPath2[MAX_PATH] = L"../../Reference/Data/Pattern1_Intro_Patrick.dat";

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

	

	CloseHandle(hFile);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(47.f, 0.f, 37.f, 1));
	m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_IDLE].ActionIndex);

	//m_iHP = 3;
	return S_OK;
}

void CPatrick_Player::Tick(_double TimeDelta)
{
	m_IsWhite = false;
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	/*_vector		vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	vLook = XMVector3Normalize(vLook);*/
	////카메라 정보 얻기
	CCamera_Free* pCamera = nullptr;
	pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera")));

	if (pCamera != nullptr)
	{
		if (pCamera->Get_m_bIsDirector_End())
		{
			static _bool move = false;

			IsSlam = false;

			if (m_pVIBufferCom->Get_IsFinish())
				IsHit = false;

			IsFoot = false;
			Gravity(TimeDelta);//중력적용
			m_isAttack2 = false;
			
			m_isPickUpStart_SandyHead = false;
			if (GetAsyncKeyState('N') & 0x0001)
			{
				pGameInstance->Set_Player((_uint)CPlayer_Typedef::PLAYER_KIND::PLAYER_SPONGEBOB);
			}






			if (pGameInstance->Get_Player_Info() == (_uint)CPlayer_Typedef::PLAYER_KIND::PLAYER_PATRICK)
			{
				//**필요한 객체 정보 얻기 ***********************************************************************************//
				_bool bIsWalk = false;
				////카메라 정보 얻기
				//CCamera_Free* pCamera = nullptr;
				//pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera")));

				//CTransform* m_pCameraTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Com_Transform"));
				//_vector m_vCameraPos = m_pCameraTransform->Get_State(CTransform::STATE_POSITION);
				//_float3 CameraPos;
				//XMStoreFloat3(&CameraPos, m_vCameraPos);

				//플레이어 위치 정보 얻기
				_vector m_vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				_float3 playerPos;
				XMStoreFloat3(&playerPos, m_vPlayerPos);


			/*	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
				cout << "playerPos fPosx : " << playerPos.x << endl;
				cout << "playerPos fPosY : " << playerPos.y << endl;
				cout << "playerPos fPosZ : " << playerPos.z << endl;
				cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;*/



				//보스 정보 얻기
				CBoss_Sandy_Body* pBoss_Sandy_Body = nullptr;
				pBoss_Sandy_Body = static_cast<CBoss_Sandy_Body*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body")));

				CBoss_Sandy_Head2* pBoss_Sandy_Head2 = nullptr;
				pBoss_Sandy_Head2 = static_cast<CBoss_Sandy_Head2*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head2")));
				CTransform* m_Boss_Sandy_Head2Transform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head2"), TEXT("Com_Transform"));
				_vector vBoss_Sandy_Head2TransformPos = m_Boss_Sandy_Head2Transform->Get_State(CTransform::STATE_POSITION);
				_float3 fBoss_Sandy_Head2pos;
				XMStoreFloat3(&fBoss_Sandy_Head2pos, vBoss_Sandy_Head2TransformPos);



				////그라운드 위치 정보 얻기
				//CGround2* pGround2 = nullptr;
				//pGround2 = static_cast<CGround2*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Ground2")));

				//CTransform* m_pGround2Transform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Ground2"), TEXT("Com_Transform"));
				//_vector m_vGround2TransformPos = m_pGround2Transform->Get_State(CTransform::STATE_POSITION);
				//_float3 pGround2Pos;
				//XMStoreFloat3(&pGround2Pos, m_vGround2TransformPos);

				//Buble정보 얻기
			//	CBubble* pBubble = static_cast<CBubble*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Bubble")));



				//************************************************************************************************************//

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


				//if (pBoss_Sandy_Body->Get_IsSlamAttackCollision())
				//{//피격할 수 있는 애님 일때만 충돌 체크
				// //	충돌체크
				//	CCollider*	pSandyCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body"), TEXT("Com_ColliderFallDown_OBB"));
				//	if (nullptr == pSandyCollider)
				//		return;

				//	if (m_pColliderCom[TYPE_OBB]->Collision_OBB(pSandyCollider))
				//		IsHit = true;
				//}

			
				CCollider*	pBoxollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Mesh_scoreboard_broken"), TEXT("Com_Collider_OBB"));
				if (nullptr == pBoxollider)
					return;

				if (m_pColliderCom[TYPE_OBB]->Collision(pBoxollider))
				{
					int a = 0;

				}

				//**Key에 대한 플레이어 움직임*******************************************************************************//
				static _float m_fAngle = 0.f;//플레이어와 카메라가 움직일 각도


				if (GetAsyncKeyState('Z') & 0x0001)
				{
					if (pBoss_Sandy_Head2->Get_IsRender())
					{
						CCollider*	pHead2Collider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head2"), TEXT("Com_Collider_OBB"));
						if (nullptr == pHead2Collider)
							return;
						//뚱이와 샌디머리 충돌체크 
						if (m_pColliderCom[TYPE_OBB]->Collision_OBB(pHead2Collider))
						{
							m_isPickUp_SandyHead = !m_isPickUp_SandyHead;


							if (m_isPickUp_SandyHead)
							{
								CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER);
								CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_HeadPop_006.ogg", CSoundMgr::PLAYER, 0.3f);

								m_isPickUpStart_SandyHead = true;
								m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_PICKUP].ActionIndex);

							}
							else
							{
								CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER);
								CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_HeadPop_007.ogg", CSoundMgr::PLAYER, 0.3f);
								pBoss_Sandy_Head2->Set_Move();
								m_isThrow_SandyHead = true;
								m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_THROW].ActionIndex);

							}

						}


					}

				}


				if (GetKeyState(VK_SHIFT) & 0x8000)
				{
					//pWand->Set_IsRender(true);
					IsAttack = true;
					CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER);
					CGameInstance::Get_Instance()->PlayMySound(L"Pat_lift3B.ogg", CSoundMgr::PLAYER, 0.1f);
					
				}


				if (GetKeyState(VK_UP) & 0x8000)
				{
					walkCount++;
					if (walkCount % 10 == 0)
					{
						//	CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER);
						if (!m_Flying)
							CGameInstance::Get_Instance()->PlayMySound(L"Pat_step2.ogg", CSoundMgr::PLAYER, 0.3f);



						if (walkCount >= 20)
							walkCount = 0;
					}

					
					if (!IsHit)
						m_pTransformCom->Go_Straight(TimeDelta, m_pNavigationCom);


					if (m_bIsJump == false && IsHit == false)
					{
						if(m_isPickUp_SandyHead)
							m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_PICKUP_WALK].ActionIndex);
						else
							m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_WALK].ActionIndex);

						bIsWalk = true;
					}

				}

				if (GetKeyState(VK_LEFT) & 0x8000)
				{
					m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.07 * -1.f);


					if (m_bIsJump == false && IsHit == false)
					{
						bIsWalk = true;
					}


				}

				if (GetKeyState(VK_RIGHT) & 0x8000)
				{
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
						CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER);
						CGameInstance::Get_Instance()->PlayMySound(L"Pat_jump_sngl.ogg", CSoundMgr::PLAYER, 0.2f);


						if (m_isPickUp_SandyHead)
							m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_PICKUP_JUMP_UP].ActionIndex);
						else
							m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_JUMP_UP].ActionIndex);

						m_FallSpeed = -20;

					}
				}



				/*CSpongeBob_Foot_L* pFoot = static_cast<CSpongeBob_Foot_L*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SpongeBob_Foot_L")));
				CSpongeBob_Wand* pWand = static_cast<CSpongeBob_Wand*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SpongeBob_Wand")));
				pFoot->Set_IsRender(false);
				pWand->Set_IsRender(false);*/
				if (GetKeyState(VK_DOWN) & 0x8000)//S키 눌르는 중에 점프하는 중이였다면 하강애니메이션, 발바닥 렌더o
				{
					if (m_bIsJump)
					{
						/*pWand->Set_IsRender(true);
						pFoot->Set_IsRender(true);*/
						m_isAttack2 = true;
						m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_SLAM_LOOP].ActionIndex);

						m_fDown += 0.008;
						IsFoot = true;
					}
					else//아니면 발바닥 렌더x, 걷는 애니메이션
					{
						walkCount++;
						if (walkCount % 10 == 0)
						{
							//	CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER);
							CGameInstance::Get_Instance()->PlayMySound(L"Pat_step2.ogg", CSoundMgr::PLAYER, 0.3f);


							if (walkCount >= 20)
								walkCount = 0;
						}

						m_fDown = 0.f;
						m_FallSpeed = 0.f;

						m_pTransformCom->Go_Backward(TimeDelta, m_pNavigationCom);

						if (m_bIsJump == false && IsHit == false)
						{
							if (m_isPickUp_SandyHead)
								m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_PICKUP_WALK].ActionIndex);
							else
								m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_WALK].ActionIndex);

							bIsWalk = true;
						}
					}
				}



				if (GetKeyState(VK_SHIFT) & 0x8000)
				{
					//pWand->Set_IsRender(true);
					IsAttack = true;
				}
				//*************************************************************************************************************************//


				//**상태에 따른 애니메이션 설정*******************************************************************************************//
				//Update_SpongeBobAnimation함수의 0은 아무 의미 없음 시간날때 잊지말고 지우기
				if (IsAttack)
				{
					//if (m_pVIBufferCom->Get_IsFinish())
					//	pWand->Set_IsRender(false);//완드는 보간전까지 랜더

					if (m_pVIBufferCom->Get_NextFinish())
					{
						IsAttack = false;
						if (m_isPickUp_SandyHead)
							m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_PICKUP_IDLE].ActionIndex);
						else
							m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_IDLE].ActionIndex, false, 0);

					}
					else
					{
						m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_ATTACK].ActionIndex);
						if (m_isPickUp_SandyHead)
							m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_PICKUP_IDLE].ActionIndex);
						else
							m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_IDLE].ActionIndex, false, 0);

					}
				}
				else if (m_bIsJump)
					m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta * 0.5, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_SLAM_LOOP].ActionIndex, false, 0);
				else if (bIsWalk)
				{
					if (m_isPickUp_SandyHead)
						m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_PICKUP_WALK].ActionIndex, false, 0);
					else
						m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_WALK].ActionIndex, false, 0);
				}
				else if (IsHit)
				{

					m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_HIT].ActionIndex);
					m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_HIT].ActionIndex, false, 0);
					
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
					
					if (m_pVIBufferCom->Get_NextFinish())
					{
						if (m_isPickUp_SandyHead)
							m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_PICKUP_IDLE].ActionIndex);
						else
							m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_IDLE].ActionIndex, false, 0);

					}
					
				}
				else if (m_isPickUp_SandyHead)//이건 안해도 될긋
				{
					if (m_isPickUpStart_SandyHead)//만약 뚱이가 샌디머리 처음 들었다면
					{
						m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_PICKUP].ActionIndex);
						m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta/** 0.2f*/, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_PICKUP].ActionIndex, false, 0);
						if (m_pVIBufferCom->Get_NextFinish())
						{
							m_isPickUpStart_SandyHead = false;
							m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_PICKUP_IDLE].ActionIndex, false, 0);
						}
					}
					else
					{
						m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_PICKUP_IDLE].ActionIndex);
						m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_PICKUP_IDLE].ActionIndex, false, 0);

					}
				}
				else if (m_isThrow_SandyHead)//만약 뚱이가 쌘디 머리 던졌다면
				{
					m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_THROW].ActionIndex);
					m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta * 2, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_THROW].ActionIndex, false, 0);
					if (m_pVIBufferCom->Get_NextFinish())
					{
						
						m_isThrow_SandyHead = false;
						m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_IDLE].ActionIndex);
						m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_IDLE].ActionIndex, false, 0);

					}
				}
				else
					m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_IDLE].ActionIndex, false, 0);

				

				if (pBoss_Sandy_Body->Get_IsJumpMotion2())//만약 보스가 앉았다면 붕 뜨기
				{

					m_FallSpeed = -20;
					m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_JUMP_DW].ActionIndex);
					m_pVIBufferCom->Update_SpongeBobAnimation(TimeDelta * 0.1, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_JUMP_DW].ActionIndex, false, 0);
				}
				else
					m_fSlowDown = 0.f;





				if (m_isPickUp_SandyHead)//샌디머리 뚱이에 올리기
				{

					/*_vector m_vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
					_float3 playerPos;
					XMStoreFloat3(&playerPos, m_vPlayerPos);*/


					m_Boss_Sandy_Head2Transform->Set_State(CTransform::STATE_POSITION, m_vPlayerPos + XMVectorSet(0, -2.2f, 0, 1));
					
				}


//********* HP관리 **************************************************************************************************//


				//	CHp_Management*			pHp_Management = CHp_Management::Get_Instance();

				if (pGameInstance->Get_Hp() < 4)//Max가 아니라면 
				{
					static _uint count = 0;
					if (IsHit)
					{
						if (count == 0 && pGameInstance->Get_Hp() != -1)
						{
							
								CGameInstance::Get_Instance()->PlayMySound(L"gspatula_pat.ogg", CSoundMgr::PLAYER_HIT, 0.3f);

							//Underpants정보 얻기
							CUnderpants* pUnderpants = static_cast<CUnderpants*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Underpants"), pGameInstance->Get_Hp()));
							pUnderpants->Set_TexutureNum(1);

							if (pGameInstance->Get_Hp() != -1)// 피 깎기
								pGameInstance->Set_Hp(-1);
						}
						count++;


					}
					else
					{
						if (m_pVIBufferCom->Get_NextFinish())
						{
							count = 0;//한번만 피 깎이도록 
						}
					}

//*****************************//

					if (m_IsShake)
					{
						//if (move)
						//	{

						//CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
						//Safe_AddRef(pGameInstance);

						//static _int m_iCount = 0;
						////카메라 정보 얻기
						//CCamera_Free* pCamera = nullptr;
						//pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera")));
						////m_iCount++;
						//CTransform* m_pCameraTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Com_Transform"));
						//_vector m_vCameraPos = m_pCameraTransform->Get_State(CTransform::STATE_POSITION);
						//_float3 CameraPos;
						//XMStoreFloat3(&CameraPos, m_vCameraPos);

						//_float fStomps = 3.f;
						//_float fY = 35 * TimeDelta * fStomps * 0.5;
						//m_pCameraTransform->Set_State(CTransform::STATE_POSITION, /*XMVectorSet(CameraPos.x) +*/ XMVectorSet(CameraPos.x, fY + playerPos.y + 11, CameraPos.z, 1));
						//if (m_iCount == 10)
						//{
						//	move = false;
						//	m_iCount = 0;
						//}

						//Safe_Release(pGameInstance);
						//}
					}




				}

				m_pColliderCom[TYPE_OBB]->Update(m_pTransformCom->Get_WorldMatrix());
			}
			else
			{
				//뚱이 아니면 스폰지밥이니까 스폰지밥 위치 스폰지밥으로 계속 옮겨놓기

				CTransform* m_pSpongeBobPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));


				_vector m_vm_pSpongeBobPlayerTransformPos = m_pSpongeBobPlayerTransform->Get_State(CTransform::STATE_POSITION);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vm_pSpongeBobPlayerTransformPos);

				_vector		vPlayerLook = m_pSpongeBobPlayerTransform->Get_State(CTransform::STATE_LOOK);
				m_pSpongeBobPlayerTransform->Set_State(CTransform::STATE_LOOK, vPlayerLook);


			}
		}
		else//연출
		{
			
			if (pCamera->Get_m_bIsDirectorCount() == 0)
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
			else if (pCamera->Get_m_bIsDirectorCount() == 1)
			{
				m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_ROUND2_2].ActionIndex);
				m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta , m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_ROUND2_2].ActionIndex, false, nextAnim);
			}

			else if (pCamera->Get_m_bIsDirectorCount() == 2)
			{

				m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_ROUND3_1].ActionIndex);
				m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta, m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_ROUND3_1].ActionIndex, false, nextAnim);
			}
			
		}

		
	}
	//CTransform* m_playerTrassnsform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head2"), TEXT("Com_Transform"));
	//_vector		vLosok = m_playerTrassnsform->Get_State(CTransform::STATE_LOOK);
	//m_pTransformCom->Chase(vLosok, TimeDelta* 0.7, 0.f);

	Safe_Release(pGameInstance);
}

void CPatrick_Player::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	////카메라 정보 얻기
	CCamera_Free* pCamera = nullptr;
	pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera")));
	
	if (pCamera != nullptr)//예외처리
	{
		if (!pCamera->Get_m_bIsDirector_End())
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		else
		{//현재 플레이어 정보얻기
			if (pGameInstance->Get_Player_Info() == (_uint)CPlayer_Typedef::PLAYER_KIND::PLAYER_PATRICK)
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		}
	}
	Safe_Release(pGameInstance);
}

HRESULT CPatrick_Player::Render()
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
			m_pVIBufferCom->Render(i, m_pShaderCom, 3);
		else
			m_pVIBufferCom->Render(i, m_pShaderCom, 0);
	}


#ifdef _DEBUG
	//m_pColliderCom[TYPE_AABB]->Render();
	m_pRendererCom->Add_DebugGroup(m_pColliderCom[TYPE_OBB]);

//	m_pColliderCom[TYPE_OBB]->Render();
	//	m_pColliderCom[TYPE_SPHERE]->Render();
	m_pNavigationCom->Render();
#endif // _DEBUG

	return S_OK;

}


HRESULT CPatrick_Player::SetUp_Components()
{
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Patrick_Player"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	
	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));

	NaviDesc.iCurrentIndex = 0;

	if (FAILED(__super::Add_Component(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
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

HRESULT CPatrick_Player::SetUp_SRV()
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



void CPatrick_Player::Gravity(_double TimeDelta)
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
	if(m_isPickUp_SandyHead)
		m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_PICKUP_IDLE].ActionIndex);
	else
		m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_IDLE].ActionIndex);


	if (fPos.y < 0)//플레이어가 0보다 더 아래라면 0으로 맞추기
	{
		m_bIsJump = false;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x, 0, fPos.z, 1.f));
	}
	else//아니라면 공중에 띄워져 있는것 : 점프 모션 취하기
	{
		m_bIsJump = true;
		m_pVIBufferCom->Set_CurrentAnimIndex(m_AnimationInfo[CPlayer_Typedef::PATRICK_ACTION::PATRICK_JUMP_UP].ActionIndex);
	}

	if (fPos.y >2.f)//
	{
		/*if(m_FlyingCount == 0)
			CGameInstance::Get_Instance()->PlayMySound(L"psgb01033_c.ogg", CSoundMgr::PLAYER, 0.1f);*/

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

CPatrick_Player * CPatrick_Player::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPatrick_Player*		pInstance = new CPatrick_Player(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CPatrick_Player");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPatrick_Player::Clone(void * pArg)
{
	CPatrick_Player*		pInstance = new CPatrick_Player(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPatrick_Player");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPatrick_Player::Free()
{
	__super::Free();

	for (auto& pColliderCom : m_pColliderCom)
		Safe_Release(pColliderCom);

	Safe_Release(m_pNavigationCom);
	
	//	Safe_Release(m_pColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

}
