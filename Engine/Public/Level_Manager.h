#pragma once

#include "Base.h"

/* 현재 게임내에 보여주는 레벨을 보관한다. */
/* 레벨의 교체 작업을 수행한다. (이전레벨용 객체들을 정리한다.) */

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