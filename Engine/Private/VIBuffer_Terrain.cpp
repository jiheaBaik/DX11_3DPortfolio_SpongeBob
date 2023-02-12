#include "..\Public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{

}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & Prototype)
	: CVIBuffer(Prototype)
	, m_iNumVerticesX(Prototype.m_iNumVerticesX)
	, m_iNumVerticesZ(Prototype.m_iNumVerticesZ)
{

}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	m_iStride = sizeof(VTXNORTEX);
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	m_iNumBuffers = 1;
	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	VTXNORTEX*			pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	for (_uint i = 0; i < iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX; ++j)
		{
			//FXMVECTOR XMFLOAT3
			_uint iIndex = i * iNumVerticesX + j;
			m_pVerticesPos[iIndex] = pVertices[iIndex].vPosition = _float3(j, 0.f, i);
			pVertices[iIndex].vNormal = _float3(0.0f, 0.f, 0.f);
			pVertices[iIndex].vTexUV = _float2(j / (iNumVerticesX - 1.0f), i / (iNumVerticesZ - 1.f));
		}
	}

	m_iNumPrimitive = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;
	m_iNumIndicesPerPrimitive = 3;
	m_iBytePerPrimitive = sizeof(FACEINDICES32);
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	FACEINDICES32*			pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	_uint		iNumFaces = 0;

	for (_uint i = 0; i < iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * iNumVerticesX + j;

			_uint iIndices[] = {
				iIndex + iNumVerticesX,
				iIndex + iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_float3 out;
			_float3 out1;
			((FACEINDICES32*)m_pIndices)[iNumFaces]._1 = pIndices[iNumFaces]._1 = iIndices[0];
			((FACEINDICES32*)m_pIndices)[iNumFaces]._2 = pIndices[iNumFaces]._2 = iIndices[1];
			((FACEINDICES32*)m_pIndices)[iNumFaces]._3 = pIndices[iNumFaces]._3 = iIndices[2];

			_float3 p0 = m_pVerticesPos[iIndices[0]];
			_float3 p1 = m_pVerticesPos[iIndices[1]];
			_float3 p2 = m_pVerticesPos[iIndices[2]];

			ComputeNormal(&p0, &p1, &p2, &out);

			++iNumFaces;

			((FACEINDICES32*)m_pIndices)[iNumFaces]._1 = pIndices[iNumFaces]._1 = iIndices[0];
			((FACEINDICES32*)m_pIndices)[iNumFaces]._2 = pIndices[iNumFaces]._2 = iIndices[2];
			((FACEINDICES32*)m_pIndices)[iNumFaces]._3 = pIndices[iNumFaces]._3 = iIndices[3];

			_float3 p3 = m_pVerticesPos[iIndices[0]];
			_float3 p4 = m_pVerticesPos[iIndices[2]];
			_float3 p5 = m_pVerticesPos[iIndices[3]];

			ComputeNormal(&p3, &p4, &p5, &out1);

			/*pVertices[iIndex].vNormal = */
			XMStoreFloat3(&pVertices[iIndex].vNormal, XMVector3Normalize(XMLoadFloat3(&out) + XMLoadFloat3(&out1)));

			++iNumFaces;

		}
	}
	for (_uint i = 0; i < m_iNumVertices; ++i)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));


	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_BufferSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_BufferSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iBytePerPrimitive * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_BufferSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_BufferSubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

#pragma region

	Safe_Delete_Array(pVertices);

	Safe_Delete_Array(pIndices);

	//Safe_Delete(m_pVerticesPos);
	//Safe_Delete(m_pIndices);
	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(const _tchar* pHeightMapFilePath)
{
	HANDLE			hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_ulong			dwByte = 0;

	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	_ulong*			pPixel = new _ulong[m_iNumVerticesX * m_iNumVerticesZ];
	ReadFile(hFile, pPixel, sizeof(_ulong) * m_iNumVerticesX * m_iNumVerticesZ, &dwByte, nullptr);

	CloseHandle(hFile);

	m_iStride = sizeof(VTXNORTEX);
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumBuffers = 1;

#pragma region VERTEX_BUFFER

	VTXNORTEX*			pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3(j, (pPixel[iIndex] & 0x000000ff) / 10.f, i);
			pVertices[iIndex].vNormal = _float3(0.0f, 0.f, 0.f);
			pVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.0f), i / (m_iNumVerticesZ - 1.f));
		}
	}
	





#pragma endregion
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;	
	m_iNumIndicesPerPrimitive = 3;
	m_iBytePerPrimitive = sizeof(FACEINDICES32);
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region INDEX_BUFFER


	FACEINDICES32*			pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	_uint		iNumFaces = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[] = {
				iIndex + m_iNumVerticesX, 
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex			
			};

			_vector		vSour, vDest, vNormal;

			pIndices[iNumFaces]._1 = iIndices[0];
			pIndices[iNumFaces]._2 = iIndices[1];
			pIndices[iNumFaces]._3 = iIndices[2];

			vSour = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._3].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._3].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._3].vNormal) + vNormal);	
			++iNumFaces;

			pIndices[iNumFaces]._1 = iIndices[0];
			pIndices[iNumFaces]._2 = iIndices[2];
			pIndices[iNumFaces]._3 = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._3].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._3].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._3].vNormal) + vNormal);
			++iNumFaces;			
		}
	}

	for (_uint i = 0; i < m_iNumVertices; ++i)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));

	
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_BufferSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
 	m_BufferSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iBytePerPrimitive * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_BufferSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_BufferSubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	

#pragma region

	Safe_Delete_Array(pVertices);

	Safe_Delete_Array(pIndices);

	Safe_Delete_Array(pPixel);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CVIBuffer_Terrain::ComputeNormal(_float3 * p0, _float3 * p1, _float3 * p2, _float3 * out)
{
	_fvector u = XMLoadFloat3(&(*p0)) - XMLoadFloat3(&(*p1));
	_fvector v = XMLoadFloat3(&(*p1)) - XMLoadFloat3(&(*p2));
	XMStoreFloat3(out, XMVector3Normalize(XMVector3Cross(v, u)));
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar* pHeightMapFilePath)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pHeightMapFilePath)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();


}
