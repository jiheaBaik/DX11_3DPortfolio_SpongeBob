#pragma once
#include "Base.h"
#include <sqlite3.h>

BEGIN(Engine)
class CDatabaseManager final : public CBase
{
	DECLARE_SINGLETON(CDatabaseManager)
public:
	CDatabaseManager();
	virtual ~CDatabaseManager() = default;

public:
	virtual void Free() override;
};
END
