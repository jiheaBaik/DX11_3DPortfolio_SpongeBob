#include "..\Public\Boss_Sandy_Body.h"
#include "GameInstance.h"



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
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (static_cast<OBJINFODESC*>(pArg) != nullptr)
		m_Info = *(OBJINFODESC*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, -2.f, 5.f, 1));

	m_pVIBufferCom->Set_CurrentAnimIndex(50);
	return S_OK;
}

void CBoss_Sandy_Body::Tick(_double TimeDelta)
{
	m_pVIBufferCom->Update_Animation(TimeDelta);
	m_pVIBufferCom->Set_CurrentAnimIndex(m_ActionIndex);
}

void CBoss_Sandy_Body::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CBoss_Sandy_Body::Render()
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
			//if (i == 0)
			//{
				if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
					return E_FAIL;

				m_pVIBufferCom->Render(i, m_pShaderCom, 0);

			//}
		}
	}



	return S_OK;


}

HRESULT CBoss_Sandy_Body::SetUp_Components()
{
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Sandy_Body"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Sandy_Body::SetUp_ConstantTable()
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

	//	Safe_Release(m_pColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
