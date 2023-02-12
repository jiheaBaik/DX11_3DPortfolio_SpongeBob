#pragma once
//class CHp_Management
//{
//public:
//	CHp_Management();
//	~CHp_Management();
//};

#include "Base.h"



BEGIN(Engine)
class CPlayer_Management final : public CBase
{
	DECLARE_SINGLETON(CPlayer_Management)

public:
	CPlayer_Management();
	virtual ~CPlayer_Management() = default;

	void ManageMent();
	_int Get_Player() { return m_iPlayerKind; }
	void Set_Player(_uint _player) { m_iPlayerKind = _player; }
private:
	_uint m_iPlayerKind;

public:
	virtual void Free() override;
};

END