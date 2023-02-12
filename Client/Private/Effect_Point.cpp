#include "stdafx.h"
#include "..\Public\Effect_Point.h"
#include "GameInstance.h"

CEffect_Point::CEffect_Point(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CEffect_Point::CEffect_Point(const CEffect_Point & Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CEffect_Point::NativeConstruct_Prototype()
{

	return S_OK;
}

HRESULT CEffect_Point::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(23.38f, 0.f, 20.f, 1));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(rand() % 5, 0.f, rand() % 5, 1.f));

	return S_OK;
}

void CEffect_Point::Tick(_double TimeDelta)
{
	m_pVIBufferCom->Update(TimeDelta);



	if (m_icount == 110)
	{
		m_icount = 0;
		Reset();
		m_IsRender = false;
	}
}

void CEffect_Point::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;
	if (m_IsRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
		m_icount++;
	}

}

void CEffect_Point::Reset()
{
	m_pVIBufferCom->Reset();
}


HRESULT CEffect_Point::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Point::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Ray"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point_Instance"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CEffect_Point::SetUp_SRV()
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

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamFloat3(), sizeof(_float3))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_SRV(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CEffect_Point * CEffect_Point::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffect_Point*		pInstance = new CEffect_Point(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Point");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Point::Clone(void * pArg)
{
	CEffect_Point*		pInstance = new CEffect_Point(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Point");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Point::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
