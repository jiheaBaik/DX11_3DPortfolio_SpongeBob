#include "..\Public\Target_Manager.h"
#include "RenderTarget.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"

IMPLEMENT_SINGLETON(CTarget_Manager)

CTarget_Manager::CTarget_Manager()
{

}

ID3D11ShaderResourceView * CTarget_Manager::Get_SRV(const _tchar * pRenderTargetTag) const
{
	CRenderTarget*	pRenderTarget = Find_RenderTarget(pRenderTargetTag);
	if (nullptr == pRenderTarget)
		return nullptr;

	return pRenderTarget->Get_SRV();	
}

HRESULT CTarget_Manager::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{

#ifdef _DEBUG
	m_pShader = CShader::Create(pDevice, pContext, TEXT("../../Reference/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElement);
	if (nullptr == m_pShader)
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_Rect::Create(pDevice, pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	D3D11_VIEWPORT		Viewport;
	ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;
	pContext->RSGetViewports(&iNumViewports, &Viewport);
	

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f)));

#endif

	return S_OK;
}

HRESULT CTarget_Manager::Add_RenderTarget(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	if (nullptr != Find_RenderTarget(pRenderTargetTag))
		return E_FAIL;

	CRenderTarget*		pRenderTarget = CRenderTarget::Create(pDevice, pContext, iWidth, iHeight, eFormat, vClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(pRenderTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const _tchar * pMRTTag, const _tchar * pRenderTargetTag)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(pRenderTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;


	list<CRenderTarget*>*	pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;

		MRTList.push_back(pRenderTarget);

		m_MRTs.emplace(pMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(ID3D11DeviceContext* pContext, const _tchar * pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	pContext->OMGetRenderTargets(1, &m_pBackBufferView, &m_pDepthStencilView);

	ID3D11RenderTargetView*		pRTVs[8] = { nullptr };

	_uint			iNumRenderTargets = 0;

	for (auto& pRenderTarget : *pMRTList)	
	{ 		
		pRenderTarget->Clear();
		pRTVs[iNumRenderTargets++] = pRenderTarget->Get_RTV();
	}

	pContext->OMSetRenderTargets(iNumRenderTargets, pRTVs, m_pDepthStencilView);

	return S_OK;
}



HRESULT CTarget_Manager::End_MRT(ID3D11DeviceContext* pContext, const _tchar * pMRTTag)
{
	pContext->OMSetRenderTargets(1, &m_pBackBufferView, m_pDepthStencilView);

	Safe_Release(m_pBackBufferView);
	Safe_Release(m_pDepthStencilView);

	return S_OK;
}

#ifdef _DEBUG

HRESULT CTarget_Manager::Ready_Debug_Desc(const _tchar * pRenderTargetTag, _float fLeftX, _float fTopY, _float fSizeX, _float fSizeY)
{
	CRenderTarget*	pRenderTarget = Find_RenderTarget(pRenderTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_Debug_Desc(fLeftX, fTopY, fSizeX, fSizeY);	
}

HRESULT CTarget_Manager::Render_Debug(const _tchar* pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
	{
		if (nullptr != pRenderTarget)
		{
			/* 셰이더에 값을 던진다.(텍스쳐, 각 렌더타겟들의 디버깅장면을 어디에 어떤사이즈로 그릴지?!) */
			/* 셰이더 */

			m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
			m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

			pRenderTarget->Render(m_pShader, m_pVIBuffer);			
		}			
	}

	return S_OK;
}
#endif // _DEBUG

CRenderTarget * CTarget_Manager::Find_RenderTarget(const _tchar * pRenderTargetTag) const
{
	auto	iter = find_if(m_RenderTargets.begin(), m_RenderTargets.end(), CTag_Finder(pRenderTargetTag));

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;	
}

list<CRenderTarget*>* CTarget_Manager::Find_MRT(const _tchar * pMRTTag)
{
	auto	iter = find_if(m_MRTs.begin(), m_MRTs.end(), CTag_Finder(pMRTTag));

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)		
			Safe_Release(pRenderTarget);

		Pair.second.clear();		
	}
	m_MRTs.clear();

	for (auto& Pair : m_RenderTargets)
		Safe_Release(Pair.second);

	m_RenderTargets.clear();

#ifdef _DEBUG
	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);
#endif



}
