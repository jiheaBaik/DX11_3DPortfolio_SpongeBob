#include "stdafx.h"
#include "..\Public\Light_Cone_Blue.h"

#include "GameInstance.h"
#include "Camera_Free.h"
CLight_Cone_Blue::CLight_Cone_Blue(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CLight_Cone_Blue::CLight_Cone_Blue(const CLight_Cone_Blue & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CLight_Cone_Blue::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CLight_Cone_Blue::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (static_cast<OBJINFODESC*>(pArg) != nullptr)
		m_Info = *(OBJINFODESC*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_dTotalTime = rand() % 5;
	return S_OK;
}

void CLight_Cone_Blue::Tick(_double TimeDelta)
{
	if (m_dTotalTime <= 5.f && !m_isChange)
	{
		m_isChange = false;
		m_dTotalTime += 0.01;
		m_fangle = 0.002;
	}
	else
	{
		m_isChange = true;
		m_dTotalTime -= 0.01;
		m_fangle = -0.002;
		if (m_dTotalTime <= 0.0f)
			m_isChange = false;
	}

	


	m_pTransformCom->Rotation(XMLoadFloat3(&_float3(0.f, 0.f, 1.f)), m_fangle);


	m_fDistance = 0.f;
	/*CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CTransform* m_pCameraTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Com_Transform"));
	_vector m_vCameraPos = m_pCameraTransform->Get_State(CTransform::STATE_POSITION);

	XMStoreFloat(&m_fDistance, XMVector3Length((m_vCameraPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION))));

	Safe_Release(pGameInstance);*/

}

void CLight_Cone_Blue::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CLight_Cone_Blue::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pVIBufferCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		
		m_pVIBufferCom->Render(i, m_pShaderCom, 5);

	}

	

	return S_OK;
}

HRESULT CLight_Cone_Blue::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Light_Cone_Blue"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (m_Info.fScale.x == 0.f || m_Info.fScale.x == NULL)
		m_Info.fScale = _float3(1.0f, 1.0f, 1.0f);
	m_pTransformCom->Set_Scaled(_float3(m_Info.fScale.x, m_Info.fScale.y, m_Info.fScale.z));
	if (m_Info.fPos.x == 0.f || m_Info.fScale.x == NULL)
		m_Info.fPos = _float3(0.f, 0.f, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_Info.fPos.x, m_Info.fPos.y, m_Info.fPos.z, 1));
	if (m_Info.fRotation != 0.f || m_Info.fRotation != NULL)
		m_pTransformCom->Rotation(XMLoadFloat3(&m_Info.fAxist), m_Info.fRotation);
	return S_OK;
}

HRESULT CLight_Cone_Blue::SetUp_ConstantTable()
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

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLight_Cone_Blue * CLight_Cone_Blue::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLight_Cone_Blue*		pInstance = new CLight_Cone_Blue(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CPos_Light");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CLight_Cone_Blue::Clone(void * pArg)
{
	CLight_Cone_Blue*		pInstance = new CLight_Cone_Blue(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPos_Light");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLight_Cone_Blue::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
