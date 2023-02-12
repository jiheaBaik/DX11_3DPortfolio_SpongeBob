#include "..\Public\VIBuffer_Cube.h"



CVIBuffer_Cube::CVIBuffer_Cube(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube & Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Cube::NativeConstruct_Prototype()
{
	m_iStride = sizeof(VTXCUBETEX);
	m_iNumVertices = 8;
	m_iNumBuffers = 1;

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	VTXCUBETEX*			pVertices = new VTXCUBETEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXCUBETEX) * m_iNumVertices);

	m_pVerticesPos[0] = pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexUV = pVertices[0].vPosition;

	m_pVerticesPos[1] = pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexUV = pVertices[0].vPosition;

	m_pVerticesPos[2] = pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexUV = pVertices[0].vPosition;

	m_pVerticesPos[3] = pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexUV = pVertices[0].vPosition;

	m_pVerticesPos[4] = pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexUV = pVertices[0].vPosition;

	m_pVerticesPos[5] = pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexUV = pVertices[0].vPosition;

	m_pVerticesPos[6] = pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[6].vTexUV = pVertices[0].vPosition;

	m_pVerticesPos[7] = pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexUV = pVertices[0].vPosition;


	ZeroMemory(&m_BufferSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_BufferSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

	m_iNumPrimitive = 12;
	m_iNumIndicesPerPrimitive = 3;
	m_iBytePerPrimitive = sizeof(FACEINDICES16);
	m_eFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iBytePerPrimitive * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	FACEINDICES16*			pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	/* +X */
	((FACEINDICES32*)m_pIndices)[0]._1 = pIndices[0]._1 = 1; ((FACEINDICES32*)m_pIndices)[0]._2 = pIndices[0]._2 = 5; ((FACEINDICES32*)m_pIndices)[0]._3 = pIndices[0]._3 = 6;
	((FACEINDICES32*)m_pIndices)[1]._1 = pIndices[1]._1 = 1; ((FACEINDICES32*)m_pIndices)[1]._2 = pIndices[1]._2 = 6; ((FACEINDICES32*)m_pIndices)[1]._3 = pIndices[1]._3 = 2;

	/* -X */
	((FACEINDICES32*)m_pIndices)[2]._1 = pIndices[2]._1 = 4; ((FACEINDICES32*)m_pIndices)[2]._2 = pIndices[2]._2 = 0; ((FACEINDICES32*)m_pIndices)[2]._3 = pIndices[2]._3 = 3;
	((FACEINDICES32*)m_pIndices)[3]._1 = pIndices[3]._1 = 4; ((FACEINDICES32*)m_pIndices)[3]._2 = pIndices[3]._2 = 3; ((FACEINDICES32*)m_pIndices)[3]._3 = pIndices[3]._3 = 7;

	/* +Y */
	((FACEINDICES32*)m_pIndices)[4]._1 = pIndices[4]._1 = 4; ((FACEINDICES32*)m_pIndices)[4]._2 = pIndices[4]._2 = 5; ((FACEINDICES32*)m_pIndices)[4]._3 = pIndices[4]._3 = 1;
	((FACEINDICES32*)m_pIndices)[5]._1 = pIndices[5]._1 = 4; ((FACEINDICES32*)m_pIndices)[5]._2 = pIndices[5]._2 = 1; ((FACEINDICES32*)m_pIndices)[5]._3 = pIndices[5]._3 = 0;

	/* -Y */
	((FACEINDICES32*)m_pIndices)[6]._1 = pIndices[6]._1 = 3; ((FACEINDICES32*)m_pIndices)[6]._2 = pIndices[6]._2 = 2; ((FACEINDICES32*)m_pIndices)[6]._3 = pIndices[6]._3 = 6;
	((FACEINDICES32*)m_pIndices)[7]._1 = pIndices[7]._1 = 3; ((FACEINDICES32*)m_pIndices)[7]._2 = pIndices[7]._2 = 6; ((FACEINDICES32*)m_pIndices)[7]._3 = pIndices[7]._3 = 7;

	/* +Z */
	((FACEINDICES32*)m_pIndices)[8]._1 = pIndices[8]._1 = 5; ((FACEINDICES32*)m_pIndices)[8]._2 = pIndices[8]._2 = 4; ((FACEINDICES32*)m_pIndices)[8]._3 = pIndices[8]._3 = 7;
	((FACEINDICES32*)m_pIndices)[9]._1 = pIndices[9]._1 = 5; ((FACEINDICES32*)m_pIndices)[9]._2 = pIndices[9]._2 = 7; ((FACEINDICES32*)m_pIndices)[9]._3 = pIndices[9]._3 = 6;

	/* -Z */
	((FACEINDICES32*)m_pIndices)[10]._1 = pIndices[10]._1 = 0; ((FACEINDICES32*)m_pIndices)[10]._2 = pIndices[10]._2 = 1; ((FACEINDICES32*)m_pIndices)[10]._3 = pIndices[10]._3 = 2;
	((FACEINDICES32*)m_pIndices)[11]._1 = pIndices[11]._1 = 0; ((FACEINDICES32*)m_pIndices)[11]._2 = pIndices[11]._2 = 2; ((FACEINDICES32*)m_pIndices)[11]._3 = pIndices[11]._3 = 3;

	ZeroMemory(&m_BufferSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_BufferSubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma region

	return S_OK;
}

HRESULT CVIBuffer_Cube::NativeConstruct(void * pArg)
{
	return S_OK;
}

CVIBuffer_Cube * CVIBuffer_Cube::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_Cube*		pInstance = new CVIBuffer_Cube(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Cube::Clone(void * pArg)
{
	CVIBuffer_Cube*		pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();
}
