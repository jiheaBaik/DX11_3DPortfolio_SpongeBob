#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_Instance final : public CVIBuffer
{
private:
	CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& Prototype);
	virtual ~CVIBuffer_Point_Instance() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(_uint iNumInstance);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render() override;

public:
	void Update(_double TimeDelta);
	void Update2(_double TimeDelta);

	void FiledInitial();
	void GrassInitial(_float3 _pos, _vector _right, _vector _up, _vector _look);

	void Reset();
	//void EditSpeed(_float _speed){ m_pSpeeds  = }
	
private:
	ID3D11Buffer*					m_pVBInstance = nullptr;
	_uint							m_iStrideInstance = 0;
	_uint							m_iNumInstance = 0;

private:
	_float*							m_pSpeeds = nullptr;
	_float m_fGrassAddPos = 0;
public:
	static CVIBuffer_Point_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END