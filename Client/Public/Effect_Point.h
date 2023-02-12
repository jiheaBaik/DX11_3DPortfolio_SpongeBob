#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Point_Instance;
END


BEGIN(Client)

class CEffect_Point final : public CGameObject
{
private:
	CEffect_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Point(const CEffect_Point& Prototype);
	virtual ~CEffect_Point() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;

private:
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CVIBuffer_Point_Instance*	m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();
	

public:
	void Reset();
	void IsRender(_bool _isRender) { m_IsRender = _isRender; }
	_bool m_IsRender = false;
	_uint m_icount = 0;
public:
	static CEffect_Point* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END