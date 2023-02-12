#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
public:
	typedef struct ShaderDesc
	{
		ID3DX11EffectPass*		pPass;
		ID3D11InputLayout*		pInputLayout;
	}SHADERDESC;

private:
	explicit CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CShader(const CShader& Prototype);
	virtual ~CShader() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElement, _uint iNumElement);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	/* 셰이더 전역변수에 클라이언트에서 던지고 싶은 값을 전달한다. */
	HRESULT Set_RawValue(const char* pConstantName, const void* pData, _uint iLength);
	HRESULT Set_SRV(const char* pConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT Begin(_uint iPassIndex);

private:
	ID3DX11Effect*				m_pEffect = nullptr;

	vector<SHADERDESC>			m_ShaderDesces;
	typedef vector<SHADERDESC>	SHADERDESCS;

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElement, _uint iNumElement);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END