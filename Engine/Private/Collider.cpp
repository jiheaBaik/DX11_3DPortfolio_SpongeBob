#include "..\Public\Collider.h"
#include "DebugDraw.h"
#include "PipeLine.h"

CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CCollider::CCollider(const CCollider & Prototype)
	: CComponent(Prototype)	
	, m_eType(Prototype.m_eType)
#ifdef _DEBUG
	, m_pInputLayout(Prototype.m_pInputLayout)

	, m_pEffect(Prototype.m_pEffect)
	, m_pBatch(Prototype.m_pBatch)
#endif // _DEBUG
{
	if (nullptr != Prototype.m_pAABB_Original)
		m_pAABB_Original = new BoundingBox(*Prototype.m_pAABB_Original);
	if (nullptr != Prototype.m_pOBB_Original)
		m_pOBB_Original = new BoundingOrientedBox(*Prototype.m_pOBB_Original);
	if (nullptr != Prototype.m_pSphere_Original)
		m_pSphere_Original = new BoundingSphere(*Prototype.m_pSphere_Original);

#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG
}

HRESULT CCollider::NativeConstruct_Prototype(TYPE eType)
{
	switch (eType)
	{
	case TYPE_AABB:
		m_pAABB_Original = new BoundingBox(_float3(0.f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f));
		break;
	case TYPE_OBB:
		m_pOBB_Original = new BoundingOrientedBox(_float3(0.f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f), _float4(0.f, 0.f, 0.f, 1.f));
		break;
	case TYPE_SPHERE:
		m_pSphere_Original = new BoundingSphere(_float3(0.f, 0.f, 0.f), 0.5f);
		break;
	}

	m_eType = eType;

#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	if (nullptr == m_pBatch)
		return E_FAIL;

	m_pEffect = new BasicEffect(m_pDevice);
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t	ShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

#endif // _DEBUG

	return S_OK;
}

HRESULT CCollider::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
	{
		COLLIDERDESC		ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

		ColliderDesc = *(COLLIDERDESC*)pArg;

		switch (m_eType)
		{
		case TYPE_AABB:
			m_pAABB_Original->Center = ColliderDesc.vTranslation;
			m_pAABB_Original->Extents = _float3(ColliderDesc.vScale.x * 0.5f, ColliderDesc.vScale.y * 0.5f, ColliderDesc.vScale.z * 0.5f);

			m_pAABB = new BoundingBox(*m_pAABB_Original);			
			break;
		case TYPE_OBB:
			m_pOBB_Original->Center = ColliderDesc.vTranslation;
			m_pOBB_Original->Extents = _float3(ColliderDesc.vScale.x * 0.5f, ColliderDesc.vScale.y * 0.5f, ColliderDesc.vScale.z * 0.5f);

			m_pOBB = new BoundingOrientedBox(*m_pOBB_Original);
			break;
		case TYPE_SPHERE:
			m_pSphere_Original->Center = ColliderDesc.vTranslation;
			m_pSphere_Original->Radius = ColliderDesc.vScale.x * 0.5f;

			m_pSphere = new BoundingSphere(*m_pSphere_Original);
			break;
		}
	}

	return S_OK;
}
_bool CCollider::Collision(CCollider * pCollider)
{
	switch (m_eType)
	{
	case TYPE_AABB:
		{
			TYPE		eTargetType = pCollider->m_eType;

			if (TYPE_AABB == eTargetType)			
				m_isCollision = m_pAABB->Intersects(*pCollider->m_pAABB);
			if (TYPE_OBB == eTargetType)
				m_isCollision = m_pAABB->Intersects(*pCollider->m_pOBB);
			if (TYPE_SPHERE == eTargetType)
				m_isCollision = m_pAABB->Intersects(*pCollider->m_pSphere);		
		}
		break;
	case TYPE_OBB:
	{
		TYPE		eTargetType = pCollider->m_eType;

		if (TYPE_AABB == eTargetType)
			m_isCollision = m_pOBB->Intersects(*pCollider->m_pAABB);
		if (TYPE_OBB == eTargetType)
			m_isCollision = m_pOBB->Intersects(*pCollider->m_pOBB);
		if (TYPE_SPHERE == eTargetType)
			m_isCollision = m_pOBB->Intersects(*pCollider->m_pSphere);
	}
	break;
	case TYPE_SPHERE:
		{
			TYPE		eTargetType = pCollider->m_eType;

			if (TYPE_AABB == eTargetType)
				m_isCollision = m_pSphere->Intersects(*pCollider->m_pAABB);
			if (TYPE_OBB == eTargetType)
				m_isCollision = m_pSphere->Intersects(*pCollider->m_pOBB);
			if (TYPE_SPHERE == eTargetType)
				m_isCollision = m_pSphere->Intersects(*pCollider->m_pSphere);
		}
	break;

	}

	//return _bool();
	return m_isCollision;

}
_bool CCollider::Collision_AABB(CCollider * pCollider)
{
	_float3		vSourMin = Compute_MinPoint();
	_float3		vDestMin = pCollider->Compute_MinPoint();

	_float3		vSourMax = Compute_MaxPoint();
	_float3		vDestMax = pCollider->Compute_MaxPoint();


	m_isCollision = false;

	/* 너비가 겹치지 않았는지? */
	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))	
		return false;

	/* 높이가 겹치지 않았는지? */
	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		return false;

	/* 깊이가 겹치지 않았는지? */
	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
		return false;

	m_isCollision = true;

	return true;	
}

