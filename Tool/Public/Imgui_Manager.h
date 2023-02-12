#pragma once

#include "Tool_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CLevel;
END


BEGIN(Tool)
class CImgui_Manager final :public CBase
{
	DECLARE_SINGLETON(CImgui_Manager)
private:
	CImgui_Manager();
	virtual ~CImgui_Manager() = default;

public:
	HRESULT NativeConstruct(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Frame();
	HRESULT Render();

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	CLevel*				m_pCurrentLevel = nullptr;
	LEVEL				m_eCurrentLevel = LEVEL_END;

private:
	void Render_Menu();

public:
	virtual void Free() override;
};

END