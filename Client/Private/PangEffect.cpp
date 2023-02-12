#include "stdafx.h"
#include "..\Public\PangEffect.h"
#include "GameInstance.h"
#include<iostream>

CPangEffect::CPangEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CPangEffect::CPangEffect(const CPangEffect & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CPangEffect::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPangEffect::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (static_cast<PANGDESC*>(pArg) != nullptr)
		mPangDesc = *(PANGDESC*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, pos);

	//	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CPangEffect::Tick(_double TimeDelta)
{



	m_fFrame += 22.0f * TimeDelta;

	if (22.0f <= m_fFrame)
		m_fFrame = 0.f;



	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	//// 위치 정보 얻기
	CTransform* pSnadyTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body"), TEXT("Com_Transform"));
	_vector vSnadyPos = pSnadyTransform->Get_State(CTransform::STATE_POSITION);
	_float3 fSnadyPos;

	XMStoreFloat3(&fSnadyPos, vSnadyPos);


	//(pos : look)
	_vector		vSandyLook = pSnadyTransform->Get_State(CTransform::STATE_LOOK)  *-1;
	vSandyLook = XMVector3Normalize(vSandyLook);
	_float3 fSandyLook;
	XMStoreFloat3(&fSandyLook, vSandyLook);


	_float fdistance = mPangDesc.addPosition;
	_vector		pos;

	pos = XMVectorSet(fSnadyPos.x + (fSandyLook.x * fdistance), fSnadyPos.y + (fSandyLook.y * fdistance) + mPangDesc.distance, fSnadyPos.z + fSandyLook.z + (fSandyLook.z * fdistance), 1);


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pos);




	CTransform* m_pCameraTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Com_Transform"));
	_vector m_vCameradPos = m_pCameraTransform->Get_State(CTransform::STATE_POSITION);

	_vector		vCameraLook = m_pCameraTransform->Get_State(CTransform::STATE_LOOK);




	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector		vLook = XMVector3Normalize(m_vCameradPos - vPosition);

	vLook *= -1.f;
	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
	_vector		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));


	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);



	m_pTransformCom->Set_Scaled(_float3(mPangDesc.scale, mPangDesc.scale, mPangDesc.scale));


	RELEASE_INSTANCE(CGameInstance);
}

void CPangEffect::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	if (m_IsRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

}

HRESULT CPangEffect::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	m_pShaderCom->Begin(6);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPangEffect::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Pang"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPangEffect::SetUp_SRV()
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

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamFloat3(), sizeof(_float3))))
	return E_FAIL;*/


	if (FAILED(m_pTextureCom->SetUp_SRV(m_pShaderCom, "g_DiffuseTexture", m_fFrame)))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_SRV("g_DepthTexture", pGameInstance->Get_RTSRV(TEXT("Target_Depth")))))
	return E_FAIL;*/


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CPangEffect * CPangEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPangEffect*		pInstance = new CPangEffect(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CPangEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPangEffect::Clone(void * pArg)
{
	CPangEffect*		pInstance = new CPangEffect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPangEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPangEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
