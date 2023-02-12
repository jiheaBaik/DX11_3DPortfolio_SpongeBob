#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& Prototype);
	virtual ~CTexture() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT SetUp_SRV(class CShader* pShader, const char* pConstantName, _uint iTextureIndex);

private:
	vector<ID3D11ShaderResourceView*>				m_SRVs;
	typedef vector<ID3D11ShaderResourceView*>		SRVS;
public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END