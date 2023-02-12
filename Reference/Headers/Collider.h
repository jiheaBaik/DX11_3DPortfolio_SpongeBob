#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

public:
	typedef struct tagOBBDesc
	{
		_float3		vCenter;
		_float3		vCenterAxis[3];
		_float3		vAlignAxis[3];
	}OBBDESC;

public:
	typedef struct tagColliderDesc
	{
		_float3		vScale;
		_float3		vTranslation;		
	}COLLIDERDESC;
public:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eType);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	_bool Collision(CCollider* pCollider);

	_bool Collision_AABB(CCollider* pCollider);
	_bool Collision_OBB(CCollider* pCollider);
	_bool Collision_SPHERE(CCollider* pCollider);

public:
	void Update(_fmatrix TransformMatrix);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif // _DEBUG

private:
	BoundingBox*				m_pAABB_Original = nullptr;
	BoundingOrientedBox*		m_pOBB_Original = nullptr;
	BoundingSphere*				m_pSphere_Original = nullptr;

	BoundingBox*				m_pAABB = nullptr;
	BoundingOrientedBox*		m_pOBB = nullptr;	
	BoundingSphere*				m_pSphere = nullptr;
	TYPE						m_eType = TYPE_END;

	_bool						m_isCollision = false;

#ifdef _DEBUG
private:
	PrimitiveBatch<VertexPositionColor>*			m_pBatch = nullptr;
	BasicEffect*									m_pEffect = nullptr;
	ID3D11InputLayout*								m_pInputLayout = nullptr;
#endif // _DEBUG

private:
	_matrix Remove_Rotation(_fmatrix TransformMatrix);
	_float3 Compute_MinPoint();
	_float3 Compute_MaxPoint();
	OBBDESC Compute_OBBDesc();

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END