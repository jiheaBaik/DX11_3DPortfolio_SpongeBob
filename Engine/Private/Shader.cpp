#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CShader::CShader(const CShader & Prototype)
	: CComponent(Prototype)
	, m_pEffect(Prototype.m_pEffect)
	, m_ShaderDesces(Prototype.m_ShaderDesces)
{
	Safe_AddRef(m_pEffect);

	for (auto& ShadeDesc : m_ShaderDesces)
	{
		Safe_AddRef(ShadeDesc.pInputLayout);
		Safe_AddRef(ShadeDesc.pPass);
	}
}



/*
]LPCSTR SemanticName;
UINT SemanticIndex;
DXGI_FORMAT Format;
UINT InputSlot;
UINT AlignedByteOffset;
D3D11_INPUT_CLASSIFICATION InputSlotClass;
UINT InstanceDataStepRate;
*/

HRESULT CShader::NativeConstruct_Prototype(const _tchar * pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElement, _uint iNumElement)
{
	/*D3DCompileFromFile();

	D3DX11CreateEffectFromMemory();*/

	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL0 | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;

#endif
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;
	/*
	D3DX11_EFFECT_DESC		EffectDesc;
	ZeroMemory(&EffectDesc, sizeof(D3DX11_EFFECT_DESC));

	m_pEffect->GetDesc(&EffectDesc);*/

	ID3DX11EffectTechnique*		pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC	TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

	pTechnique->GetDesc(&TechniqueDesc);

	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		SHADERDESC			ShaderDesc;

		ShaderDesc.pPass = pTechnique->GetPassByIndex(i);

		D3DX11_PASS_DESC	PassDesc;
		ZeroMemory(&PassDesc, sizeof(D3DX11_PASS_DESC));

		ShaderDesc.pPass->GetDesc(&PassDesc);

		if (FAILED(m_pDevice->CreateInputLayout(pElement, iNumElement, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &ShaderDesc.pInputLayout)))
			return E_FAIL;

		m_ShaderDesces.push_back(ShaderDesc);
	}

	return S_OK;
}

HRESULT CShader::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CShader::Set_RawValue(const char* pConstantName, const void* pData, _uint iLength)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	/* 지정해준 이름에 해당하는 셰이더 전역변수의 컴객체를 얻어온다. */
	ID3DX11EffectVariable*	pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	/* 해당 변수에 전달해준 값을 전달한다. */
	/* 텍스쳐의 값 x */
	return pVariable->SetRawValue(pData, 0, iLength);
}

HRESULT CShader::Set_SRV(const char * pConstantName, ID3D11ShaderResourceView * pSRV)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	/* 지정해준 이름에 해당하는 셰이더 전역변수의 컴객체를 얻어온다. */
	ID3DX11EffectVariable*	pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable*	pSRVVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVVariable)
		return E_FAIL;

	/* 해당 변수에 전달해준 값을 전달한다. */
	return pSRVVariable->SetResource(pSRV);
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (iPassIndex >= m_ShaderDesces.size())
		return E_FAIL;

	m_pContext->IASetInputLayout(m_ShaderDesces[iPassIndex].pInputLayout);

	m_ShaderDesces[iPassIndex].pPass->Apply(0, m_pContext);

	return S_OK;
}

CShader * CShader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElement, _uint iNumElement)
{
	CShader*		pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pShaderFilePath, pElement, iNumElement)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CShader::Clone(void * pArg)
{
	CShader*		pInstance = new CShader(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	for (auto& ShadeDesc : m_ShaderDesces)
	{
		Safe_Release(ShadeDesc.pInputLayout);
		Safe_Release(ShadeDesc.pPass);
	}

	m_ShaderDesces.clear();

	Safe_Release(m_pEffect);
}
