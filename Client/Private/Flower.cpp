#include "stdafx.h"
#include "..\Public\Flower.h"



#include "GameInstance.h"
#include "Taxi.h"
#include "Gate.h"


CFlower::CFlower(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CFlower::CFlower(const CFlower & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CFlower::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CFlower::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (static_cast<FLOWERDESC*>(pArg) != nullptr)
		m_flowrDesc = *(FLOWERDESC*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_flowrDesc.pos + XMVectorSet(0, 1, 0, 1));
	return S_OK;
}

void CFlower::Tick(_double TimeDelta)
{
	
	_float m_fangle = 0.05f;
	m_pTransformCom->Rotation(XMLoadFloat3(&_float3(0.f, 1.f, 0.f)), m_fangle);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCollider*	pPlayerCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Collider_OBB"));
	if (nullptr == pPlayerCollider)
		return;

	if (!m_isMove)
	{
		if (m_pColliderCom[TYPE_SPHERE]->Collision(pPlayerCollider))
		{
			CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
			_vector PlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
			_bool isEnd = m_pTransformCom->Chase(PlayerPos, TimeDelta* 0.9f, 1.f);

			if (isEnd)
			{
				CTaxi* pCTaxit = static_cast<CTaxi*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Taxi")));
				pCTaxit->Set_FlowerCount();

					CGate* pGate = static_cast<CGate*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Gate"),2));
					pGate->Set_FlowerCount();
					//pGate->Set_Open();

					pGate  = static_cast<CGate*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Gate"), 3));
					pGate->Set_FlowerCount();
				//	pGate->Set_Open();

					CGameInstance::Get_Instance()->StopSound(CSoundMgr::EFFECT);
					CGameInstance::Get_Instance()->PlayMySound(L"SFX_ShinyThing_add_ding_001.ogg", CSoundMgr::EFFECT, 0.5f);
					
				isDead = true;

			}

		}
	}
	

		_float3 fPos;
		XMStoreFloat3(&fPos, m_flowrDesc.pos + XMVectorSet(0, 1, 0, 1));
	
		
		if (m_isMove)
		{
			if (vy < fPos.y + 2 && !m_isUnder)
			{
				vy = (vy + g);
				g += addY;
			}
			else
			{
				m_isUnder = true;
				if (vy < fPos.y - 1.f)
				{
					m_isMove = false;
					vy = fPos.y;
				}
				else
				{
					vy = (vy - g);
					g += addY2;
				}
			}
		}
	
		
			
		if (m_isMove)
		{
			_float3 fLook;
			XMStoreFloat3(&fLook, m_flowrDesc.vLook);

			_vector m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			m_vPos += m_flowrDesc.vLook * 1.2f * _float(TimeDelta);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);

			_float3 fPos;
			XMStoreFloat3(&fPos, m_vPos);

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x + 0, vy, fPos.z + 0/*vx*/, 1));
		}

		m_pColliderCom[TYPE_SPHERE]->Update(m_pTransformCom->Get_WorldMatrix());

		RELEASE_INSTANCE(CGameInstance);


}

void CFlower::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CFlower::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pVIBufferCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
	

		m_pVIBufferCom->Render(i, m_pShaderCom, 8);
	}


#ifdef _DEBUG

	m_pRendererCom->Add_DebugGroup(m_pColliderCom[TYPE_SPHERE]);

#endif // _DEBUG

	return S_OK;
}

HRESULT CFlower::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_FIELD, TEXT("Prototype_Component_Model_Flower"), (CComponent**)&m_pVIBufferCom)))
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
	ColliderDesc.vTranslation = _float3(0.0f, 0.5f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pColliderCom[TYPE_OBB], &ColliderDesc)))
		return E_FAIL;



	/* For.Com_Collider_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(5, 5, 5);
	ColliderDesc.vTranslation = _float3(0.0f, 0.f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pColliderCom[TYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CFlower::SetUp_ConstantTable()
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
	if (FAILED(m_pShaderCom->Set_RawValue("g_Color", &m_flowrDesc.vColor, sizeof(_vector))))
		return E_FAIL;
	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

CFlower * CFlower::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CFlower*		pInstance = new CFlower(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CFlower");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CFlower::Clone(void * pArg)
{
	CFlower*		pInstance = new CFlower(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFlower");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFlower::Free()
{
	__super::Free();
	for (auto& pColliderCom : m_pColliderCom)
		Safe_Release(pColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
