#pragma once


#include "Base.h"
BEGIN(Engine)
class CHp_Management final : public CBase
{
	DECLARE_SINGLETON(CHp_Management)
public:
	CHp_Management();
	virtual ~CHp_Management() = default;

	void ManageMent();
	_int Get_Hp() { return m_iHp; }
	void Set_Hp(_int _hp) { m_iHp += _hp; }
	void Inital_Hp() { m_iHp = 3; }
private:
	_int m_iHp;

public:
	virtual void Free() override;
};

END