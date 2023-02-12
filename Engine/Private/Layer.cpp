#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pComponentTag, _uint iIndex)
{
	auto	iter = m_ObjectList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Component(pComponentTag);	
}

CGameObject * CLayer::Get_GameObject(_uint iIndex)
{
	if (iIndex >= m_ObjectList.size())
		return nullptr;

	auto iter = m_ObjectList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter);
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	m_ObjectList.push_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::Delete_GameObject(CGameObject * pGameObject)
{
	std::list<CGameObject*>::iterator it;
	it = std::find(m_ObjectList.begin(), m_ObjectList.end(), pGameObject);

	if (it != m_ObjectList.end())
	{
		Safe_Release(pGameObject);
		m_ObjectList.erase(it);
	}
	else
		return E_FAIL;
	return S_OK;
}

HRESULT CLayer::NativeConstruct()
{

	return S_OK;
}

void CLayer::Tick(_double TimeDelta)
{
	for (auto& iter = m_ObjectList.begin(); iter != m_ObjectList.end();)
	{
		if (nullptr != *iter && (*iter)->Get_Dead()) {
			Safe_Release(*iter);
			*iter = nullptr;
			iter = m_ObjectList.erase(iter);
			continue;
		}
		else if (nullptr != *iter) {
			(*iter)->Tick(TimeDelta);
			iter++;
		}
	}
	
}

void CLayer::LateTick(_double TimeDelta)
{
	for (auto& pGameObject : m_ObjectList)
	{
		if (nullptr != pGameObject)
			pGameObject->LateTick(TimeDelta);
	}
}

CLayer * CLayer::Create()
{
	CLayer*		pInstance = new CLayer();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Created : CLayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_ObjectList)
		Safe_Release(pGameObject);

	m_ObjectList.clear();

}

