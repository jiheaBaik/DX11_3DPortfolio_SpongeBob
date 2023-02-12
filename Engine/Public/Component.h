#pragma once

#include "Base.h"

/* 모든 컴포넌트들의 부모다. */

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CComponent(const CComponent& Prototype);
	virtual ~CComponent() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

#ifdef _DEBUG
public:
	virtual HRESULT Render() { return S_OK; };
#endif // _DEBUG

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	_bool					m_isCloned = false;

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free();
};

END