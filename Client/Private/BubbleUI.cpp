#include "stdafx.h"
#include "..\Public\BubbleUI.h"

#include <iostream>
#include "GameInstance.h"

CBubbleUI::CBubbleUI(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CBubbleUI::CBubbleUI(const CBubbleUI & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CBubbleUI::NativeConstruct_Prototype()
{
	

	return S_OK;
}

HRESULT CBubbleUI::NativeConstruct(void * pArg)
{

	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	random = rand() % 1270;

	m_fX = random;
	random = rand() % 300 + 700;
	m_fY = random;

	random = rand() % 50 + 80;
	m_fSizeX = random;
	m_fSizeY = random;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	random2 = (rand() % 50 + 5) * 0.1f;
	

	return S_OK;

}

void CBubbleUI::Tick(_double TimeDelta)
{
	

	m_fAddY += random2;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, (-m_fY + g_iWinSizeY * 0.5f) + m_fAddY, 0.0f, 1.f));

	//CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	//Safe_AddRef(pGameInstance);

	

	//if (GetAsyncKeyState('H') & 0x0001)
	//{
	//	m_IsSandyHit = true;
	//}


	//if (/*Move &&*/m_IsSandyHit)
	//{
	//	if (m_dTotalTime <= 1.2f && !m_isUnder)
	//	{
	//		m_fX -= 1.;
	//		m_dTotalTime += 0.01;
	//	}
	//	else
	//	{
	//		flag = true;
	//		m_isUnder = true;
	//		m_dTotalTime -= 0.01;
	//		m_fX += 1.f;
	//		if (m_dTotalTime <= -1.f)
	//		{
	//			m_dTotalTime = 0.f;
	//			m_isUnder = false;
	//			m_IsSandyHit = false;

	//			m_fX = 1280;
	//			m_fY = 500;
	//		}
	//	}
	//}


	//if (m_iCount >= 10)
	//	m_iTextureNum = 0;
	//else
	//	m_iTextureNum = 1;


	//if (m_iCount >= 20)
	//{
	//	m_iCount = 0;

	//}
	//
	//m_iCount++;

	//Safe_Release(pGameInstance);
	
}

void CBubbleUI::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	
	
	
	////cout << "rand : " << random << endl;
	//if (m_IsSandyHit)
	//{
	//	if (!flag)
	//	{
	//		switch (random)
	//		{
	//		case 0:
	//			CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_CrowdCheer_002.ogg", CSoundMgr::EFFECT, 0.5f);
	//			CGameInstance::Get_Instance()->PlayMySound(L"FAB1013.ogg", CSoundMgr::NPC, 0.5f);

	//			break;
	//		case 1:
	//			CGameInstance::Get_Instance()->PlayMySound(L"SFX_Amb_Poseidome_Crowd.ogg", CSoundMgr::EFFECT, 0.5f);
	//			CGameInstance::Get_Instance()->PlayMySound(L"FAB1046.ogg", CSoundMgr::NPC, 0.5f);

	//			break;
	//		case 2:
	//			CGameInstance::Get_Instance()->PlayMySound(L"SFX_RoboSandy_CrowdCheer_001.ogg", CSoundMgr::EFFECT, 0.5f);
	//			CGameInstance::Get_Instance()->PlayMySound(L"FAB1029.ogg", CSoundMgr::NPC, 0.5f);

	//			break;
	//		default:
	//			CGameInstance::Get_Instance()->PlayMySound(L"SFX_Amb_Poseidome_Crowd.ogg", CSoundMgr::EFFECT, 0.5f);
	//			CGameInstance::Get_Instance()->PlayMySound(L"FAB1026.ogg", CSoundMgr::NPC, 0.5f);

	//			break;
	//		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	/*}
	else
		flag = false;*/
}

HRESULT CBubbleUI::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CBubbleUI::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_BubbleUI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBubbleUI::SetUp_SRV()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_float4x4		ViewMatrix;

	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureNum)))
		return E_FAIL;

	return S_OK;
}

CBubbleUI * CBubbleUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBubbleUI*		pInstance = new CBubbleUI(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CBubbleUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBubbleUI::Clone(void * pArg)
{
	CBubbleUI*		pInstance = new CBubbleUI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBubbleUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBubbleUI::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
