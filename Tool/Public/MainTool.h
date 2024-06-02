#pragma once
#include "Imgui_Manager.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
class CVIBuffer_Rect;
class CVIBuffer_Terrain;
class CTexture;
END

BEGIN(Tool)
class CMainTool final : public CBase
{
private:
	CMainTool();
	virtual ~CMainTool() = default;

public:
	HRESULT NativeConstruct();
	void Tick(float fTimeDelta);
	HRESULT Render();

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	CGameInstance*		m_pGameInstance = nullptr;
	CImgui_Manager*		m_pImgui_Manager = nullptr;

	CRenderer*				m_pRenderer = nullptr;

#ifdef _DEBUG
private:
	_uint				m_iNumRender = 0;
	_float				m_fTimeAcc = 0.f;
#endif // _DEBUG

private:
	HRESULT Ready_RenderState();
	HRESULT Ready_Prototype_Component_Static();
	HRESULT Ready_Prototype_GameObject();

public:
	static CMainTool* Create();
	virtual void Free() override;

};

END