#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct NaviDesc
	{
		_int		iCurrentIndex;
	}NAVIDESC;

private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pNavigationDataFile);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	_bool isMove(_fvector vPosition);
#ifdef _DEBUG
public:
	HRESULT Render();
	HRESULT ToolRender();
	HRESULT Set_CellsPush(_float3 _meshPos[]);
	HRESULT Set_CellsPop();// { m_Cells.pop_back(); }
	
#endif // _DEBUG

private:
	_int					m_iCurrentIndex = -1;
public:
	_float Get_pos() { return m_YPos; }
private:
	vector<class CCell*>				m_Cells;
	vector<class CCell*>				m_CellsTemp;

	typedef vector<class CCell*>		CELLS;

#ifdef _DEBUG
private:
	class CShader*			m_pShader = nullptr;
#endif // _DEBUG

private:
	HRESULT Ready_Neighbor();
	_float m_YPos;
public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFile);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END