#include "..\Public\VIBuffer_Cell.h"



CVIBuffer_Cell::CVIBuffer_Cell(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{

}

CVIBuffer_Cell::CVIBuffer_Cell(const CVIBuffer_Cell & Prototype)
	: CVIBuffer(Prototype)
{

}
HRESULT CVIBuffer_Cell::NativeConstruct_Prototype(const _float3 * pPoints)
{
	m_iStride = sizeof(VTXCOL);
	m_iNumVertices = 3;
	m_iNumBuffers = 1;

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXCOL*			pVertices = new VTXCOL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXCOL) * m_iNumVertices);

	pVertices[0].vPosition = pPoints[0];
	pVertices[0].vColor = _float4(1.0f, 1.f, 1.0f, 1.f);

	pVertices[1].vPosition = pPoints[1];
	pVertices[1].vColor = _float4(1.0f, 1.f, 1.0f, 1.f);

	pVertices[2].vPosition = pPoints[2];
	pVertices[2].vColor = _float4(1.0f, 1.f, 1.0f, 1.f);

	ZeroMemory(&m_BufferSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_BufferSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion
	m_eFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = sizeof(_ushort) * 4;
	m_BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_ushort*			pIndices = new _ushort[4];
	ZeroMemory(pIndices, sizeof(_ushort) * 4);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	pIndices[3] = 0;

	ZeroMemory(&m_BufferSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_BufferSubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma region

	return S_OK;

}

HRESULT CVIBuffer_Cell::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Cell::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	ID3D11Buffer*		pVertexBuffers[] = {
		m_pVB
	};

	_uint				iStrides[] = {
		m_iStride
	};

	_uint				iOffset[] = {
		0
	};


	m_pContext->IASetVertexBuffers(0, m_iNumBuffers, pVertexBuffers, iStrides, iOffset);
	m_pContext->IASetIndexBuffer(m_pIB, m_eFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	m_pContext->DrawIndexed(4, 0, 0);

	return S_OK;
}

CVIBuffer_Cell * CVIBuffer_Cell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _float3 * pPoints)
{
	CVIBuffer_Cell*		pInstance = new CVIBuffer_Cell(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pPoints)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Cell");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Cell::Clone(void * pArg)
{
	CVIBuffer_Cell*		pInstance = new CVIBuffer_Cell(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Cell::Free()
{
	__super::Free();
}
