#include "..\Public\GameObject.h"
#include "GameInstance.h"

const _tchar* CGameObject::m_pTransformTag = TEXT("Com_Transform");

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CGameObject::CGameObject(const CGameObject & Prototype)
	: m_pDevice(Prototype.m_pDevice)
	, m_pContext(Prototype.m_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag)
{
	return Find_Component(pComponentTag);	
}

HRESULT CGameObject::NativeConstruct_Prototype()
{


	return S_OK;
}

HRESULT CGameObject::NativeConstruct(const CTransform::TRANSFORMDESC* pTransformDesc)
{
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_Components.emplace(m_pTransformTag, m_pTransformCom);

	Safe_AddRef(m_pTransformCom);
	
	if(nullptr != pTransformDesc)
		m_pTransformCom->Set_TransformDesc(*pTransformDesc);	

	return S_OK;
}

void CGameObject::Tick(_double TimeDelta)
{


}

void CGameObject::LateTick(_double TimeDelta)
{

}

HRESULT CGameObject::Render()
{

	return S_OK;
}

HRESULT CGameObject::Add_Component(const _tchar * pComponentTag, _uint iLevelIndex, const _tchar * pPrototypeTag, CComponent** ppOut, void * pArg)
{
	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CComponent*		pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(pComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	Safe_Release(pGameInstance);
	return S_OK;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pTransformCom);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