_bool CCollider::Collision_OBB(CCollider * pCollider)
{
	OBBDESC			OBBDesc[2];

	OBBDesc[0] = Compute_OBBDesc();
	OBBDesc[1] = pCollider->Compute_OBBDesc();

	m_isCollision = false;

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			_float		fDistance[3] = { 0.0f };

			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter),
				XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[0]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[1]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[2]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[0]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[1]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[2]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			if (fDistance[0] > fDistance[1] + fDistance[2])
				return false;
		}
	}

	m_isCollision = true;

	return true;
}

_bool CCollider::Collision_SPHERE(CCollider * pCollider)
{
	return _bool();
}

void CCollider::Update(_fmatrix TransformMatrix)
{
	switch (m_eType)
	{
	case TYPE_AABB:
		m_pAABB_Original->Transform(*m_pAABB, Remove_Rotation(TransformMatrix));
		break;
	case TYPE_OBB:
		m_pOBB_Original->Transform(*m_pOBB, TransformMatrix);
		break;
	case TYPE_SPHERE:
		m_pSphere_Original->Transform(*m_pSphere, TransformMatrix);
		break;
	}
}


#ifdef _DEBUG

HRESULT CCollider::Render()
{
	m_pContext->GSSetShader(nullptr, nullptr, 0);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->SetWorld(XMMatrixIdentity());

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	m_pEffect->SetView(pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	RELEASE_INSTANCE(CPipeLine);

	_vector		vColor = m_isCollision == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);

	m_pEffect->Apply(m_pContext);

	m_pBatch->Begin();	

	switch (m_eType)
	{
	case TYPE_AABB:
		DX::Draw(m_pBatch, *m_pAABB, vColor);
		break;
	case TYPE_OBB:
		DX::Draw(m_pBatch, *m_pOBB, vColor);
		break;
	case TYPE_SPHERE:
		DX::Draw(m_pBatch, *m_pSphere, vColor);
		break;
	}

	m_pBatch->End();

	return S_OK;
}
#endif // _DEBUG

_matrix CCollider::Remove_Rotation(_fmatrix TransformMatrix)
{
	_matrix		Result = TransformMatrix;

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[0]));
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[1]));
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[2]));

	Result.r[0] = vRight;
	Result.r[1] = vUp;
	Result.r[2] = vLook;	

	return Result;
}

_float3 CCollider::Compute_MinPoint()
{
	if (nullptr == m_pAABB)
		return _float3(0.f, 0.f, 0.f);

	return _float3(m_pAABB->Center.x - m_pAABB->Extents.x, 
		m_pAABB->Center.y - m_pAABB->Extents.y, 
		m_pAABB->Center.z - m_pAABB->Extents.z);	
}

_float3 CCollider::Compute_MaxPoint()
{
	if (nullptr == m_pAABB)
		return _float3(0.f, 0.f, 0.f);

	return _float3(m_pAABB->Center.x + m_pAABB->Extents.x,
		m_pAABB->Center.y + m_pAABB->Extents.y,
		m_pAABB->Center.z + m_pAABB->Extents.z);
}

CCollider::OBBDESC CCollider::Compute_OBBDesc()
{
	if (nullptr == m_pOBB)
		return OBBDESC();

	OBBDESC			OBBDesc;
	ZeroMemory(&OBBDesc, sizeof(OBBDESC));

	_float3		vPoints[8];

	m_pOBB->GetCorners(vPoints);

	OBBDesc.vCenter = m_pOBB->Center;

	XMStoreFloat3(&OBBDesc.vCenterAxis[0], 
		(XMLoadFloat3(&vPoints[5]) + XMLoadFloat3(&vPoints[2])) * 0.5f - 
		XMLoadFloat3(&OBBDesc.vCenter));

	XMStoreFloat3(&OBBDesc.vCenterAxis[1],
		(XMLoadFloat3(&vPoints[5]) + XMLoadFloat3(&vPoints[7])) * 0.5f -
		XMLoadFloat3(&OBBDesc.vCenter));

	XMStoreFloat3(&OBBDesc.vCenterAxis[2],
		(XMLoadFloat3(&vPoints[5]) + XMLoadFloat3(&vPoints[0])) * 0.5f -
		XMLoadFloat3(&OBBDesc.vCenter));


	XMStoreFloat3(&OBBDesc.vAlignAxis[0],
		XMVector3Normalize(XMLoadFloat3(&vPoints[5]) - XMLoadFloat3(&vPoints[4])));

	XMStoreFloat3(&OBBDesc.vAlignAxis[1],
		XMVector3Normalize(XMLoadFloat3(&vPoints[5]) - XMLoadFloat3(&vPoints[1])));

	XMStoreFloat3(&OBBDesc.vAlignAxis[2],
		XMVector3Normalize(XMLoadFloat3(&vPoints[5]) - XMLoadFloat3(&vPoints[6])));

	
	return OBBDesc;
}

CCollider * CCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eType)
{
	CCollider*		pInstance = new CCollider(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType)))
	{
		MSG_BOX("Failed to Created : CCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider*		pInstance = new CCollider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCollider");
		Safe_Release(pInstance);
	}
	return pInstance;

}

void CCollider::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pInputLayout);
#endif // _DEBUG

	Safe_Delete(m_pAABB_Original);
	Safe_Delete(m_pOBB_Original);
	Safe_Delete(m_pSphere_Original);

	Safe_Delete(m_pAABB);
	Safe_Delete(m_pOBB);
	Safe_Delete(m_pSphere);

#ifdef _DEBUG
	if (false == m_isCloned)
	{
		Safe_Delete(m_pEffect);
		Safe_Delete(m_pBatch);
	}
	
#endif // _DEBUG





}
