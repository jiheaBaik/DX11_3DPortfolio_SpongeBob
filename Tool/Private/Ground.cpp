#include "..\Public\Ground.h"



#include "GameInstance.h"

CGround::CGround(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CGround::CGround(const CGround & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CGround::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGround::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (static_cast<OBJINFODESC*>(pArg) != nullptr)
		m_Info = *(OBJINFODESC*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void CGround::Tick(_double TimeDelta)
{
}

void CGround::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CGround::Render()
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
		//if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		//	return E_FAIL;
		//if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		//	return E_FAIL;

		//	m_pShaderCom->Begin(0);

		m_pVIBufferCom->Render(i, m_pShaderCom, 0);
	}

	

//	m_pNavigationCom->ToolRender();

	return S_OK;
}

HRESULT CGround::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Ground"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	///* For.Com_Navigation */
	//CNavigation::NAVIDESC		NaviDesc;
	//ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));
	//NaviDesc.iCurrentIndex = 0;
	///* for.Com_Navigation */

	//if (FAILED(__super::Add_Component(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
	//	return E_FAIL;

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

HRESULT CGround::SetUp_ConstantTable()
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

CGround * CGround::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CGround*		pInstance = new CGround(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CGroung");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGround::Clone(void * pArg)
{
	CGround*		pInstance = new CGround(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGroung");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGround::Free()
{
	__super::Free();
	//Safe_Release(m_pNavigationCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}