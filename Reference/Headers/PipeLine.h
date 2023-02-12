#pragma once

/* 게임내에서 사용될 뷰, 투영행렬을 보관한다. */
#include "Base.h"

BEGIN(Engine)

class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)

public:
	enum TRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

private:
	CPipeLine();
	virtual ~CPipeLine() = default;
public:
	_matrix Get_TransformMatrix(TRANSFORMSTATE eState) {
		return XMLoadFloat4x4(&m_StateMatrix[eState]);
	}

	_vector Get_CamPosition() const {
		return XMLoadFloat3(&m_vCamPosition);
	}

	_float3 Get_CamFloat3() const {
		return m_vCamPosition;
	}

	_float4x4 Get_Transformfloat4x4_TP(TRANSFORMSTATE eState);

	void Set_Transform(TRANSFORMSTATE eState, _fmatrix StateMatrix) {
		XMStoreFloat4x4(&m_StateMatrix[eState], StateMatrix);
	}

public:
	void Tick();

private:
	_float4x4			m_StateMatrix[D3DTS_END];
	_float3				m_vCamPosition;

public:	
	virtual void Free() override;
};

END