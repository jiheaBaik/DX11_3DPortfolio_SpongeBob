#include "..\Public\Cell.h"

CCell::CCell(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCell::NativeConstruct(const _float3 * pPoints, _int  iIndex)
{
	m_vPoints[POINT_A] = pPoints[POINT_A];
	m_vPoints[POINT_B] = pPoints[POINT_B];
	m_vPoints[POINT_C] = pPoints[POINT_C];

	XMStoreFloat3(&m_vLine[LINE_AB], XMVector3Normalize(XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A])));
	XMStoreFloat3(&m_vLine[LINE_BC], XMVector3Normalize(XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B])));
	XMStoreFloat3(&m_vLine[LINE_CA], XMVector3Normalize(XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C])));

	m_vNormal[LINE_AB] = _float3(m_vLine[LINE_AB].z * -1.f, 0.f, m_vLine[LINE_AB].x);
	m_vNormal[LINE_BC] = _float3(m_vLine[LINE_BC].z * -1.f, 0.f, m_vLine[LINE_BC].x);
	m_vNormal[LINE_CA] = _float3(m_vLine[LINE_CA].z * -1.f, 0.f, m_vLine[LINE_CA].x);

	m_iIndex = iIndex;

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, pPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

_bool CCell::Compare_Points(_fvector SourPoint, _fvector DestPoint)
{
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), SourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), DestPoint))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), DestPoint))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), SourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), DestPoint))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), DestPoint))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), SourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), DestPoint))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), DestPoint))
			return true;
	}

	return false;
}

_bool CCell::isIn(_fvector vPosition, _int* pNeighborIndex)
{
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector	vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));

		if (0 < XMVectorGetX(XMVector3Dot(vDir, XMLoadFloat3(&m_vNormal[i]))))
		{
			*pNeighborIndex = m_iNeighbor[i];
			return false;
		}
	}

	_vector vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_vPoints[0]),
		XMLoadFloat3(&m_vPoints[1]),
		XMLoadFloat3(&m_vPoints[2]));

	// y = (-ax - cz - d) / b
	//ax+by+cz+d=0 
	m_GetfY = (-XMVectorGetX(vPlane) * XMVectorGetX(vPosition) - XMVectorGetZ(vPlane) * XMVectorGetZ(vPosition) - XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);
	

	return true;
}

#ifdef _DEBUG
HRESULT CCell::Render()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;



	m_pVIBuffer->Render();

	return S_OK;
}
#endif // _DEBUG

CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _float3 * pPoints, _int  iIndex)
{
	CCell*		pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct(pPoints, iIndex)))
	{
		MSG_BOX("Failed to Created : CCell");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCell::Free()
{
#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif // _DEBUG
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
