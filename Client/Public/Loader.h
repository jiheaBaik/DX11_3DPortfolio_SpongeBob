#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* ������ ������� */
/* �����Ҵ�� ������ �ڿ����� �ε��Ѵ�.  */

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;

public:
	LEVEL Get_NextLevelID() {
		return m_eNextLevel;
	}

	CRITICAL_SECTION* Get_CriticalSection() {
		return &m_CriticalSection;
	}

	const _tchar* Get_LoadingText() {
		return m_szLoading;
	}

	_bool isFinished() {
		return m_isFinished;
	}

public:
	HRESULT NativeConstruct(LEVEL eNextLevel);

public:
	HRESULT Loading_ForLogoLevel();
	HRESULT Loading_ForMenuLevel();
	HRESULT Loading_ForFieldLevel();
	HRESULT Loading_ForGamePlayLevel();

	
private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

private:
	_bool				m_isFinished = false;
	LEVEL				m_eNextLevel = LEVEL_END;
	_tchar				m_szLoading[MAX_PATH] = TEXT("");

private:
	HANDLE				m_hThread = 0;
	CRITICAL_SECTION	m_CriticalSection;




public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel);
	virtual void Free() override;
};

END