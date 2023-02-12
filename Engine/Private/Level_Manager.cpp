#include "..\Public\Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{


}

HRESULT CLevel_Manager::Change_Level(_uint iCurrentLevelIndex, CLevel * pNextLevel)
{
	if (nullptr == pNextLevel)
		return E_FAIL;

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if(nullptr != m_pCurrentLevel)
		pGameInstance->Clear_Container(m_iCurrentLevelIndex);	

	Safe_Release(m_pCurrentLevel);

	m_pCurrentLevel = pNextLevel;		

	m_iCurrentLevelIndex = iCurrentLevelIndex;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Manager::Tick(_double TimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	m_pCurrentLevel->Tick(TimeDelta);

	return S_OK;
}

HRESULT CLevel_Manager::LateTick(_double TimeDelta)
{

	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	m_pCurrentLevel->LateTick(TimeDelta);

	return S_OK;
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	return m_pCurrentLevel->Render();	
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurrentLevel);
}



