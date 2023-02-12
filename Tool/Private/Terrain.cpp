#include "..\Public\Terrain.h"
#include "GameInstance.h"
CTerrain::CTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CTerrain::CTerrain(const CTerrain & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CTerrain::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));	

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void CTerrain::Tick(_double TimeDelta)
{

}

void CTerrain::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CTerrain::Render()
{
	D3D11_RASTERIZER_DESC wfd;
	ID3D11RasterizerState*		m_WireframeRS = nullptr;
	ZeroMemory(&wfd, sizeof(D3D11_RASTERIZER_DESC));
	wfd.FillMode = D3D11_FILL_WIREFRAME;
	wfd.CullMode = D3D11_CULL_NONE;
	wfd.DepthClipEnable = true;
	m_pDevice->CreateRasterizerState(&wfd, &m_WireframeRS);
	m_pContext->RSSetState(m_WireframeRS);
	Safe_Release(m_WireframeRS);

	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin(4);

	m_pVIBufferCom->Render();

//	D3D11_RASTERIZER_DESC wfd;
//	ID3D11RasterizerState*		m_WireframeRS = nullptr;
	ZeroMemory(&wfd, sizeof(D3D11_RASTERIZER_DESC));
	wfd.FillMode = D3D11_FILL_SOLID;
	wfd.CullMode = D3D11_CULL_NONE;
	wfd.DepthClipEnable = true;
	m_pDevice->CreateRasterizerState(&wfd, &m_WireframeRS);
	m_pContext->RSSetState(m_WireframeRS);
	Safe_Release(m_WireframeRS);

	return S_OK;
}

HRESULT CTerrain::SetUp_Components()
{

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
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::SetUp_ConstantTable()
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

void CTerrain::SetTerrain(_uint x, _uint y)
{
	//m_pVIBufferCom->SetTerrain(x, y);
}

CTerrain * CTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTerrain*		pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTerrain::Clone(void * pArg)
{
	CTerrain*		pInstance = new CTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
