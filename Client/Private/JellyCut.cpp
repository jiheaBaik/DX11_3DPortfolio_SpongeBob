#include "stdafx.h"
#include "..\Public\JellyCut.h"

#include <iostream>
#include "GameInstance.h"

CJellyCut::CJellyCut(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CJellyCut::CJellyCut(const CJellyCut & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CJellyCut::NativeConstruct_Prototype()
{
	

	return S_OK;
}

HRESULT CJellyCut::NativeConstruct(void * pArg)
{

	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fX = g_iWinSizeX * 0.5f/*+ 500*/;
	m_fY = g_iWinSizeY * 0.5f ;
	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY ;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	

	return S_OK;

}

void CJellyCut::Tick(_double TimeDelta)
{
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.0f, 1.f));
}

void CJellyCut::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	if (m_IsAlphaSet)
	{
		m_fAlPha -= 0.01;
	}
	
	if(m_IsRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	
	
}

HRESULT CJellyCut::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	if(m_IsAlphaSet)
		m_pShaderCom->Begin(13);
	else
		m_pShaderCom->Begin(6);


	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CJellyCut::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_TakeJellyCut"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CJellyCut::SetUp_SRV()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_float4x4		ViewMatrix;

	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureNum)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_Dvalue", &m_fAlPha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CJellyCut * CJellyCut::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CJellyCut*		pInstance = new CJellyCut(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CCut");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CJellyCut::Clone(void * pArg)
{
	CJellyCut*		pInstance = new CJellyCut(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCut");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CJellyCut::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
