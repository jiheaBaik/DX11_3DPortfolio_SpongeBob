#include "stdafx.h"
#include "..\Public\Boss_Sandy_Head2.h"
#include "GameInstance.h"

//#include <iostream>
//#include "Boss_Sandy_Body.h"
//#include "Player.h"
//#include "Boss_HPBar.h"
//#include "Boss_HP_Energy.h"
//#include "Camera_Free.h"
#include "Boss_Sandy_Head.h"
#include "Boss_Sandy_Body.h"


#include <iostream>
CBoss_Sandy_Head2::CBoss_Sandy_Head2(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
	//	pBoss_Sandy_Body = CBoss_Sandy_Body::Create(m_pDevice, m_pContext);

}

CBoss_Sandy_Head2::CBoss_Sandy_Head2(const CBoss_Sandy_Head2 & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CBoss_Sandy_Head2::NativeConstruct_Prototype()
{


	return S_OK;
}

HRESULT CBoss_Sandy_Head2::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC TransformDesc(5.f, XMConvertToRadians(90.f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	return S_OK;
}

void CBoss_Sandy_Head2::Tick(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CBoss_Sandy_Head* pBoss_Sandy_Head = static_cast<CBoss_Sandy_Head*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head")));

	m_pVIBufferCom->Set_CurrentAnimIndex(0);
	m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta, 0, false, 0);
	if (pBoss_Sandy_Head->Get_IsRender())
	{
		CTransform* m_pHeadTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head"), TEXT("Com_Transform"));
		_vector m_vHeadPos = m_pHeadTransform->Get_State(CTransform::STATE_POSITION);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vHeadPos);

		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos + XMVectorSet(0, vy, vx, 1));
		_float3 fPos;
		XMStoreFloat3(&fPos, m_vHeadPos);
		//cout << "Original fPosx : " << fPos.x << endl;

		vy =0;
		m_isUnder = false;
		g = 0.1;
		vx = 0.1;

		m_isRender = false;
		m_isMoveEnd = false;


		

	}
	else
	{
		if (GetAsyncKeyState('D') & 0x0001)
		{
			CTransform* m_playerTranssform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer"), TEXT("Com_Transform"));
			_vector		sss = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


			m_playerTranssform->Chase(sss, TimeDelta, 0.f);

		}

		

		m_isRender = true;

		if (!m_isMoveEnd)
		{
			if (vy < 1.4 && !m_isUnder)
			{
				//m_isUnder = true;
				vy = (vy + g);
				vx += addX;

				_vector m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				_float3 fPos;
				XMStoreFloat3(&fPos, m_vPos);
				//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos + XMVectorSet(0, vy, vx, 1));
				
				
				
				
				CTransform* m_playerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer"), TEXT("Com_Transform"));
				_vector		vLook = m_playerTransform->Get_State(CTransform::STATE_LOOK);
				vLook = XMVector3Normalize(vLook);
				 m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

				m_vPos += vLook * 7 *_float(TimeDelta);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);

				m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				 fPos;
				XMStoreFloat3(&fPos, m_vPos);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x + 0, vy, fPos.z + 0/*vx*/, 1));
				
				
				//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x + 0, vy, fPos.z + vx, 1));

			}
			else
			{
				vy = (vy - g);
				g += addY;
				m_isUnder = true;
				if (vy < -5.2)
				{
					vy = -5.2;
					vx = 0;
					m_isMoveEnd = true;
				}
				//-5.2
				

				CTransform* m_playerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer"), TEXT("Com_Transform"));
				_vector		vLook = m_playerTransform->Get_State(CTransform::STATE_LOOK);
				vLook = XMVector3Normalize(vLook);
				_vector m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

				m_vPos += vLook *7 *_float(TimeDelta);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);

				 m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				_float3 fPos;
				XMStoreFloat3(&fPos, m_vPos);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x + 0, vy, fPos.z + 0/*vx*/, 1));
				//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos + XMVectorSet(0, vy, vx, 1));

			}
		}
		
		_vector m_vPos5= m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		
		_float3 fPssos;
		XMStoreFloat3(&fPssos, m_vPos5);

	/*	cout << "___________________________________________" << endl;
		cout << "m_vPos5 fPosx : " << fPssos.x << endl;
		cout << "m_vPos5 fPosY : " << fPssos.y << endl;
		cout << "m_vPos5 fPosZ : " << fPssos.z << endl;
		cout << "___________________________________________" << endl;*/


		CCollider*	pBoxollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Mesh_scoreboard_broken"), TEXT("Com_Collider_OBB"));
		if (nullptr == pBoxollider)
			return;
		_vector m_vPos;
		_float3 fPos;

		


		if (m_pColliderCom[TYPE_OBB]->Collision(pBoxollider))
		{
		/*	 m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			 XMStoreFloat3(&fPos, m_vPos);*/
			 m_isMoveEnd = true;
			 CBoss_Sandy_Body* pBoss_Sandy_Body = static_cast<CBoss_Sandy_Body*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body")));
			 pBoss_Sandy_Body->Set_IsSlamAttackCollision();
			 m_isCillision = true;
			// pBubblePangEffect->IsRender(true);
			
		}
		
			//pBubblePangEffect->IsRender(true);

	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x + 0, vy, fPos.z + 0/*vx*/, 1));


		
	}

	if (m_isCillision)
	{
		count++;
		CBoss_Sandy_Body* pBoss_Sandy_Body = static_cast<CBoss_Sandy_Body*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body")));
		pBoss_Sandy_Body->Set_First_Anim(0);
		_vector m_vheadCurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 fPos;
		XMStoreFloat3(&fPos, m_vheadCurrentPos);
		pBoss_Sandy_Body->Set_IsScoreBoxCollision(true);
	//	if(fPos.y > 0)
		//	m_pTransformCom->Set_State(CTransform::STATE_POSITION,/* m_vheadCurrentPos + */XMVectorSet(fPos.x + 0, fPos.y - 0.25, fPos.z + 0/*vx*/, 1));
	//	else if(fPos.y < 0)
			//m_pTransformCom->Set_State(CTransform::STATE_POSITION,/* m_vheadCurrentPos + */XMVectorSet(fPos.x + 0, fPos.y + 0.25, fPos.z + 0/*vx*/, 1));
		/*else*/
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,/* m_vheadCurrentPos + */XMVectorSet(fPos.x,0, fPos.z , 1));

		if (count >= 100)
		{
			//m_isCillision = 0;
			CTransform* m_pSandyBosyTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body"), TEXT("Com_Transform"));
			
			_vector m_vTargetPos = m_pSandyBosyTransform->Get_State(CTransform::STATE_POSITION);
		
		//	m_pTransformCom->LookAt(m_vTargetPos);
			_vector vLook = m_pSandyBosyTransform->Get_State(CTransform::STATE_LOOK)* -1;

			_vector m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			CCollider*	pCollider2 = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body"), TEXT("Com_ColliderSpring2_OBB"));
			if (nullptr == pCollider2)
				return;

			if (m_pColliderCom[TYPE_OBB]->Collision_OBB(pCollider2))
				m_isCillision2 = true;

			pBoss_Sandy_Body->Set_Last_Anim(-1);
			pBoss_Sandy_Head->Set_IsScorBoxHit(false);
			if (!IsFirstHit)
			{
				pBoss_Sandy_Head->Set_IsScorBoxHit(true);
				IsFirstHit = true;
			}


			if (!m_isCillision2 /*m_pTransformCom->Compute_Distance(m_vTargetPos) >= 1*/)
			{
				m_vPos += XMVector3Normalize(vLook) * 5 * _float(TimeDelta);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);


				CTransform* m_pSandyBosyTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body"), TEXT("Com_Transform"));

				 m_pSandyBosyTransform->LookAt(m_vPos);

			}
			else
			{
				count = 0;
				CBoss_Sandy_Body* pBoss_Sandy_Body = static_cast<CBoss_Sandy_Body*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body")));
				pBoss_Sandy_Body->Set_Last_Anim(0);
				m_isCillision = false;
				m_isCillision2 = false;

				
				m_vPos += XMVector3Normalize(vLook) * 5 * _float(TimeDelta);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);

				_vector vLookTarget = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				m_pSandyBosyTransform->LookAt(vLookTarget);

				IsFirstHit = false;
			}
				
			












			//_bool IsEnd = m_pTransformCom->Chase(vLook, TimeDelta* 0.7 , 3.f);

		/*	if (IsEnd)
			{
				pBoss_Sandy_Head->Set_IsRender(true);
				m_isRender = false;
				m_isCillision = false;
				count = 0;
				m_isMoveEnd = false;
				vy = 0;
				m_isUnder = false;
				g = 0.1;
				vx = 0.1;

			}*/
		}
	}
	
	
	//Collider
	m_pColliderCom[TYPE_OBB]->Update(m_pTransformCom->Get_WorldMatrix());
		RELEASE_INSTANCE(CGameInstance);
		
}

