#include "stdafx.h"
#include "..\Public\Boss_Sandy.h"
#include "GameInstance.h"

#include <iostream>



CBoss_Sandy::CBoss_Sandy(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
	

}

CBoss_Sandy::CBoss_Sandy(const CBoss_Sandy & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CBoss_Sandy::NativeConstruct_Prototype()
{

	
	return S_OK;
}

//unsigned int APIENTRY ThreadEntryFunc(void* pArg)
//{
//
//
//	CBoss_Sandy*		Boss_Sandy = (CBoss_Sandy*)pArg;
//
//	EnterCriticalSection(Boss_Sandy->Get_CriticalSection());
//
//	/* 자원로드할꺼야. */
//	/*switch (pLoader->Get_NextLevelID())
//	{
//	case LEVEL_LOGO:
//		pLoader->Loading_ForLogoLevel();
//		break;
//	case LEVEL_GAMEPLAY:
//		pLoader->Loading_ForGamePlayLevel();
//		break;
//	}
//*/
//	LeaveCriticalSection(Boss_Sandy->Get_CriticalSection());
//
//	return 0;
//}


HRESULT CBoss_Sandy::NativeConstruct(void * pArg)
{
	//InitializeCriticalSection(&m_CriticalSection);

	/*m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;*/

	pBoss_Sandy_Body = CBoss_Sandy_Body::Create(m_pDevice, m_pContext);
	pBoss_Sandy_Head = CBoss_Sandy_Head::Create(m_pDevice, m_pContext);
	

	/* For.Prototype_Component_Model_Boss_Sandy_Body*/
	_matrix PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Sandy_Body"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Boss_Sandy/", "Boss_Sandy_Body.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject Boss_Sandy_Body*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Sandy_Body"),
		pBoss_Sandy_Body)))
		return E_FAIL;

	/* For.Prototype_Component_Model_Boss_Sandy_Head*/
	 PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Sandy_Head"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Boss_Sandy/", "Boss_Sandy_Head.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject Boss_Sandy_Head*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Sandy_Head"),
		pBoss_Sandy_Head)))
		return E_FAIL;

	pBoss_Sandy_Body->NativeConstruct(pArg);
	pBoss_Sandy_Head->NativeConstruct(pArg);

	return S_OK;
}

void CBoss_Sandy::Tick(_double TimeDelta)
{
	//static _bool Isfirst = true;

	//if (Isfirst)
	//{
	//	Isfirst = false;
	//	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc, &TimeDelta, 0, nullptr);
	//	/*if (0 == m_hThread)
	//		return E_FAIL;*/
	//}
	//_static
	
	pBoss_Sandy_Body->Tick(TimeDelta);
	pBoss_Sandy_Head->Tick(TimeDelta);

}

void CBoss_Sandy::LateTick(_double TimeDelta)
{
	//if (nullptr == m_pRendererCom)
	//	return;

	//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	pBoss_Sandy_Body->LateTick(TimeDelta);
	pBoss_Sandy_Head->LateTick(TimeDelta);
}

HRESULT CBoss_Sandy::Render()
{
	pBoss_Sandy_Body->Render();
	pBoss_Sandy_Head->Render();
	return S_OK;
}

HRESULT CBoss_Sandy::SetUp_Components()
{
	/*if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;*/
	pBoss_Sandy_Body->SetUp_Components();
	pBoss_Sandy_Head->SetUp_Components();

	return S_OK;
}

HRESULT CBoss_Sandy::SetUp_SRV()
{
	pBoss_Sandy_Body->SetUp_SRV();
	pBoss_Sandy_Head->SetUp_SRV();

	return S_OK;
}

CBoss_Sandy * CBoss_Sandy::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBoss_Sandy*		pInstance = new CBoss_Sandy(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CForkLift");
		Safe_Release(pInstance);
	}

	

	return pInstance;
}

CGameObject * CBoss_Sandy::Clone(void * pArg)
{
	CBoss_Sandy*		pInstance = new CBoss_Sandy(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CForkLift");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_Sandy::Free()
{
	__super::Free();

	//	Safe_Release(m_pColliderCom);
	Safe_Release(m_pRendererCom);
	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pVIBufferCom);
	Safe_Release(pBoss_Sandy_Head);
	Safe_Release(pBoss_Sandy_Head);
//	DeleteCriticalSection(&m_CriticalSection);
}
