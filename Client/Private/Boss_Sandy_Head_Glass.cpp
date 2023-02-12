#include "stdafx.h"
#include "..\Public\Boss_Sandy_Head_Glass.h"
#include "GameInstance.h"

#include <iostream>
#include "Boss_Sandy_Head.h"




CBoss_Sandy_Head_Glass::CBoss_Sandy_Head_Glass(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
	//	pBoss_Sandy_Body = CBoss_Sandy_Body::Create(m_pDevice, m_pContext);

}

CBoss_Sandy_Head_Glass::CBoss_Sandy_Head_Glass(const CBoss_Sandy_Head_Glass & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CBoss_Sandy_Head_Glass::NativeConstruct_Prototype()
{


	return S_OK;
}

HRESULT CBoss_Sandy_Head_Glass::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&mGlassDesc, pArg, sizeof(GLASSDESC));

	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_SocketMatrixPtr()))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_pTransformCom->Rotation(XMLoadFloat3(&_float3(1.f, 0.f, 0.f)), -89.6f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0, 0.f));

	return S_OK;
}

void CBoss_Sandy_Head_Glass::Tick(_double TimeDelta)
{
	
}

void CBoss_Sandy_Head_Glass::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*			pTargetTransform = (CTransform*)pGameInstance->Get_Component(mGlassDesc.eLevel, mGlassDesc.pLayerTag, CGameObject::m_pTransformTag, mGlassDesc.iIndex);
	XMStoreFloat4x4(&m_SocketMatrix, XMLoadFloat4x4(&m_BoneOffsetMatrix) * XMLoadFloat4x4(m_pBoneMatrix) * XMLoadFloat4x4(&m_PivotMatrix) * pTargetTransform->Get_WorldMatrix());


	//if (true == pGameInstance->isIn_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 2.f))
//	{

	CBoss_Sandy_Head* pBoss_Sandy_Head = static_cast<CBoss_Sandy_Head*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head")));


	if(pBoss_Sandy_Head->Get_IsRender())
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	//}


	
//	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CBoss_Sandy_Head_Glass::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pVIBufferCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{

		if (i == 1)
		{
			if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			m_pVIBufferCom->Render(i, m_pShaderCom, 2);
		}
			


	}




	return S_OK;
}

HRESULT CBoss_Sandy_Head_Glass::SetUp_Components()
{
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Boss_Sandy_Head_Glass"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBoss_Sandy_Head_Glass::SetUp_SRV()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	_float4x4	SocketMatrixTP;

	XMStoreFloat4x4(&SocketMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_SocketMatrix)));

	if (FAILED(m_pShaderCom->Set_RawValue("g_SocketMatrix", &SocketMatrixTP, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_Transformfloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_Transformfloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CBoss_Sandy_Head_Glass::SetUp_SocketMatrixPtr()
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	CModel*			pTargetModel = (CModel*)pGameInstance->Get_Component(mGlassDesc.eLevel, mGlassDesc.pLayerTag, mGlassDesc.pModelComTag, mGlassDesc.iIndex);
	if (nullptr == pTargetModel)
		return E_FAIL;

	m_pBoneMatrix = pTargetModel->Get_BoneMatrixPtr(mGlassDesc.pBoneName);
	if (nullptr == m_pBoneMatrix)
		return E_FAIL;

	m_BoneOffsetMatrix = pTargetModel->Get_BoneOffsetMatrix(mGlassDesc.pBoneName);
	m_PivotMatrix = pTargetModel->Get_PivotMatrix();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
CBoss_Sandy_Head_Glass * CBoss_Sandy_Head_Glass::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBoss_Sandy_Head_Glass*		pInstance = new CBoss_Sandy_Head_Glass(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CForkLift");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoss_Sandy_Head_Glass::Clone(void * pArg)
{
	CBoss_Sandy_Head_Glass*		pInstance = new CBoss_Sandy_Head_Glass(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CForkLift");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_Sandy_Head_Glass::Free()
{
	__super::Free();

	//for (auto& pColliderCom : m_pColliderCom)
	//	Safe_Release(pColliderCom);


	//	Safe_Release(m_pColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	//	Safe_Release(pBoss_Sandy_Body);
	//	Safe_Release(pBoss_Sandy_Body);

}