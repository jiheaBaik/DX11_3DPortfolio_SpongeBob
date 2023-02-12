#include "stdafx.h"
#include "..\Public\Flame.h"
#include "GameInstance.h"

#include <iostream>




CFlame::CFlame(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
	//	pBoss_Sandy_Body = CBoss_Sandy_Body::Create(m_pDevice, m_pContext);

}

CFlame::CFlame(const CFlame & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CFlame::NativeConstruct_Prototype()
{


	return S_OK;
}

HRESULT CFlame::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&mFlameDesc, pArg, sizeof(FLAMEDESC));

	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_SocketMatrixPtr()))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_pTransformCom->Rotation(XMLoadFloat3(&_float3(1.f, 0.f, 0.f)), -89.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 4.f, -4.5, 0.f));

	return S_OK;
}

void CFlame::Tick(_double TimeDelta)
{


	m_pTransformCom->Set_Scaled(_float3(14, 14, 14));
}

void CFlame::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*			pTargetTransform = (CTransform*)pGameInstance->Get_Component(mFlameDesc.eLevel, mFlameDesc.pLayerTag, CGameObject::m_pTransformTag, mFlameDesc.iIndex);
	XMStoreFloat4x4(&m_SocketMatrix, XMLoadFloat4x4(&m_BoneOffsetMatrix) * XMLoadFloat4x4(m_pBoneMatrix) * XMLoadFloat4x4(&m_PivotMatrix) * pTargetTransform->Get_WorldMatrix());



		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CFlame::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pVIBufferCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{

		
			if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			m_pVIBufferCom->Render(i, m_pShaderCom,2);
		

	}



	return S_OK;
}

HRESULT CFlame::SetUp_Components()
{
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Boss_Sandy_Flame"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CFlame::SetUp_SRV()
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


HRESULT CFlame::SetUp_SocketMatrixPtr()
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	CModel*			pTargetModel = (CModel*)pGameInstance->Get_Component(mFlameDesc.eLevel, mFlameDesc.pLayerTag, mFlameDesc.pModelComTag, mFlameDesc.iIndex);
	if (nullptr == pTargetModel)
		return E_FAIL;

	m_pBoneMatrix = pTargetModel->Get_BoneMatrixPtr(mFlameDesc.pBoneName);
	if (nullptr == m_pBoneMatrix)
		return E_FAIL;

	m_BoneOffsetMatrix = pTargetModel->Get_BoneOffsetMatrix(mFlameDesc.pBoneName);
	m_PivotMatrix = pTargetModel->Get_PivotMatrix();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
CFlame * CFlame::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CFlame*		pInstance = new CFlame(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CForkLift");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CFlame::Clone(void * pArg)
{
	CFlame*		pInstance = new CFlame(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CForkLift");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFlame::Free()
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
