#include "stdafx.h"
#include "..\Public\PoofEffect.h"
#include "GameInstance.h"
#include<iostream>

CPoofEffect::CPoofEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CPoofEffect::CPoofEffect(const CPoofEffect & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CPoofEffect::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPoofEffect::NativeConstruct(void * pArg)
{	
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));	

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (static_cast<POOFDESC*>(pArg) != nullptr)
		mPoofDesc = *(POOFDESC*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	return S_OK;
}

void CPoofEffect::Tick(_double TimeDelta)
{


	m_fFrame += 30.0f * TimeDelta;

	if (30.0f <= m_fFrame)
		isDead = true;


	

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	
	////// 위치 정보 얻기
	CTransform* pTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	//_vector vSnadyPos = pSnadyTransform->Get_State(CTransform::STATE_POSITION);
	//_float3 fSnadyPos;

	//XMStoreFloat3(&fSnadyPos, vSnadyPos);

	_vector pos = pTransform->Get_State(CTransform::STATE_POSITION);
	_vector look = pTransform->Get_State(CTransform::STATE_LOOK) * -1;
	
	_float3 fpos;
	XMStoreFloat3(&fpos, pos);
	_float3 fLook;
	XMStoreFloat3(&fLook, look);


	_float fdistance = 0.f;
	//_vector		pos;

	//if(look == -1)
	//		pos = XMVectorSet(fSnadyPos.x + (fSandyLook.x * fdistance), fSnadyPos.y + (fSandyLook.y * fdistance) + 1, fSnadyPos.z + fSandyLook.z + (fSandyLook.z * fdistance), 1);
	//else
		_vector vpos = XMVectorSet(fpos.x , fpos.y + 1.5f, fpos.z , 1);


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vpos);

	




	CTransform* m_pCameraTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_FIELD, TEXT("Layer_Camera"), TEXT("Com_Transform"));
	_vector m_vCameradPos = m_pCameraTransform->Get_State(CTransform::STATE_POSITION);
	
	_vector		vCameraLook = m_pCameraTransform->Get_State(CTransform::STATE_LOOK);
	


	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector		vLook = XMVector3Normalize(m_vCameradPos - vPosition);
	vLook = XMVectorSetY(vLook, 0.f);
	vLook *= -1.f;
	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
	_vector		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));


	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);

	_float size = 5.f;
	m_pTransformCom->Set_Scaled(_float3(size, size, size));


	//m_vCameradPos = XMVectorSetY(m_vCameradPos, 0.f);
	//m_pTransformCom->LookAt(m_vCameradPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CPoofEffect::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	//if(m_IsRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);

}

HRESULT CPoofEffect::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPoofEffect::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Poof"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPoofEffect::SetUp_SRV()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

//	_float4x4		ViewMatrix;

	//XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());

	/*_vector m_vright = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	_vector m_vup = m_pTransformCom->Get_State(CTransform::STATE_UP);
	_vector m_vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);*/

	


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

CPoofEffect * CPoofEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPoofEffect*		pInstance = new CPoofEffect(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CPoofEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPoofEffect::Clone(void * pArg)
{
	CPoofEffect*		pInstance = new CPoofEffect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPoofEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPoofEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
