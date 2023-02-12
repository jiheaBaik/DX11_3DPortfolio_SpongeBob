#include "..\Public\Navigation.h"
#include "Cell.h"
#include "Shader.h"
#include "PipeLine.h"

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_Cells(rhs.m_Cells)
#ifdef _DEBUG
	, m_pShader(rhs.m_pShader)
#endif // _DEBUG
{
#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif // _DEBUG

	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);

}

HRESULT CNavigation::NativeConstruct_Prototype(const _tchar * pNavigationDataFile)
{
	_ulong		dwByte = 0;
	HANDLE		hFile = CreateFile(pNavigationDataFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	while (true)
	{
		_float3		vPoints[3];

		ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
		if (0 == dwByte)
			break;

		CCell*		pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);

	if (FAILED(Ready_Neighbor()))
		return E_FAIL;

#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_Cell.hlsl"), VTXCOL_DECLARATION::Element, VTXCOL_DECLARATION::iNumElement);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif // _DEBUG


	return S_OK;
}

HRESULT CNavigation::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		m_iCurrentIndex = ((NAVIDESC*)pArg)->iCurrentIndex;

	return S_OK;
}

_bool CNavigation::isMove(_fvector vPosition)
{
	if (-1 == m_iCurrentIndex ||
		m_iCurrentIndex >= m_Cells.size())
		return false;

	_int		iNeighborIndex = -1;

	if (true == m_Cells[m_iCurrentIndex]->isIn(vPosition, &iNeighborIndex))
	{

		m_YPos = m_Cells[m_iCurrentIndex]->Get_YPos();
		return true;
	}
	else
	{

		/* 나간방향에 이웃이 있었다라면? */
		if (0 <= iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (true == m_Cells[iNeighborIndex]->isIn(vPosition, &iNeighborIndex))
				{
					m_iCurrentIndex = iNeighborIndex;
					break;
				}
				//else
					//break;
			}

			m_YPos = m_Cells[iNeighborIndex]->Get_YPos();
			return true;
		}
		else /* 이웃이 없었다라면. */
		{
			return false;
		}
	}


	return _bool();
}

#ifdef _DEBUG
HRESULT CNavigation::Render()
{
	if (nullptr == m_pShader)
		return E_FAIL;

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());


	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &pPipeLine->Get_Transformfloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &pPipeLine->Get_Transformfloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CPipeLine);

	if (FAILED(m_pShader->Set_RawValue("g_vColor", &_float4(0.f, 1.f, 0.f, 1.f), sizeof(_float4))))
		return E_FAIL;

	

	if (-1 == m_iCurrentIndex)
	{
		if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShader->Set_RawValue("g_vColor", &_float4(0.f, 1.f, 0.f, 1.f), sizeof(_float4))))
			return E_FAIL;

		m_pShader->Begin(0);

		for (auto& pCell : m_Cells)
		{
			if (nullptr != pCell)
				pCell->Render();
		}
	}
	else
	{
		WorldMatrix._42 = 0.1f;

		XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)));


		if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShader->Set_RawValue("g_vColor", &_float4(1.f, 0.f, 0.f, 1.f), sizeof(_float4))))
			return E_FAIL;

		m_pShader->Begin(0);

		m_Cells[m_iCurrentIndex]->Render();
	}


	return S_OK;
}
HRESULT CNavigation::ToolRender()
{

	if (nullptr == m_pShader)
		return E_FAIL;

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());


	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &pPipeLine->Get_Transformfloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &pPipeLine->Get_Transformfloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CPipeLine);


	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_vColor", &_float4(0.f, 0.f, 1.f, 1.f), sizeof(_float4))))
		return E_FAIL;

	m_pShader->Begin(0);

	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell)
			pCell->Render();
	}
	


	return S_OK;
}
HRESULT CNavigation::Set_CellsPush(_float3 _meshPos[])
{
	CCell*		pGetCell = CCell::Create(m_pDevice, m_pContext, _meshPos, m_Cells.size());
	if (nullptr == pGetCell)
		return E_FAIL;

	m_Cells.push_back(pGetCell);

	//이거 하면 왜 누수나는지 알기
	//Safe_AddRef(pCell);
	return S_OK;
}

HRESULT CNavigation::Set_CellsPop()
{
	m_CellsTemp.push_back(m_Cells[m_Cells.size() - 1]);
	m_Cells.pop_back();
	

	return S_OK;
}
#endif // _DEBUG

HRESULT CNavigation::Ready_Neighbor()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell->Get_Index());

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell->Get_Index());

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell->Get_Index());

		}
	}

	return S_OK;
}

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pNavigationDataFile)
{
	CNavigation*		pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pNavigationDataFile)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*		pInstance = new CNavigation(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavigation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();

	for (auto& pCellTemp : m_CellsTemp)
		Safe_Release(pCellTemp);

	m_CellsTemp.clear();

#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif // _DEBUG

}
