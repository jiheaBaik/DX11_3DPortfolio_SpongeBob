#pragma once

#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{	
		_float3		vEye, vAt;
		_float		fFovy, fAspect, fNear, fFar;
		CTransform::TRANSFORMDESC		TransformDesc;
	}CAMERADESC;

public:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT  Render();	

protected:	
	CAMERADESC						m_CameraDesc;
	class CPipeLine*				m_pPipeLine = nullptr;

protected:
	HRESULT Bind_Matrices();


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END