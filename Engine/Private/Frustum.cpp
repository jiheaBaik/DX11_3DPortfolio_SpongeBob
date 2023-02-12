#include "..\Public\Frustum.h"
#include "PipeLine.h"

IMPLEMENT_SINGLETON(CFrustum)

CFrustum::CFrustum()
{
}

HRESULT CFrustum::NativeConstruct()
{
	/* 투영스페이스 상의 여덟개 점을 정의한다.  */
	m_vOriginal_Points[0] = _float3(-1.f, 1.f, 0.f);
	m_vOriginal_Points[1] = _float3(1.f, 1.f, 0.f);
	m_vOriginal_Points[2] = _float3(1.f, -1.f, 0.f);
	m_vOriginal_Points[3] = _float3(-1.f, -1.f, 0.f);

	m_vOriginal_Points[4] = _float3(-1.f, 1.f, 1.f);
	m_vOriginal_Points[5] = _float3(1.f, 1.f, 1.f);
	m_vOriginal_Points[6] = _float3(1.f, -1.f, 1.f);
	m_vOriginal_Points[7] = _float3(-1.f, -1.f, 1.f);

	return S_OK;
}

void CFrustum::Transform_ToWorldSpace()
{
	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix			ProjMatrix = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	_matrix			ViewMatrix = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);

	ProjMatrix = XMMatrixInverse(nullptr, ProjMatrix);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);

	for (_uint i = 0; i < 8; ++i)
	{
		XMStoreFloat3(&m_vWorld_Points[i], XMVector3TransformCoord(XMLoadFloat3(&m_vOriginal_Points[i]), ProjMatrix));
		XMStoreFloat3(&m_vWorld_Points[i], XMVector3TransformCoord(XMLoadFloat3(&m_vWorld_Points[i]), ViewMatrix));
	}

	XMStoreFloat4(&m_vPlaneInWorld[0], XMPlaneFromPoints(XMLoadFloat3(&m_vWorld_Points[1]), XMLoadFloat3(&m_vWorld_Points[5]), XMLoadFloat3(&m_vWorld_Points[6])));
	XMStoreFloat4(&m_vPlaneInWorld[1], XMPlaneFromPoints(XMLoadFloat3(&m_vWorld_Points[4]), XMLoadFloat3(&m_vWorld_Points[0]), XMLoadFloat3(&m_vWorld_Points[3])));
	XMStoreFloat4(&m_vPlaneInWorld[2], XMPlaneFromPoints(XMLoadFloat3(&m_vWorld_Points[4]), XMLoadFloat3(&m_vWorld_Points[5]), XMLoadFloat3(&m_vWorld_Points[1])));
	XMStoreFloat4(&m_vPlaneInWorld[3], XMPlaneFromPoints(XMLoadFloat3(&m_vWorld_Points[3]), XMLoadFloat3(&m_vWorld_Points[2]), XMLoadFloat3(&m_vWorld_Points[6])));
	XMStoreFloat4(&m_vPlaneInWorld[4], XMPlaneFromPoints(XMLoadFloat3(&m_vWorld_Points[5]), XMLoadFloat3(&m_vWorld_Points[4]), XMLoadFloat3(&m_vWorld_Points[7])));
	XMStoreFloat4(&m_vPlaneInWorld[5], XMPlaneFromPoints(XMLoadFloat3(&m_vWorld_Points[0]), XMLoadFloat3(&m_vWorld_Points[1]), XMLoadFloat3(&m_vWorld_Points[2])));

	RELEASE_INSTANCE(CPipeLine);		
}

void CFrustum::Transform_ToLocalSpace(_fmatrix WorldMatrixInv)
{
	_vector		vPoint[8];

	for (_uint i = 0; i < 8; ++i)	
		vPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&m_vWorld_Points[i]), WorldMatrixInv);

	XMStoreFloat4(&m_vPlaneInLocal[0], XMPlaneFromPoints(vPoint[1], vPoint[5], vPoint[6]));
	XMStoreFloat4(&m_vPlaneInLocal[1], XMPlaneFromPoints(vPoint[4], vPoint[0], vPoint[3]));
	XMStoreFloat4(&m_vPlaneInLocal[2], XMPlaneFromPoints(vPoint[4], vPoint[5], vPoint[1]));
	XMStoreFloat4(&m_vPlaneInLocal[3], XMPlaneFromPoints(vPoint[3], vPoint[2], vPoint[6]));
	XMStoreFloat4(&m_vPlaneInLocal[4], XMPlaneFromPoints(vPoint[5], vPoint[4], vPoint[7]));
	XMStoreFloat4(&m_vPlaneInLocal[5], XMPlaneFromPoints(vPoint[0], vPoint[1], vPoint[2]));
}

_bool CFrustum::isIn_Frustum_World(_fvector vPoint, _float fRange)
{
	/*
	a b c d
	x y z 1

	ax + by + cz + d < 0
	*/

	for (_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vPlaneInWorld[i]), vPoint)))
			return false;
	}	

	return true;	
}

_bool CFrustum::isIn_Frustum_Local(_fvector vPoint, _float fRange)
{
	for (_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vPlaneInLocal[i]), vPoint)))
			return false;
	}

	return true;
}

void CFrustum::Free()
{
}
