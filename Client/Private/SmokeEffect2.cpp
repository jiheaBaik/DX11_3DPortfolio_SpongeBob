#include "stdafx.h"
#include "..\Public\SmokeEffect2.h"
#include "GameInstance.h"
#include<iostream>

CSmokeEffect2::CSmokeEffect2(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CSmokeEffect2::CSmokeEffect2(const CSmokeEffect2 & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CSmokeEffect2::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CSmokeEffect2::NativeConstruct(void * pArg)
{	
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));	

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (static_cast<_vector*>(pArg) != nullptr)
		pos = *(_vector*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pos);


	//m_pTransformCom->Set_Scaled(_float3(70, 70, 50));

//	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CSmokeEffect2::Tick(_double TimeDelta)
{


	//if (!m_IsTurn)
	//{
	//	

	//	if (65.0f <= m_fFrame)
	//	{
	//		m_IsTurn = true;
	//		return;
	//	}
	//		//m_fFrame = 0.f;

	//	m_fFrame += 65.0f * TimeDelta;
	//}
	//else
	//{
	//	if (0.f > m_fFrame)
	//	{
	//		m_IsTurn = false;
	//		return;
	//	}

	//	
	//	m_fFrame -= 65.0f * TimeDelta;
	//}

	//cout << "frame : " << endl;
	m_fFrame += 32.0f * TimeDelta;

	if (32.0f <= m_fFrame)
		m_fFrame = 0.f;

	

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	////// 위치 정보 얻기
	//CTransform* pScoreBoardTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Mesh_scoreboard_broken"), TEXT("Com_Transform"));
	//_vector vScoreBoadyPos = pScoreBoardTransform->Get_State(CTransform::STATE_POSITION);
	//_float3 fScoreBoardBbodyPos;
	//XMStoreFloat3(&fScoreBoardBbodyPos, vScoreBoadyPos);


	
	
	
	CTransform* m_pCameraTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Com_Transform"));
	_vector m_vCameradPos = m_pCameraTransform->Get_State(CTransform::STATE_POSITION);
	
	_vector		vCameraLook = m_pCameraTransform->Get_State(CTransform::STATE_LOOK);
	

	//_vector		vBrokenyLook = pScoreBoardTransform->Get_State(CTransform::STATE_LOOK) * -1;
	//vBrokenyLook = XMVector3Normalize(vBrokenyLook);
	//_float3 fBrokenLook;
	//XMStoreFloat3(&fBrokenLook, vBrokenyLook);

//	_float fdistance = 5.f;
	//_vector		vpos = XMVectorSet(fScoreBoardBbodyPos.x + (fBrokenLook.x * fdistance), fScoreBoardBbodyPos.y + (fBrokenLook.y * fdistance) + 5, fScoreBoardBbodyPos.z + (fBrokenLook.z * fdistance), 1);


	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vpos);



	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector		vLook = XMVector3Normalize(m_vCameradPos - vPosition);
	//vLook = XMVectorSetY(vLook, 0.f);
	vLook *= -1.f;
	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
	_vector		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));


	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);




	m_pTransformCom->Set_Scaled(_float3(20, 20, 20));


	//m_vCameradPos = XMVectorSetY(m_vCameradPos, 0.f);
	//m_pTransformCom->LookAt(m_vCameradPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CSmokeEffect2::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	if (m_IsRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CSmokeEffect2::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	m_pShaderCom->Begin(6);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CSmokeEffect2::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeEffect2"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSmokeEffect2::SetUp_SRV()
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

CSmokeEffect2 * CSmokeEffect2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSmokeEffect2*		pInstance = new CSmokeEffect2(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSmokeEffect2::Clone(void * pArg)
{
	CSmokeEffect2*		pInstance = new CSmokeEffect2(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSmokeEffect2::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
