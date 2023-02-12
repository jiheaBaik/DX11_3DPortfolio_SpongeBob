#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT NativeConstruct();
	void Tick(_double TimeDelta);
	HRESULT Render();

private:
	CGameInstance*			m_pGameInstance = nullptr;

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

	CRenderer*				m_pRenderer = nullptr;

	_tchar					m_szFPS[MAX_PATH] = TEXT("");
	_tchar					m_szFlowerCount[MAX_PATH] = TEXT("");

	_uint					m_iNumRender = 0;
	_double					m_TimeAcc = 0.0;

public:
	HRESULT Open_Level(LEVEL eLevel);
	HRESULT Ready_Prototype_Components();
	HRESULT Ready_Gara();
	HRESULT Ready_Fonts();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END