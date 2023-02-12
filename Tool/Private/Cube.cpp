#include "..\Public\Cube.h"
#include "GameInstance.h"

CCube::CCube(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CCube::CCube(const CCube & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CCube::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CCube::NativeConstruct(void * pArg)
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

void CCube::Tick(_double TimeDelta)
{
}

void CCube::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CCube::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin(m_ShaderColor);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CCube::SetUp_Components()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	D3D11_RASTERIZER_DESC wfd;
	ID3D11RasterizerState*		m_WireframeRS = nullptr;
	ZeroMemory(&wfd, sizeof(D3D11_RASTERIZER_DESC));
	wfd.FillMode = D3D11_FILL_SOLID;
	wfd.CullMode = D3D11_CULL_NONE;
	wfd.DepthClipEnable = true;
	m_pDevice->CreateRasterizerState(&wfd, &m_WireframeRS);
	m_pContext->RSSetState(m_WireframeRS);
	Safe_Release(m_WireframeRS);
	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	///* For.Com_Texture */
	//if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_TERRAIN, TEXT("Prototype_Component_Texture_Terrain"), (CComponent**)&m_pTextureCom)))
	//	return E_FAIL;

	///* For.Com_Shader */
	//if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_TERRAIN, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
	//	return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	if (m_Info.fScale.x == 0.f || m_Info.fScale.x == NULL)
		m_Info.fScale = _float3(1.0f, 1.0f, 1.0f);
	m_pTransformCom->Set_Scaled(_float3(m_Info.fScale.x, m_Info.fScale.y, m_Info.fScale.z));
	if (m_Info.fPos.x == 0.f || m_Info.fScale.x == NULL)
		m_Info.fPos = _float3(0.f, 0.f, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_Info.fPos.x, m_Info.fPos.y, m_Info.fPos.z,1));

	return S_OK;
}

HRESULT CCube::SetUp_ConstantTable()
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

	/*if (FAILED(m_pTextureCom->SetUp_ConstantTable(m_pShaderCom, "g_DiffuseTexture", 0)))
	return E_FAIL;*/

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CCube * CCube::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCube*		pInstance = new CCube(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCube::Clone(void * pArg)
{
	CCube*		pInstance = new CCube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCube::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
