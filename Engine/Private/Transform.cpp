#include "..\Public\Transform.h"
#include "Navigation.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CTransform::CTransform(const CTransform & Prototype)
	: CComponent(Prototype)
	, m_WorldMatrix(Prototype.m_WorldMatrix)
{

}

_float3 CTransform::Get_Scaled()
{
	_float3		vScale;

	vScale.x = XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_RIGHT)));
	vScale.y = XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_UP)));
	vScale.z = XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_LOOK)));

	return vScale;
}

_float4x4 CTransform::Get_WorldFloat4x4_TP() const
{	
	_float4x4		TransposeMatrix;

	XMStoreFloat4x4(&TransposeMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));

	return TransposeMatrix;	
}

void CTransform::Set_Scaled(const _float3 & vScale)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vRight = XMVector3Normalize(vRight) * vScale.x;
	vUp = XMVector3Normalize(vUp) * vScale.y;
	vLook = XMVector3Normalize(vLook) * vScale.z;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

HRESULT CTransform::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTransform::Go_Straight(_double TimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * _float(m_TransformDesc.SpeedPerSec * TimeDelta);

	if (nullptr != pNavigation)
	{
		if (false == pNavigation->isMove(vPosition))
		{
			int a = 10;
			return;
		}
	}

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_double TimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * _float(m_TransformDesc.SpeedPerSec * TimeDelta);
	if (nullptr != pNavigation)
	{
		if (false == pNavigation->isMove(vPosition))
			return;
	}
	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_double TimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * _float(m_TransformDesc.SpeedPerSec * TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_double TimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * _float(m_TransformDesc.SpeedPerSec * TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_float3		vScale = Get_Scaled();

	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	_matrix		RotationMatrix;

	m_fAngle = fRadian;
	m_iRotationAxist = vAxis;
	RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Turn(_fvector vAxis, _double TimeDelta)
{
	_matrix		RotationMatrix;

	RotationMatrix = XMMatrixRotationAxis(vAxis, _float(m_TransformDesc.RotationPerSec * TimeDelta));

	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	// XMVector4Transform();
	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

_bool CTransform::Chase(_fvector vTargetPos, _double TimeDelta, _float fLimitDistance)
{	
	LookAt(vTargetPos);

	if (fLimitDistance <= Compute_Distance(vTargetPos))
	{
		Go_Straight(TimeDelta);
		return false;
	}
	else
		return true;
}

void CTransform::LookAt(_fvector vTargetPos)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	_vector		vLook = vTargetPos - vPosition;

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);

	_vector		vUp = XMVector3Cross(vLook, vRight);

	_float3		vScale = Get_Scaled();

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	Set_Scaled(vScale);	
}

_float CTransform::Compute_Distance(_fvector vTargetPos)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	return XMVectorGetX(XMVector3Length(vPosition - vTargetPos));
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform*		pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*		pInstance = new CTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTransform::Free()
{
	__super::Free();


}
