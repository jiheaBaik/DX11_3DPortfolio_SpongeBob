#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& Prototype);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual HRESULT Render();

public:
	_uint Get_NumPrimitive() const {
		return m_iNumPrimitive;
	}

	const _float3* Get_VerticesPos() const {
		return m_pVerticesPos;
	}

	const void* Get_Indices() const {
		return m_pIndices;
	}

	DXGI_FORMAT Get_IndexFormat() const {
		return m_eFormat;
	}

protected:
	D3D11_BUFFER_DESC			m_BufferDesc;
	D3D11_SUBRESOURCE_DATA		m_BufferSubResourceData;

protected:
	ID3D11Buffer*				m_pVB = nullptr;
	_uint						m_iNumBuffers = 0;
	_uint						m_iStride = 0;
	_uint						m_iNumVertices = 0;
	_float3*				    m_pVerticesPos = nullptr;
protected:
	ID3D11Buffer*				m_pIB = nullptr;
	_uint						m_iNumPrimitive = 0;
	_uint						m_iNumIndicesPerPrimitive = 0;
	_uint						m_iBytePerPrimitive = 0;
	DXGI_FORMAT					m_eFormat;
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology;
	void*								m_pIndices = nullptr;
protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();



public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END