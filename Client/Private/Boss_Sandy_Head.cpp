#include "stdafx.h"
#include "..\Public\Boss_Sandy_Head.h"
#include "GameInstance.h"

#include <iostream>
#include "Boss_Sandy_Body.h"
#include "Player.h"
#include "Boss_HPBar.h"
#include "Boss_HP_Energy.h"
#include "Camera_Free.h"

#include "Effect_Point.h"
#include "Fish.h"

CBoss_Sandy_Head::CBoss_Sandy_Head(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
	//	pBoss_Sandy_Body = CBoss_Sandy_Body::Create(m_pDevice, m_pContext);

}

CBoss_Sandy_Head::CBoss_Sandy_Head(const CBoss_Sandy_Head & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CBoss_Sandy_Head::NativeConstruct_Prototype()
{


	return S_OK;
}

HRESULT CBoss_Sandy_Head::NativeConstruct(void * pArg)
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

	////Safe_Release(pGameInstance);



	////m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(47.f, 0.f, 45.f, 1));
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

void CBoss_Sandy_Head::Tick(_double TimeDelta)
{
	IsHit = false;
	//샌디바디 정보 얻기 
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	////카메라 정보 얻기
	CCamera_Free* pCamera = nullptr;
	pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera")));


	if (GetAsyncKeyState('B') & 0x0001)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CBoss_HP_Energy* pBoss_HP_Energy = static_cast<CBoss_HP_Energy*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Laye_Eenergy"), m_iHP));
		//pBoss_HP_Energy->Set_Dead();
		pBoss_HP_Energy->Set_IsEnd();
		if (m_iHP != -1)// 피 깎기
			m_iHP--;
		if (m_iHP == 5)
		{
			pCamera->Set_m_bIsDirector_End(false);
			pGameInstance->Set_Player((_uint)CPlayer_Typedef::PLAYER_KIND::PLAYER_PATRICK);
			pCamera->Set_m_bIsDirectorCount();
		}
		Safe_Release(pGameInstance);
	}


	if (pCamera != nullptr)
	{
		if (pCamera->Get_m_bIsDirector_End())
		{
			CBoss_Sandy_Body* pBoss_Sandy_Body = nullptr;
			pBoss_Sandy_Body = static_cast<CBoss_Sandy_Body*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body")));

			CTransform* m_pBodyTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body"), TEXT("Com_Transform"));
			_vector m_vBodyPos = m_pBodyTransform->Get_State(CTransform::STATE_POSITION);

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vBodyPos);

			CPlayer* pPlayer = nullptr;
			CTransform* pPlayerTransform = nullptr;

			if (pGameInstance->Get_Player_Info() == 0)
			{
				pPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_Player")));
				pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
			}
			else
			{
				pPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer")));
				pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer"), TEXT("Com_Transform"));
			}

			_vector PlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
			_float3 fplayerPos;
			XMStoreFloat3(&fplayerPos, PlayerPos);

			m_iDeletaTimeEdit = 1;





			//패턴 1 걷다가 점프공격
			m_iCurrentAnimIndex = pBoss_Sandy_Body->Get_CurrentAnimIndex();

			if (m_iCurrentAnimIndex == m_AnimationInfo[SANDY_PROVOKE_WALK_THRUST_HEAD].ActionIndex)
			{

				iFootCount++;
				if (iFootCount % 40 == 0)
				{
					CGameInstance::Get_Instance()->StopSound(CSoundMgr::MONSTER_FOOT);
					CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_ftstp_Toe_006.ogg", CSoundMgr::MONSTER_FOOT, 0.5f);

				}

				if (iFootCount >= 80)
					iFootCount = 0;
			}
			else
				iFootCount = 0;

			if (m_pVIBufferCom->Get_NextFinish())
			{

				if (m_iCurrentAnimIndex == m_AnimationInfo[SANDY_PROVOKE_WALK_THRUST_HEAD].ActionIndex ||
					m_iCurrentAnimIndex == m_AnimationInfo[SANDY_HIT_SIT_ELECTIVICITY_90AGREE].ActionIndex)
				{
					m_isRender = false;
					if (m_iCurrentAnimIndex == m_AnimationInfo[SANDY_HIT_SIT_ELECTIVICITY_90AGREE].ActionIndex)
					{

						CGameInstance::Get_Instance()->PlayMySound(L"Elect.ogg", CSoundMgr::MONSTER, 0.5f);

					}
					
					//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0, 0, -5, 1.f));
				}
				else
				{
					m_isRender = true;
					//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0, 0, -5, 1.f));
					
				}
				
				m_pVIBufferCom->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
				m_iReal_Time_tAnimIndex = m_iCurrentAnimIndex;
				m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta * m_iDeletaTimeEdit, pBoss_Sandy_Body->Get_CurrentNextdex(), false, 0);
			}
			else
			{
				//넥스트를 정해준후임 알아서 선혀보간하든 현제 프레임을 하던m_iCurrentAnimIndex 가 결정
				//m_iCurrentAnimIndex 와 nextAnim가 같다면 현제 프레임을 돌린다는거고 
				//다르다면 선형보간중인것임.
				/*------------------ <*** 패턴 1Page ***> -----------------------------*/
				//만약 현제 애님을 돌린다면(선형보간중이 아니라면)
				if (m_iCurrentAnimIndex == pBoss_Sandy_Body->Get_CurrentNextdex())
				{

					//현재 동작하고 있는 애님이 조건문일 경우 플레이어를 향해 움직이기 (플레이이어가 점프 중 일땐 제외)
					if (pPlayer->Get_IsJump() == false && (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_WALK].ActionIndex/* ||
																															 m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_RUN].ActionIndex*/))
					{
						_vector SandyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
						m_pTransformCom->Chase(PlayerPos, TimeDelta, 0.f);



					}
					else if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_RUN].ActionIndex)
					{
						if (pBoss_Sandy_Body->Get_IsSpringAttack())
						{//스프링패턴
						 //IsSpringAttack = true;
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
					else if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_ATTACK_SPRING1].ActionIndex)
					{//만약 스프링 공격이라면 앞으로전진

						CTransform* m_pRope_TopTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Rope_Top"), TEXT("Com_Transform"), 4);
						_vector m_vRope_ToPos = m_pRope_TopTransform->Get_State(CTransform::STATE_POSITION);
						m_pTransformCom->Chase(m_vRope_ToPos, TimeDelta, 0.f);
					}
					//점프공격 후 피격당했을 시
					//머리 띄웠다가 다시 떨어지기
					static _float fTotalTime = 0.f;
					static _bool bisUnder = false;
					static _float fY = 0.f;
					if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_HIT_SIT_ELECTIVICITY].ActionIndex)
					{
						CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_Zap_007.ogg", CSoundMgr::MONSTER_FOOT, 0.5f);

					//	CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_CrowdCheer_002.ogg", CSoundMgr::EFFECT, 0.5f);

						CEffect_Point* pEffectPoint = static_cast<CEffect_Point*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Point")));
						pEffectPoint->IsRender(true);

						IsHit = true;
						m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0, fY, 0, 1.f));
						if (fTotalTime <= 0.25f && !bisUnder)
						{
							bisUnder = false;
							fTotalTime += 0.01;
							fY += 0.3;
						}
						else
						{
							bisUnder = true;
							fTotalTime -= 0.01;
							fY -= 0.3;
							if (fTotalTime <= 0.0f)
								bisUnder = false;
						}
						m_iDeletaTimeEdit = 0.5f;
					}
					else
					{
						fTotalTime = 0.f;
						bisUnder = false;
						fY = 0.f;
						
					}

					if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_PROVOKE_HAND].ActionIndex)
					{
						m_pTransformCom->LookAt(PlayerPos);
						IsHit = false;
					}

					if (m_iReal_Time_tAnimIndex == m_AnimationInfo[SANDY_PROVOKE_WALK_THRUST_HEAD].ActionIndex)
					{
						CTransform* m_pHeadTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head2"), TEXT("Com_Transform"));
						_vector m_vheadCurrentPos = m_pHeadTransform->Get_State(CTransform::STATE_POSITION);

						if (pBoss_Sandy_Body->Get_IsScoreBoxCollision())
						{
							//IsHit = true;
							m_pTransformCom->LookAt(m_vheadCurrentPos);
						}
						
					}

				}
				m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta * m_iDeletaTimeEdit, m_iCurrentAnimIndex, false, 0);


			}
			//항상 y는 0
			_vector m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 fPos;
			XMStoreFloat3(&fPos, m_vPos);
			if (m_iReal_Time_tAnimIndex != m_AnimationInfo[SANDY_HIT_SIT_ELECTIVICITY].ActionIndex)//전기 피격당하기만 빼고 
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x, 0, fPos.z, 1));



			//********* HP관리 **************************************************************************************************//


			if (m_iHP < 9)//Max가 아니라면 
			{
				static _uint count = 0;
				if (IsHit || m_IsScorBoxHit)
				{
					//CFish* pfish = static_cast<CFish*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_FIsh")));
					CFish* pFish = static_cast<CFish*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Fish")));

					if (m_IsScorBoxHit || count == 0 && m_iHP != -1)

					{
						//CBoss_HP_Energy정보 얻기
						CBoss_HP_Energy* pBoss_HP_Energy = static_cast<CBoss_HP_Energy*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Laye_Eenergy"), m_iHP));
						//pBoss_HP_Energy->Set_Dead();
						pBoss_HP_Energy->Set_IsEnd();
						if (m_iHP != -1)// 피 깎기
							m_iHP--;

						//CFish* pFish = static_cast<CFish*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Fish")));
				//		pFish->Set_FishRender(true);

						if (m_iHP == 5)
						{
							pFish->Set_FishRender(false);
							pCamera->Set_m_bIsDirector_End(false);
							pGameInstance->Set_Player((_uint)CPlayer_Typedef::PLAYER_KIND::PLAYER_PATRICK);
							pCamera->Set_m_bIsDirectorCount();
						}
						if (m_iHP == 2)
						{
							pFish->Set_FishRender(false);
							pCamera->Set_m_bIsDirector_End(false);
							pGameInstance->Set_Player((_uint)CPlayer_Typedef::PLAYER_KIND::PLAYER_SPONGEBOB);
							pCamera->Set_m_bIsDirectorCount();
						}
						if (m_iHP == 0)
						{
							pFish->Set_FishRender(false);
						}
					}
					else if (m_iHP == -1)
					{
						pFish->Set_FishRender(false);
						pCamera->Set_m_bIsDirector_End(false);
						pCamera->Set_m_bIsDirectorCount();
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


			}
			//Collider
			m_pColliderCom[TYPE_OBB]->Update(m_pTransformCom->Get_WorldMatrix());
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

void CBoss_Sandy_Head::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == pGameInstance->isIn_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 2.f))
	{
		if(m_isRender)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	}


	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CBoss_Sandy_Head::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pVIBufferCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{

		if (i == 1)
		{

			//if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			//	return E_FAIL;
			//ID3D11BlendState * d3dBlendState;
			//D3D11_BLEND_DESC bd;
			//ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));

			//bd.AlphaToCoverageEnable = FALSE;
			//bd.IndependentBlendEnable = FALSE;
			//bd.RenderTarget[0].BlendEnable = TRUE;
			//bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			//bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			//bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			//bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			//bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			//bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			//bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			//if (FAILED(m_pDevice->CreateBlendState(&bd, &d3dBlendState)))
			//	return false;

			//m_pContext->OMSetBlendState(d3dBlendState, 0, 0xffffffff);
			//m_pVIBufferCom->Render(i, m_pShaderCom, 1);

			//Safe_Release(d3dBlendState);

		}
		else
		{
			if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			m_pVIBufferCom->Render(i, m_pShaderCom, 0);

		}


	}

	//ID3D11BlendState * d3dBlendState;
	//D3D11_BLEND_DESC bd;
	//ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	//bd.RenderTarget[0].BlendEnable = false;
	//bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	////Safe_AddRef(d3;
	//if (FAILED(m_pDevice->CreateBlendState(&bd, &d3dBlendState)))
	//	return false;

	//m_pContext->OMSetBlendState(d3dBlendState, 0, 0xffffffff);
	//Safe_Release(d3dBlendState);

