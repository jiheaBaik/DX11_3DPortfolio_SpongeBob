#include "stdafx.h"
#include "..\Public\EndPangEffect_Point.h"
#include "GameInstance.h"

CEndPangEffect_Point::CEndPangEffect_Point(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CEndPangEffect_Point::CEndPangEffect_Point(const CEndPangEffect_Point & Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CEndPangEffect_Point::NativeConstruct_Prototype()
{

	return S_OK;
}

HRESULT CEndPangEffect_Point::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(rand() % 20, -7, rand() % 20, 1));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-120, 0.f, -120, 1.f));
	m_pVIBufferCom->FiledInitial();
	return S_OK;
}

void CEndPangEffect_Point::Tick(_double TimeDelta)
{
	m_pVIBufferCom->Update2(TimeDelta * 0.1f);


	//m_iTextureNum = rand() % 7;



	//if (m_icount == 110)
	//{
	//	m_icount = 0;
	//	//	Reset();
	//	//	m_IsRender = false;
	//}
}

void CEndPangEffect_Point::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;
	//if (m_IsRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
		m_icount++;
	}

}

void CEndPangEffect_Point::Reset()
{
	m_pVIBufferCom->Reset();
}


HRESULT CEndPangEffect_Point::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEndPangEffect_Point::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_EndPang"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point_Instance"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CEndPangEffect_Point::SetUp_SRV()
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

CEndPangEffect_Point * CEndPangEffect_Point::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEndPangEffect_Point*		pInstance = new CEndPangEffect_Point(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CEndPangEffect_Point");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEndPangEffect_Point::Clone(void * pArg)
{
	CEndPangEffect_Point*		pInstance = new CEndPangEffect_Point(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEndPangEffect_Point");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEndPangEffect_Point::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