void CBoss_Sandy_Head2::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//if (true == pGameInstance->isIn_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 2.f))
//{
		CBoss_Sandy_Head* pBoss_Sandy_Head = static_cast<CBoss_Sandy_Head*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head")));

		if (!pBoss_Sandy_Head->Get_IsRender())
		{

			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		}
	//}


	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CBoss_Sandy_Head2::Render()
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
			m_pVIBufferCom->Render(i, m_pShaderCom,m_iShaderNumber);

		}


	}


	if (m_iShaderNumber == 2)
	{
		int a = 0;
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

HRESULT CBoss_Sandy_Head2::SetUp_Components()
{
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Sandy_Head2"), (CComponent**)&m_pVIBufferCom)))
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

	ColliderDesc.vScale = _float3(4.5f, 5.f, 4.5f);
	ColliderDesc.vTranslation = _float3(0.0f, 6.f, 0.f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pColliderCom[TYPE_OBB], &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Collider_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(2.0f, 2.f, 2.f);
	ColliderDesc.vTranslation = _float3(0.0f, 5.2f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pColliderCom[TYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Sandy_Head2::SetUp_SRV()
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

void CBoss_Sandy_Head2::Set_Move()
{
	m_isMoveEnd = false;
	vy = 0;
	m_isUnder = false;
	g = 0.1;
	vx = 0.1;
}


CBoss_Sandy_Head2 * CBoss_Sandy_Head2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBoss_Sandy_Head2*		pInstance = new CBoss_Sandy_Head2(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : Boss_Sandy_Head2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoss_Sandy_Head2::Clone(void * pArg)
{
	CBoss_Sandy_Head2*		pInstance = new CBoss_Sandy_Head2(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : Boss_Sandy_Head2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_Sandy_Head2::Free()
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