#ifdef _DEBUG
	//m_pColliderCom[TYPE_AABB]->Render();
	m_pRendererCom->Add_DebugGroup(m_pColliderCom[TYPE_OBB]);

	//m_pColliderCom[TYPE_OBB]->Render();
	//	m_pColliderCom[TYPE_SPHERE]->Render();
#endif // _DEBUG



	return S_OK;
}

HRESULT CBoss_Sandy_Head::SetUp_Components()
{
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Sandy_Head"), (CComponent**)&m_pVIBufferCom)))
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

	ColliderDesc.vScale = _float3(4.5f, 4.5f, 4.5f);
	ColliderDesc.vTranslation = _float3(0.0f, 6.f, 0.f);

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

HRESULT CBoss_Sandy_Head::SetUp_SRV()
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


CBoss_Sandy_Head * CBoss_Sandy_Head::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBoss_Sandy_Head*		pInstance = new CBoss_Sandy_Head(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CForkLift");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoss_Sandy_Head::Clone(void * pArg)
{
	CBoss_Sandy_Head*		pInstance = new CBoss_Sandy_Head(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CForkLift");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_Sandy_Head::Free()
{
	__super::Free();

	for (auto& pColliderCom : m_pColliderCom)
		Safe_Release(pColliderCom);


	//	Safe_Release(m_pColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	//	Safe_Release(pBoss_Sandy_Body);
	//	Safe_Release(pBoss_Sandy_Body);

}
