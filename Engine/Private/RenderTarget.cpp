#include "..\Public\RenderTarget.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

CRenderTarget::CRenderTarget(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderTarget::NativeConstruct(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	m_vClearColor = vClearColor;
	/* 렌더타겟을 생성한다. */
	

	/* 1.텍스쳐를 생성하낟.  */


	D3D11_TEXTURE2D_DESC		TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = eFormat;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture)))
		return E_FAIL;

	/* 2.텍스쳐의 정보를 활용하여 렌더타겟 뷰를 생성한다.  */
	if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture, nullptr, &m_pRTV)))
		return E_FAIL;

	/* 3.셰이더 리소스 뷰를 생성한다.   */
	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture, nullptr, &m_pSRV)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderTarget::Clear()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	m_pContext->ClearRenderTargetView(m_pRTV, (_float*)&m_vClearColor);

	return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderTarget::Render(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	pShader->Set_SRV("g_Texture", m_pSRV);

	pShader->Begin(0);

	pVIBuffer->Render();


	return S_OK;
}


HRESULT CRenderTarget::Ready_Debug_Desc(_float fLeftX, _float fTopY, _float fSizeX, _float fSizeY)
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	m_WorldMatrix._11 = fSizeX;
	m_WorldMatrix._22 = fSizeY;

	D3D11_VIEWPORT		Viewport;
	ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &Viewport);
	

	m_WorldMatrix._41 = fLeftX + fSizeX * 0.5f - Viewport.Width * 0.5f;
	m_WorldMatrix._42 = -(fTopY + fSizeY * 0.5f) + Viewport.Height * 0.5f;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));
	

	return S_OK;
}
#endif // _DEBUG

CRenderTarget * CRenderTarget::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	CRenderTarget*		pInstance = new CRenderTarget(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct(iWidth, iHeight, eFormat, vClearColor)))
	{
		MSG_BOX("Failed to Created : CRenderTarget");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRenderTarget::Free()
{
	if(0 == m_vClearColor.y)	
		SaveDDSTextureToFile(m_pContext, m_pTexture, TEXT("../Bin/Diffuse.dds"));

	Safe_Release(m_pTexture);

	Safe_Release(m_pRTV);
	Safe_Release(m_pSRV);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
