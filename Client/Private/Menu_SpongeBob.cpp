#include "stdafx.h"
#include "..\Public\Menu_SpongeBob.h"

#include "GameInstance.h"



CMenu_SpongeBob::CMenu_SpongeBob(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CMenu_SpongeBob::CMenu_SpongeBob(const CMenu_SpongeBob & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CMenu_SpongeBob::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CMenu_SpongeBob::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC TransformDesc(5.f, XMConvertToRadians(90.f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (static_cast<PATRICKDESC*>(pArg) != nullptr)
		m_Info = *(PATRICKDESC*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Info.pos);
	m_pTransformCom->Rotation(XMLoadFloat3(&m_Info.vAxit), m_Info.fAngle);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-1, -1.5f, 0, 1));
	m_pVIBufferCom->Set_CurrentAnimIndex(0);


	return S_OK;
}

void CMenu_SpongeBob::Tick(_double TimeDelta)
{
	m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta, 0, true, 0);
}

void CMenu_SpongeBob::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;
	if(m_isrender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CMenu_SpongeBob::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	//_uint iNumMeshContainers = m_pVIBufferCom->Get_NumMeshContainers();

//	for (_uint i = 0; i < iNumMeshContainers; ++i)
	//{
		if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 0, aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pVIBufferCom->Render(0, m_pShaderCom, 0);
//	}
	return S_OK;
}

HRESULT CMenu_SpongeBob::SetUp_Components()
{
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Menu_SpongeBob"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMenu_SpongeBob::SetUp_SRV()
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

CMenu_SpongeBob * CMenu_SpongeBob::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMenu_SpongeBob*		pInstance = new CMenu_SpongeBob(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CForkLift");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMenu_SpongeBob::Clone(void * pArg)
{
	CMenu_SpongeBob*		pInstance = new CMenu_SpongeBob(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CForkLift");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMenu_SpongeBob::Free()
{
	__super::Free();

	//	Safe_Release(m_pColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
