#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_double			SpeedPerSec;
		_double			RotationPerSec;

		tagTransformDesc() = default;
		tagTransformDesc(_double _SpeedPerSec, _double _RotationPerSec) 
			: SpeedPerSec(_SpeedPerSec), RotationPerSec(_RotationPerSec) 
		{
		}
	}TRANSFORMDESC;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& Prototype);
	virtual ~CTransform() = default;

public:
	_vector Get_State(STATE eState) {
		return XMLoadFloat4((_float4*)&m_WorldMatrix.m[eState][0]);
	}

	_float3 Get_Scaled();

	_float Get_Angle(){ return m_fAngle;}
	_vector Get_RotationAxist() { return m_iRotationAxist; }

	void Set_Angle(_float _angle) {  m_fAngle = _angle; }


	_float4x4 Get_WorldFloat4x4() const {
		return m_WorldMatrix;
	}

	_float4x4 Get_WorldFloat4x4_TP() const;

	_matrix Get_WorldMatrix() const {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}


	void Set_State(STATE eState, _fvector vState) {
		XMStoreFloat3((_float3*)&m_WorldMatrix.m[eState][0], vState);
	}

	void Set_Scaled(const _float3& vScale);

	void Set_TransformDesc(const TRANSFORMDESC& TransformDesc) {
		m_TransformDesc = TransformDesc;
	}

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

public:
	void Go_Straight(_double TimeDelta, class CNavigation* pNavigation = nullptr);

	void Go_Backward(_double TimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Right(_double TimeDelta);
	void Go_Left(_double TimeDelta);

	void Rotation(_fvector vAxis, _float fRadian);
	void Turn(_fvector vAxis, _double TimeDelta);
	_bool Chase(_fvector vTargetPos, _double TimeDelta, _float fLimitDistance = 0.1f);
	void LookAt(_fvector vTargetPos);

	_float Compute_Distance(_fvector vTargetPos);
	

private:
	_float4x4			m_WorldMatrix;
	TRANSFORMDESC		m_TransformDesc;
	_float m_fAngle = 0.f;
	_vector m_iRotationAxist = {};
public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END