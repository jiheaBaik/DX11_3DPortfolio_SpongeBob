#include "..\Public\PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{

}

_float4x4 CPipeLine::Get_Transformfloat4x4_TP(TRANSFORMSTATE eState)
{
	_float4x4		TransformMatrix;
	
	XMStoreFloat4x4(&TransformMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_StateMatrix[eState])));

	return TransformMatrix;
}

void CPipeLine::Tick()
{
	
	_matrix		CamWorldMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_StateMatrix[D3DTS_VIEW]));

	XMStoreFloat3(&m_vCamPosition, CamWorldMatrix.r[3]);

}

void CPipeLine::Free()
{
}
