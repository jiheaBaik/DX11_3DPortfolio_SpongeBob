#pragma once

#include "Base.h"
BEGIN(Engine)
class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking)

public:
	CPicking();
	virtual ~CPicking() = default;
public:
	HRESULT Initialize(HWND hWnd, ID3D11Device** pDevice, ID3D11DeviceContext** pContext);
	void Compute_RayInWorldSpace();
	_float Get_Pos(class CTransform* pTransform, class CTransform* pPlayerTransform, class CVIBuffer* pVIBuffer);

	_bool Picking(class CTransform* pTransform, class CVIBuffer* pVIBuffer, _float3* pOut = nullptr);
	//_bool RaySphereIntersect(_float3 m_vRayDir, _float3 m_vRayPos,_float radius);
private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	HWND				m_hWnd = 0;
	class CPipeLine*				m_pPipeLine = nullptr;

private:
	_vector				m_vRayDir;
	_vector				m_vRayPos;

public:
	virtual void Free() override;

};

END