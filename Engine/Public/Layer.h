#pragma once

#include "Base.h"

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();	
	virtual ~CLayer() = default;

public:
	class CComponent* Get_Component(const _tchar* pComponentTag, _uint iIndex = 0);
	class CGameObject* Get_GameObject(_uint iIndex = 0);
public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	HRESULT Delete_GameObject(class CGameObject* pGameObject);
public:
	HRESULT NativeConstruct();
	void Tick(_double TimeDelta);
	void LateTick(_double TimeDelta);


private:
	list<class CGameObject*>			m_ObjectList;
	typedef list<class CGameObject*>	OBJECTS;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END