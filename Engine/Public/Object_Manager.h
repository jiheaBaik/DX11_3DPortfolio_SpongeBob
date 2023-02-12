#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);

public:
	HRESULT Reserve_Container(_uint iNumLevels);

	/* 객체들의 원형을 추가한다.ㅏ */
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pGameObject);

	/* 객체들의 사본을 생성하여 m_pLayers에 추가해준다. */
	HRESULT Clone_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg);
	HRESULT Distroy_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _bool IsAll, _uint iIndex = 0);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex = 0);
public:
	void Tick(_double TimeDelta);
	void LateTick(_double TimeDelta);

public:
	HRESULT Clear(_uint iLevelIndex);
	
private:
	map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef map<const _tchar*, class CGameObject*>	PROTOTYPES;

private:
	/* 내가 지정한 집합별로 객체들을 보관하낟. */
	/* ex: 언데스몬스터레이어. 배경객체레이어. 유아이레이어. */
	map<const _tchar*, class CLayer*>*			m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>	LAYERS;

	_uint										m_iNumLevels = 0;

private:
	class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	virtual void Free() override;
};

END