#pragma once

#include "Base.h"

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
public:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	ID3D11ShaderResourceView* Get_SRV(const _tchar* pRenderTargetTag) const ;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext*	pContext);
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pRenderTargetTag); /* 렌더타겟들을 모아서 보관하겠다. */

	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);
	HRESULT End_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);

#ifdef _DEBUG
public:
	HRESULT Ready_Debug_Desc(const _tchar* pRenderTargetTag, _float fLeftX, _float fTopY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(const _tchar* pMRTTag);
#endif // _DEBUG

private:
	map<const _tchar*, class CRenderTarget*>			m_RenderTargets;
	typedef map<const _tchar*, class CRenderTarget*>	RENDERTARGETS;

	map<const _tchar*, list<class CRenderTarget*>>			m_MRTs;
	typedef map<const _tchar*, list<class CRenderTarget*>>	MRTS;

private:
	ID3D11RenderTargetView*						m_pBackBufferView = nullptr;
	ID3D11DepthStencilView*						m_pDepthStencilView = nullptr;

#ifdef _DEBUG
private:
	class CShader*								m_pShader = nullptr;
	class CVIBuffer_Rect*						m_pVIBuffer = nullptr;
	_float4x4									m_ViewMatrix;
	_float4x4									m_ProjMatrix;
#endif // _DEBUG

private:
	class CRenderTarget* Find_RenderTarget(const _tchar* pRenderTargetTag) const;
	list<CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);

public:
	virtual void Free() override;
};

END