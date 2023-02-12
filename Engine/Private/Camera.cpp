#include "..\Public\Camera.h"
#include "PipeLine.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
	, m_pPipeLine(CPipeLine::Get_Instance())
{
	Safe_AddRef(m_pPipeLine);
}

CCamera::CCamera(const CCamera & Prototype)
	: CGameObject(Prototype)
	, m_pPipeLine(Prototype.m_pPipeLine)
{
	Safe_AddRef(m_pPipeLine);
}

HRESULT CCamera::NativeConstruct_Prototype()
{

	return S_OK;
}

HRESULT CCamera::NativeConstruct(void * pArg)
{
	if(nullptr != pArg)
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	if (FAILED(__super::NativeConstruct(&m_CameraDesc.TransformDesc)))
		return E_FAIL;		

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&m_CameraDesc.vEye));

	m_pTransformCom->LookAt(XMLoadFloat3(&m_CameraDesc.vAt));

	return S_OK;
}

void CCamera::Tick(_double TimeDelta)
{
	if (FAILED(Bind_Matrices()))
		return;
}

void CCamera::LateTick(_double TimeDelta)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

HRESULT CCamera::Bind_Matrices()
{
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	/*카메라 월드행렬의 역행렬을 구한다. == 뷰행렬 */
	/* 장치에 바인드한다. */

	_matrix		ViewMatrix = XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix());
	m_pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, ViewMatrix);

	_matrix		ProjMatrix;
	ProjMatrix = XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	m_pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, ProjMatrix);
	
	return S_OK;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pPipeLine);

	Safe_Release(m_pTransformCom);

}
