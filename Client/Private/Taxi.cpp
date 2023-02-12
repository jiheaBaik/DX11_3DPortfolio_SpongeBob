#include "stdafx.h"
#include "..\Public\Taxi.h"



#include "GameInstance.h"
#include "Player.h"
#include "Gate.h"
#include "Menu_SpongeBob.h"


CTaxi::CTaxi(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CTaxi::CTaxi(const CTaxi & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CTaxi::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CTaxi::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (static_cast<_vector*>(pArg) != nullptr)
		m_vpos = *(_vector*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vpos);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void CTaxi::Tick(_double TimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	if (m_bisCol)
	{
		count++;
	}
	//(pos : look)
	_vector		vLook = m_pTransformCom->Get_State(CTransform::STATE_RIGHT) * -1;
	vLook = XMVector3Normalize(vLook);
	_float3 fLook;
	XMStoreFloat3(&fLook, vLook);

	_vector		vpos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 fpos;
	XMStoreFloat3(&fpos, vpos);
	static _bool test = false;
	_vector		pos;
	if (GetAsyncKeyState('O') & 0x0001)
	{
		test = true;

	}
	

	CGate* pCGate = static_cast<CGate*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Gate"), 2));
	if (!test)
	{
		if (pCGate->Get_End())
		{
			test = true;
			CGameInstance::Get_Instance()->StopSound(CSoundMgr::EFFECT);
			CGameInstance::Get_Instance()->PlayMySound(L"sfx_call_taxi.ogg", CSoundMgr::EFFECT, 0.2f);
		}

	}

	if (test)
	{
		fdistance += 0.005f;
		if (fdistance > 0.45 /*&&fdistance < 0.105*/)
			m_iStopCount++;

			if (m_iStopCount > 1 && m_iStopCount < 50)
			{
				fdistance += 0.001f;
			}
			else
			{
				pos = XMVectorSet(fpos.x + (fLook.x * fdistance), fpos.y + (fLook.y * fdistance), fpos.z + fLook.z + (fLook.z * fdistance), 1);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, pos);

			}
			CCollider*	pPlayerCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Collider_OBB"));
			if (nullptr == pPlayerCollider)
				return;

			if (m_pColliderCom[TYPE_OBB]->Collision(pPlayerCollider))
			{
				CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_Player")));
				m_bisCol = true;

				pPlayer->Set_Isrender(false);
				CMenu_SpongeBob* pPatrick = static_cast<CMenu_SpongeBob*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Menu_SpongeBob")));
				pPatrick->Set_Isrender(false);
			}
	}
	
	if (GetAsyncKeyState('I') & 0x0001)
	{
		test = false;
		fdistance = 0.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vpos);

	}

	
	Safe_Release(pGameInstance);

	m_pColliderCom[TYPE_OBB]->Update(m_pTransformCom->Get_WorldMatrix());


	
}

void CTaxi::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == pGameInstance->isIn_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 2.f))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CTaxi::Render()
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
		//if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		//	return E_FAIL;
		//if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		//	return E_FAIL;

		//	m_pShaderCom->Begin(0);

		m_pVIBufferCom->Render(i, m_pShaderCom, 0);
	}


#ifdef _DEBUG
	//m_pColliderCom[TYPE_AABB]->Render();
	//m_pColliderCom[TYPE_OBB]->Render();
	m_pRendererCom->Add_DebugGroup(m_pColliderCom[TYPE_OBB]);
#endif // _DEBUG

	return S_OK;
	/*CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	wsprintf(m_szFlowerCount, TEXT("count : %d"), m_iFlowerCount);
	pGameInstance->Render_Font(TEXT("Font_Bazzi"), m_szFlowerCount, _float2(0, 50), XMVectorSet(1.f, 1.f, 1.f, 1.f));
	if (m_iFlowerCount > 10)
	{
		int a = 0;
	}
	RELEASE_INSTANCE(CGameInstance);*/

	return S_OK;
}

HRESULT CTaxi::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_FIELD, TEXT("Prototype_Component_Model_Taxi"), (CComponent**)&m_pVIBufferCom)))
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

	ColliderDesc.vScale = _float3(10.f, 10.f, 15.f);
	ColliderDesc.vTranslation = _float3(0.0f, 1.f, 0.0f);

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

HRESULT CTaxi::SetUp_ConstantTable()
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

CTaxi * CTaxi::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTaxi*		pInstance = new CTaxi(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CTaxi");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTaxi::Clone(void * pArg)
{
	CTaxi*		pInstance = new CTaxi(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTaxi");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTaxi::Free()
{
	__super::Free();
	for (auto& pColliderCom : m_pColliderCom)
		Safe_Release(pColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
