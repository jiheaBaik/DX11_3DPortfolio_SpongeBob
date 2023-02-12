#pragma once

#include "Base.h"

/* ���� ���ӳ��� �����ִ� ������ �����Ѵ�. */
/* ������ ��ü �۾��� �����Ѵ�. (���������� ��ü���� �����Ѵ�.) */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Change_Level(_uint iCurrentLevelIndex, class CLevel* pNextLevel);
	HRESULT Tick(_double TimeDelta);
	HRESULT LateTick(_double TimeDelta);
	HRESULT Render();

private:
	class CLevel*			m_pCurrentLevel = nullptr;
	_uint					m_iCurrentLevelIndex = 0;

public:
	virtual void Free() override;
};

END