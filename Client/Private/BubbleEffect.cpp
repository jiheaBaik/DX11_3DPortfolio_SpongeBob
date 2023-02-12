#include "stdafx.h"
#include "..\Public\BubbleEffect.h"
#include "GameInstance.h"
#include<iostream>

CBubbleEffect::CBubbleEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CBubbleEffect::CBubbleEffect(const CBubbleEffect & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CBubbleEffect::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBubbleEffect::NativeConstruct(void * pArg)
{	
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));	

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (static_cast<_uint*>(pArg) != nullptr)
		m_Info = *(_uint*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pos);

//	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CBubbleEffect::Tick(_double TimeDelta)
{


	
	
	

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 fPlayerPos;
	XMStoreFloat3(&fPlayerPos, vPlayerPos);


	m_fAngle -= 0.35f;
	_float3 fDistance;
	_float fDistanceArounf = 2.f;
	//_float YDistance = (rand() % 10 + 3) * 0.1f;
	_float YDistance = 1.f;

	fDistance.x = _float(fPlayerPos.x + fDistanceArounf * cosf(m_fAngle));
	fDistance.z = _float(fPlayerPos.z - fDistanceArounf* sinf(m_fAngle));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fDistance.x, fPlayerPos.y + YDistance, fDistance.z, 1));
	m_pTransformCom->LookAt(vPlayerPos);




	
	CTransform* m_pCameraTransform = nullptr;
	if(0 == m_Info)
		m_pCameraTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_FIELD, TEXT("Layer_Camera"), TEXT("Com_Transform"));
	else
		m_pCameraTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Com_Transform"));

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


	_float fscale = (rand() % 6 + 1) * 0.1f;

	m_pTransformCom->Set_Scaled(_float3(fscale, fscale, fscale));



	RELEASE_INSTANCE(CGameInstance);
}

void CBubbleEffect::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	//if(m_IsRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);

		if (m_fAngle < -12.f)
			isDead = true;
}

HRESULT CBubbleEffect::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CBubbleEffect::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bubble"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBubbleEffect::SetUp_SRV()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

//


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_Transformfloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_Transformfloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamFloat3(), sizeof(_float3))))
		return E_FAIL;*/
	

	if (FAILED(m_pTextureCom->SetUp_SRV(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_SRV("g_DepthTexture", pGameInstance->Get_RTSRV(TEXT("Target_Depth")))))
		return E_FAIL;*/


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CBubbleEffect * CBubbleEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBubbleEffect*		pInstance = new CBubbleEffect(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBubbleEffect::Clone(void * pArg)
{
	CBubbleEffect*		pInstance = new CBubbleEffect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBubbleEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
