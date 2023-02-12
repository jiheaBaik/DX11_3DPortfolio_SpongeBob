#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderTarget final : public CBase
{
public:
	CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderTarget() = default;

public:
	ID3D11RenderTargetView*	Get_RTV() const {
		return m_pRTV;
	}

	ID3D11ShaderResourceView*	Get_SRV() const {
		return m_pSRV;
	}

public:
	HRESULT NativeConstruct(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	HRESULT Clear();

public:
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);


#ifdef _DEBUG
public:
	HRESULT Ready_Debug_Desc(_float fLeftX, _float fTopY, _float fSizeX, _float fSizeY);
#endif // _DEBUG

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;

	ID3D11Texture2D*			m_pTexture = nullptr;
	ID3D11RenderTargetView*		m_pRTV = nullptr;
	ID3D11ShaderResourceView*	m_pSRV = nullptr;
	
private:
	_float4						m_vClearColor;

#ifdef _DEBUG
private:
	_float4x4					m_WorldMatrix;
#endif // _DEBUG


public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	virtual void Free() override;
};

END