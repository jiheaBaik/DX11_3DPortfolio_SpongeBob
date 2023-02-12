#include "..\Public\VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& Prototype)
	:CComponent(Prototype)
	, m_BufferDesc(Prototype.m_BufferDesc)
	, m_BufferSubResourceData(Prototype.m_BufferSubResourceData)
	, m_pVB(Prototype.m_pVB)
	, m_iNumBuffers(Prototype.m_iNumBuffers)
	, m_iStride(Prototype.m_iStride)
	, m_iNumVertices(Prototype.m_iNumVertices)
	, m_pIB(Prototype.m_pIB)
	, m_iNumPrimitive(Prototype.m_iNumPrimitive)
	, m_iNumIndicesPerPrimitive(Prototype.m_iNumIndicesPerPrimitive)
	, m_iBytePerPrimitive(Prototype.m_iBytePerPrimitive)
	, m_eFormat(Prototype.m_eFormat)
	, m_eTopology(Prototype.m_eTopology)
	, m_pVerticesPos(Prototype.m_pVerticesPos)
	, m_pIndices(Prototype.m_pIndices)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::NativeConstruct(void * pArg)
{
	return S_OK;
}


HRESULT CVIBuffer::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	ID3D11Buffer* pVertexBuffers[] = { m_pVB };
	_uint iStrides[] = { m_iStride };
	_uint iOffset[] = {0};

	m_pContext->IASetVertexBuffers(0, m_iNumBuffers, pVertexBuffers, iStrides, iOffset);
	m_pContext->IASetIndexBuffer(m_pIB, m_eFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	m_pContext->DrawIndexed(m_iNumIndicesPerPrimitive * m_iNumPrimitive, 0, 0);

	return S_OK;
}
HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_BufferSubResourceData, &m_pVB)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_BufferSubResourceData, &m_pIB)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

	if (false == m_isCloned) {
		Safe_Delete_Array(m_pVerticesPos);
		Safe_Delete_Array(m_pIndices);
	}

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}

