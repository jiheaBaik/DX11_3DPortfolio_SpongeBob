#include "..\Public\SpongeBob.h"
#include "GameInstance.h"

CSpongeBob::CSpongeBob(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CSpongeBob::CSpongeBob(const CSpongeBob & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CSpongeBob::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CSpongeBob::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (static_cast<OBJINFODESC*>(pArg) != nullptr)
		m_Info = *(OBJINFODESC*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1));

	m_pVIBufferCom->Set_CurrentAnimIndex(7);
	return S_OK;
}

void CSpongeBob::Tick(_double TimeDelta)
{
	m_pVIBufferCom->Update_Animation(TimeDelta);
	m_pVIBufferCom->Set_CurrentAnimIndex(m_ActionIndex);
}

void CSpongeBob::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CSpongeBob::Render()
{
	if (m_isRender)
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

			m_pVIBufferCom->Render(i, m_pShaderCom, 0);
		}
	}
	


	return S_OK;
}

HRESULT CSpongeBob::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_Model_SpongeBob"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/*if (m_Info.fScale.x == 0.f || m_Info.fScale.x == NULL)
		m_Info.fScale = _float3(1.0f, 1.0f, 1.0f);
	m_pTransformCom->Set_Scaled(_float3(m_Info.fScale.x, m_Info.fScale.y, m_Info.fScale.z));
	if (m_Info.fPos.x == 0.f || m_Info.fScale.x == NULL)
		m_Info.fPos = _float3(0.f, 0.f, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_Info.fPos.x, m_Info.fPos.y, m_Info.fPos.z, 1));
	if(m_Info.fRotation != 0.f || m_Info.fRotation != NULL)
		m_pTransformCom->Rotation(XMLoadFloat3(&m_Info.fAxist), m_Info.fRotation);*/
	return S_OK;
}

HRESULT CSpongeBob::SetUp_ConstantTable()
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

CSpongeBob * CSpongeBob::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSpongeBob*		pInstance = new CSpongeBob(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CSpongeBob");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSpongeBob::Clone(void * pArg)
{
	CSpongeBob*		pInstance = new CSpongeBob(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpongeBob");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpongeBob::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
