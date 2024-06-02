#include "..\Public\CustomFont.h"
		
CCustomFont::CCustomFont(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);	
}

HRESULT CCustomFont::Initialize(const _tchar * pFontFilePath)
{
	m_pSpriteFont = new SpriteFont(m_pDevice, pFontFilePath);

	m_pSpriteBatch = new SpriteBatch(m_pContext);

	return S_OK;
}

HRESULT CCustomFont::Render(const _tchar* pString, const _float2& vPosition, _fvector vColor,/*, float rotation , XMFLOAT2 const& origin , */float scale)
{
	if (nullptr == m_pSpriteFont ||
		nullptr == m_pSpriteBatch)
		return E_FAIL;

	m_pContext->GSSetShader(nullptr, nullptr, 0);

	m_pSpriteBatch->Begin();
	XMFLOAT2 test = XMFLOAT2(0, 0);
	m_pSpriteFont->DrawString(m_pSpriteBatch, pString, vPosition, vColor, 0, XMFLOAT2(0,0), scale);

	m_pSpriteBatch->End();

	return S_OK;
}

CCustomFont * CCustomFont::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pFontFilePath)
{
	CCustomFont*		pInstance = new CCustomFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pFontFilePath)))
	{
		MSG_BOX("Failed to Created : CCustomFont");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCustomFont::Free()
{
	Safe_Delete(m_pSpriteBatch);
	Safe_Delete(m_pSpriteFont);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
