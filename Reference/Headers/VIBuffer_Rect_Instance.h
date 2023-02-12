#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instance final : public CVIBuffer
{
private:
	CVIBuffer_Rect_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& Prototype);
	virtual ~CVIBuffer_Rect_Instance() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(_uint iNumInstance);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render() override;

public:
	void Update(_double TimeDelta);
	void GrassInitial(_float3 _pos);
private:
	ID3D11Buffer*					m_pVBInstance = nullptr;
	_uint							m_iStrideInstance = 0;
	_uint							m_iNumInstance = 0;

private:
	_float*							m_pSpeeds = nullptr;

public:
	static CVIBuffer_Rect_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END