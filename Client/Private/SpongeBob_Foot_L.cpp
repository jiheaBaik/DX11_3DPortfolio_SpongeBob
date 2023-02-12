#include "stdafx.h"
#include "..\Public\SpongeBob_Foot_L.h"
#include "GameInstance.h"

CSpongeBob_Foot_L::CSpongeBob_Foot_L(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CSpongeBob_Foot_L::CSpongeBob_Foot_L(const CSpongeBob_Foot_L & Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CSpongeBob_Foot_L::NativeConstruct_Prototype()
{

	return S_OK;
}

HRESULT CSpongeBob_Foot_L::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_FootDesc, pArg, sizeof(FOOTDESC));

	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_SocketMatrixPtr()))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_pTransformCom->Rotation(XMLoadFloat3(&_float3(1.f, 0.f, 0.f)), 70.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-0.2f, 0.f, 0.f, 1.f));

	/*m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, -1.f, -1.2f, 1.f));

	m_pTransformCom->Set_Scaled(_float3(0.1f, 0.1f, 0.1f));*/

	return S_OK;
}

void CSpongeBob_Foot_L::Tick(_double TimeDelta)
{

}

void CSpongeBob_Foot_L::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*			pTargetTransform = (CTransform*)pGameInstance->Get_Component(m_FootDesc.eLevel, m_FootDesc.pLayerTag, CGameObject::m_pTransformTag, m_FootDesc.iIndex);
	XMStoreFloat4x4(&m_SocketMatrix, XMLoadFloat4x4(&m_BoneOffsetMatrix) * XMLoadFloat4x4(m_pBoneMatrix) * XMLoadFloat4x4(&m_PivotMatrix) * pTargetTransform->Get_WorldMatrix());


	if (pGameInstance->Get_Player_Info() == 0)
	{
		if (m_bIsRender)//플레이어가 공중에 띄워질 시 s키 누를때만 렌더
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	}
	
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CSpongeBob_Foot_L::Render()
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

		/*ID3D11BlendState * d3dBlendState;
		D3D11_BLEND_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));

		bd.AlphaToCoverageEnable = FALSE;
		bd.IndependentBlendEnable = FALSE;
		bd.RenderTarget[0].BlendEnable = TRUE;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		if (FAILED(m_pDevice->CreateBlendState(&bd, &d3dBlendState)))
			return false;

		m_pContext->OMSetBlendState(d3dBlendState, 0, 0xffffffff);*/


		m_pVIBufferCom->Render(i, m_pShaderCom, 2);

		//Safe_Release(d3dBlendState);
	}

	//ID3D11BlendState * d3dBlendState;
	//D3D11_BLEND_DESC bd;
	//ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	//bd.RenderTarget[0].BlendEnable = false;
	//bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	////Safe_AddRef(d3;
	//if (FAILED(m_pDevice->CreateBlendState(&bd, &d3dBlendState)))
	//	return false;

	//m_pContext->OMSetBlendState(d3dBlendState, 0, 0xffffffff);
	//Safe_Release(d3dBlendState);

	return S_OK;
}

HRESULT CSpongeBob_Foot_L::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_Model_SpongeBobFoot_L"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	
	/*m_pTransformCom->Set_Scaled(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Rotation(XMLoadFloat3(&_float3(1.f,0.f,0.f)), 90.f);
	m_pTransformCom->Rotation(XMLoadFloat3(&_float3(0.f,1.f, 0.f)), 180.f);*/

	return S_OK;
}

HRESULT CSpongeBob_Foot_L::SetUp_SRV()
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

HRESULT CSpongeBob_Foot_L::SetUp_SocketMatrixPtr()
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	CModel*			pTargetModel = (CModel*)pGameInstance->Get_Component(m_FootDesc.eLevel, m_FootDesc.pLayerTag, m_FootDesc.pModelComTag, m_FootDesc.iIndex);
	if (nullptr == pTargetModel)
		return E_FAIL;

	m_pBoneMatrix = pTargetModel->Get_BoneMatrixPtr(m_FootDesc.pBoneName);
	if (nullptr == m_pBoneMatrix)
		return E_FAIL;

	m_BoneOffsetMatrix = pTargetModel->Get_BoneOffsetMatrix(m_FootDesc.pBoneName);
	m_PivotMatrix = pTargetModel->Get_PivotMatrix();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CSpongeBob_Foot_L * CSpongeBob_Foot_L::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSpongeBob_Foot_L*		pInstance = new CSpongeBob_Foot_L(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSpongeBob_Foot_L::Clone(void * pArg)
{
	CSpongeBob_Foot_L*		pInstance = new CSpongeBob_Foot_L(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpongeBob_Foot_L::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
