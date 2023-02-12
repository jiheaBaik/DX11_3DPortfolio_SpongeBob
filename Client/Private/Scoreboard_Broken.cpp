#include "stdafx.h"
#include "..\Public\Scoreboard_Broken.h"



#include "GameInstance.h"

CScoreboard_Broken::CScoreboard_Broken(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CScoreboard_Broken::CScoreboard_Broken(const CScoreboard_Broken & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CScoreboard_Broken::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CScoreboard_Broken::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (static_cast<OBJINFODESC*>(pArg) != nullptr)
		m_Info = *(OBJINFODESC*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void CScoreboard_Broken::Tick(_double TimeDelta)
{
	
	m_pColliderCom[TYPE_OBB]->Update(m_pTransformCom->Get_WorldMatrix());

}

void CScoreboard_Broken::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;
	if(m_IsBroken)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

}

HRESULT CScoreboard_Broken::Render()
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
}

void CScoreboard_Broken::Move(_double TimeDelta)
{
	_float m_fangle = 0.005;
	m_pTransformCom->Rotation(XMLoadFloat3(&_float3(0.f, 0.f, 1.f)), m_fangle);
}


HRESULT CScoreboard_Broken::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Scoreboard_Broken"), (CComponent**)&m_pVIBufferCom)))
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

	ColliderDesc.vScale = _float3(13,13,13);
	ColliderDesc.vTranslation = _float3(0.0f, 1.f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pColliderCom[TYPE_OBB], &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Collider_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(5,5,5);
	ColliderDesc.vTranslation = _float3(0.0f, 1.0f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pColliderCom[TYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;



	if (m_Info.fScale.x == 0.f || m_Info.fScale.x == NULL)
		m_Info.fScale = _float3(1.0f, 1.0f, 1.0f);
	m_pTransformCom->Set_Scaled(_float3(m_Info.fScale.x, m_Info.fScale.y, m_Info.fScale.z));
	if (m_Info.fPos.x == 0.f || m_Info.fScale.x == NULL)
		m_Info.fPos = _float3(0.f, 0.f, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_Info.fPos.x, m_Info.fPos.y, m_Info.fPos.z, 1));
	if (m_Info.fRotation != 0.f || m_Info.fRotation != NULL)
		m_pTransformCom->Rotation(XMLoadFloat3(&m_Info.fAxist), m_Info.fRotation);



	

	return S_OK;
}

HRESULT CScoreboard_Broken::SetUp_ConstantTable()
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

CScoreboard_Broken * CScoreboard_Broken::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CScoreboard_Broken*		pInstance = new CScoreboard_Broken(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CScoreboard_Broken");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CScoreboard_Broken::Clone(void * pArg)
{
	CScoreboard_Broken*		pInstance = new CScoreboard_Broken(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CScoreboard_Broken");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CScoreboard_Broken::Free()
{
	__super::Free();
	for (auto& pColliderCom : m_pColliderCom)
		Safe_Release(pColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}