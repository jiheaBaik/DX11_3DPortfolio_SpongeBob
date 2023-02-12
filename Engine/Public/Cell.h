#pragma once

#include "Base.h"
#include "VIBuffer_Cell.h"

BEGIN(Engine)

class CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCell() = default;

public:
	_vector Get_Point(POINT ePoint) {
		return XMLoadFloat3(&m_vPoints[ePoint]);
	}
	_int Get_Index() const {
		return m_iIndex;
	}

public:
	void Set_Neighbor(LINE eLine, _int iNeighborIndex) {
		m_iNeighbor[eLine] = iNeighborIndex;
	}



public:
	HRESULT NativeConstruct(const _float3* pPoints, _int  iIndex);
	_bool Compare_Points(_fvector PointA, _fvector PointB);
	_float Get_YPos() { return m_GetfY; }
public:
	_bool isIn(_fvector vPosition, _int* pNeighborIndex);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif // _DEBUG

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

private:
	_float3				m_vPoints[POINT_END];
	_float3				m_vLine[LINE_END];
	_float3				m_vNormal[LINE_END];
	_int				m_iNeighbor[LINE_END] = { -1, -1, -1 };

	_int 				m_iIndex;

	_float		m_GetfY;
#ifdef _DEBUG
private:
	CVIBuffer_Cell*		m_pVIBuffer = nullptr;
#endif // _DEBUG



public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int  iIndex);
	virtual void Free();
};

END