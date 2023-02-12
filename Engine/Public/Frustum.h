#pragma once

#include "Base.h"

/* 카메라의 시야범위 정보를 표현하는 객체이다. */

BEGIN(Engine)

class CFrustum final : public CBase
{
	DECLARE_SINGLETON(CFrustum)
private:
	CFrustum();
	virtual ~CFrustum() = default;

public:
	HRESULT NativeConstruct();
	void Transform_ToWorldSpace();
	void Transform_ToLocalSpace(_fmatrix WorldMatrixInv);

	_bool isIn_Frustum_World(_fvector vPoint, _float fRange = 0.f);
	_bool isIn_Frustum_Local(_fvector vPoint, _float fRange = 0.f);

private:
	_float3			m_vOriginal_Points[8];
	_float3			m_vWorld_Points[8];

	_float4			m_vPlaneInWorld[6];
	_float4			m_vPlaneInLocal[6];

public:
	virtual void Free() override;
};

END