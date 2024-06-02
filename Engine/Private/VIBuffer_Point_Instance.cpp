#include "..\Public\VIBuffer_Point_Instance.h"

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{

}

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& Prototype)
	: CVIBuffer(Prototype)
	, m_iNumInstance(Prototype.m_iNumInstance)
	, m_iStrideInstance(Prototype.m_iStrideInstance)
	, m_pSpeeds(Prototype.m_pSpeeds)
{

}

HRESULT CVIBuffer_Point_Instance::NativeConstruct_Prototype(_uint iNumInstance)
{
	m_iNumInstance = iNumInstance;
	m_iStride = sizeof(VTXPOINT);
	m_iNumVertices = 1;
	m_iNumBuffers = 2;

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXPOINT* pVertices = new VTXPOINT[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOINT) * m_iNumVertices);

	pVertices[0].vPosition = _float3(0.0f, 0.0f, 0.f);
	pVertices[0].vPSize = _float2(0.3f, 0.3f);

	ZeroMemory(&m_BufferSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_BufferSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

	m_iNumPrimitive = m_iNumInstance;
	m_iNumIndicesPerPrimitive = 1;
	m_iBytePerPrimitive = sizeof(_ushort);
	m_eFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iBytePerPrimitive * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_ushort* pIndices = new _ushort[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumPrimitive);

	ZeroMemory(&m_BufferSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_BufferSubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	m_pSpeeds = new _float[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		m_pSpeeds[i] = rand() % 5 + 10.f;
	}
	return S_OK;


}

HRESULT CVIBuffer_Point_Instance::NativeConstruct(void* pArg)
{
#pragma region INSTANCE_BUFFER

	m_iStrideInstance = sizeof(VTXRECTINSTANCE);

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStrideInstance * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStrideInstance;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXRECTINSTANCE* pInstanceVertices = new VTXRECTINSTANCE[m_iNumInstance];
	ZeroMemory(pInstanceVertices, sizeof(VTXRECTINSTANCE) * m_iNumInstance);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstanceVertices[i].vRight = _float4(0.3f, 0.f, 0.f, 0.f);
		pInstanceVertices[i].vUp = _float4(0.f, 0.3f, 0.f, 0.f);
		pInstanceVertices[i].vLook = _float4(0.f, 0.f, 0.3f, 0.f);
		pInstanceVertices[i].vTranslation = _float4(rand() % 50, 13.f, rand() % 50, 1.f);
	}

	ZeroMemory(&m_BufferSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_BufferSubResourceData.pSysMem = pInstanceVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_BufferSubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstanceVertices);
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint				iStrides[] = {
		m_iStride,
		m_iStrideInstance

	};

	_uint				iOffset[] = {
		0,
		0
	};


	m_pContext->IASetVertexBuffers(0, m_iNumBuffers, pVertexBuffers, iStrides, iOffset);
	m_pContext->IASetIndexBuffer(m_pIB, m_eFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);
	m_pContext->DrawIndexedInstanced(1, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Point_Instance::Update(_double TimeDelta)
{
	if (nullptr == m_pContext)
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.y -= m_pSpeeds[i] * TimeDelta;

		if (((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.y < 0.f)
			((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.y = 13.f;
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Update2(_double TimeDelta)
{
	if (nullptr == m_pContext)
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.y -= m_pSpeeds[i] * TimeDelta;

		if (((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.y < 0.f)
			((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.y = rand() % 100;
	}

	m_pContext->Unmap(m_pVBInstance, 0);



}

void CVIBuffer_Point_Instance::FiledInitial()
{

	if (nullptr == m_pContext)
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{

		((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.y = rand() % 100;
		((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.x = rand() % 300;
		((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.z = rand() % 300;

	}

	m_pContext->Unmap(m_pVBInstance, 0);

}

void CVIBuffer_Point_Instance::GrassInitial(_float3 _pos, _vector _right, _vector _up, _vector _look)
{
	if (nullptr == m_pContext)
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	_float3 fRight, fUp, fLook;
	XMStoreFloat3(&fRight, _right);
	XMStoreFloat3(&fUp, _up);
	XMStoreFloat3(&fLook, _look);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		/*((VTXRECTINSTANCE*)SubResource.pData)[i].vRight = _float4(fRight.x, fRight.y, fRight.z, 0);
		((VTXRECTINSTANCE*)SubResource.pData)[i].vUp = _float4(fUp.x, fUp.y, fUp.z, 0);
		((VTXRECTINSTANCE*)SubResource.pData)[i].vLook = _float4(fLook.x, fLook.y, fLook.z, 0);*/

		((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.y = _pos.y;
		((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.x = _pos.x;
		((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.z = _pos.z;
		//m_fGrassAddPos += 0.1f;
	}


	//for (_uint i = 0; i < m_iNumInstance; ++i)
	//{

	//	//((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.y = _pos.y;
	////	((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.x += 0.05f;
	//	((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.z += 0.05f;
	//	//m_fGrassAddPos += 0.1f;
	//}

	m_pContext->Unmap(m_pVBInstance, 0);
}


void CVIBuffer_Point_Instance::Reset()
{
	if (nullptr == m_pContext)
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{



		//((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.x = 0.f;
		((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.y = 0.f;
		//((VTXRECTINSTANCE*)SubResource.pData)[i].vTranslation.z = 0.f;

	}

	m_pContext->Unmap(m_pVBInstance, 0);

}
CVIBuffer_Point_Instance* CVIBuffer_Point_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance)
{
	CVIBuffer_Point_Instance* pInstance = new CVIBuffer_Point_Instance(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(iNumInstance)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Point_Instance::Clone(void* pArg)
{
	CVIBuffer_Point_Instance* pInstance = new CVIBuffer_Point_Instance(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Point_Instance::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pSpeeds);

	Safe_Release(m_pVBInstance);
}
