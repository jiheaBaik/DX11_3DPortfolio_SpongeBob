#include "..\Public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CTexture::CTexture(const CTexture & Prototype)
	: CComponent(Prototype)
	, m_SRVs(Prototype.m_SRVs)
{
	for (auto& pSRV : m_SRVs)
		Safe_AddRef(pSRV);
}

HRESULT CTexture::NativeConstruct_Prototype(const _tchar * pTextureFilePath, _uint iNumTextures)
{
	CoInitializeEx(nullptr, 0);//왠진 모르겠지만 png 이걸쓰면 가능(시간날때 CoInitializeEx에 대해 공부하기)

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		_tchar			szExt[MAX_PATH] = TEXT("");
		_tchar			szFullPath[MAX_PATH] = TEXT("");

		wsprintf(szFullPath, pTextureFilePath, i);

		_wsplitpath_s(szFullPath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		ID3D11ShaderResourceView*			pSRV = nullptr;

		HRESULT			hr = 0;

		if (false == lstrcmp(szExt, TEXT(".dds")))
			hr = DirectX::CreateDDSTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);

		else
			hr = DirectX::CreateWICTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);

		if (FAILED(hr))
			return E_FAIL;

		m_SRVs.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::SetUp_SRV(CShader * pShader, const char * pConstantName, _uint iTextureIndex)
{
	if (nullptr == pShader)
		return E_FAIL;

	return	pShader->Set_SRV(pConstantName, m_SRVs[iTextureIndex]);
}

CTexture * CTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	CTexture*		pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	CTexture*		pInstance = new CTexture(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pSRV : m_SRVs)
		Safe_Release(pSRV);

	m_SRVs.clear();


}
