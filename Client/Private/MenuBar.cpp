#include "stdafx.h"
#include "..\Public\MenuBar.h"

#include <iostream>
#include "GameInstance.h"

CMenuBar::CMenuBar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CMenuBar::CMenuBar(const CMenuBar & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CMenuBar::NativeConstruct_Prototype()
{


	return S_OK;
}

HRESULT CMenuBar::NativeConstruct(void * pArg)
{

	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (static_cast<OBJINFODESC*>(pArg) != nullptr)
		m_Info = *(OBJINFODESC*)pArg;

	m_fX = m_Info.fPos.x;
	m_fY = m_Info.fPos.y;
	m_fSizeX = 500;
	m_fSizeY = g_iWinSizeY;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	m_vMenuBar = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	return S_OK;

}

void CMenuBar::Tick(_double TimeDelta)
{

	_vector a = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_matrix b = m_pTransformCom->Get_WorldMatrix();

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	static _bool bIsLeftMove_End = false;
	if (bIsLeftMove_End)
	{
		if (m_bMove)
		{
			_float fStomps = 2;
			m_iCount++;
			/*if (m_iCount % 3 == 0)
				m_fX *= -1;*/

			_float fY = 70 * TimeDelta * fStomps * 0.17;
			m_vMenuBar = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vMenuBar + XMVectorSet(fY, 0, 0.f, 1.f));
			//cout << "###fY : " << fY << endl;
			if (m_iCount == 50)
			{

			//	cout << "______End : " << fY << endl;
				m_vMenuBar = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				m_bMove = false;
				m_iCount = 0;
			}
		}
		else
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_iMove - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.0f, 1.f));
	}

	

	if (!bIsLeftMove_End)
	{
		if (m_fX >= m_iMove)
		{

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_iMove - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.0f, 1.f));
			m_iMove += 15;
		}
		else
		{
			m_vMenuBar = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			bIsLeftMove_End = true;
			m_bMove = true;
		}
	}
	

	
	

	

	Safe_Release(pGameInstance);

}

void CMenuBar::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	//cout << "ptCursor.x : "<<ptCursor.x << endl;
	//	cout << "ptCursor.y : " << ptCursor.y << endl;

	static _bool bIsSoundFlag = true;
	static _bool bIsSoundFlag2 = true;
	static _bool bIsEnableFlag = false;
	bIsEnableFlag = false;

	//game_start
	if (ptCursor.x >= 827 - 300 * 0.5f && ptCursor.x <= 827 + 300 * 0.5f &&
		ptCursor.y <= 249 + 60 * 0.5f && ptCursor.y >= 249 - 60 * 0.5f)
	{
		bIsEnableFlag = true;
		m_iTextureNum = 1;
		if (bIsSoundFlag)
		{
			CGameInstance::Get_Instance()->StopSound(CSoundMgr::UI);
			CGameInstance::Get_Instance()->PlayMySound(L"SFX_UI_Wrong_Selection_nobuzz.ogg", CSoundMgr::UI, 0.5f);
		}
		bIsSoundFlag = false;
		

		if (GetKeyState(VK_LBUTTON) < 0)
		{
			CGameInstance::Get_Instance()->StopSound(CSoundMgr::UI);
			CGameInstance::Get_Instance()->PlayMySound(L"SFX_UI_Forward.ogg", CSoundMgr::UI, 0.5f);

			m_bIsClick = true;
		}
	}
	else
		bIsSoundFlag = true;
	
	//game_exit
	 if (ptCursor.x >= 829 - 300 * 0.5f && ptCursor.x <= 829 + 300 * 0.5f &&
		ptCursor.y <= 369 + 60 * 0.5f && ptCursor.y >= 369 - 60 * 0.5f)
	{
		m_iTextureNum = 2;
		bIsEnableFlag = true;
		if (bIsSoundFlag2)
		{
			CGameInstance::Get_Instance()->StopSound(CSoundMgr::UI);
			CGameInstance::Get_Instance()->PlayMySound(L"SFX_UI_Wrong_Selection_nobuzz.ogg", CSoundMgr::UI, 0.5f);
		}
		bIsSoundFlag2 = false;
		if (GetKeyState(VK_LBUTTON) < 0)
		{
			CGameInstance::Get_Instance()->StopSound(CSoundMgr::UI);
			CGameInstance::Get_Instance()->PlayMySound(L"SFX_UI_Forward.ogg", CSoundMgr::UI, 0.5f);

		}
	}
	 else
		 bIsSoundFlag2 = true;

	 if (!bIsEnableFlag)
		 m_iTextureNum = 0;

	//game_start
	//if (ptCursor.x >= 920 - 300 * 0.5f && ptCursor.x <= 920 + 300 * 0.5f &&
	//	ptCursor.y <= 350 + 60 * 0.5f && ptCursor.y >= 350 - 60 * 0.5f)
	//{
	//	m_iTextureNum = 1;
	//	if (GetKeyState(VK_LBUTTON) < 0)
	//	{
	//		m_bIsClick = true;
	//	}
	//}//game_exit
	//else if (ptCursor.x >= 920 - 300 * 0.5f && ptCursor.x <= 920 + 300 * 0.5f &&
	//	ptCursor.y <= 490 + 60 * 0.5f && ptCursor.y >= 490 - 60 * 0.5f)
	//{
	//	m_iTextureNum = 2;
	//	if (GetKeyState(VK_LBUTTON) < 0)
	//	{

	//	}
	//}
	//else
	//	m_iTextureNum = 0;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CMenuBar::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CMenuBar::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_MENU, TEXT("Prototype_Component_Texture_MenuBar"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMenuBar::SetUp_SRV()
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

CMenuBar * CMenuBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMenuBar*		pInstance = new CMenuBar(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CUnderpants");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMenuBar::Clone(void * pArg)
{
	CMenuBar*		pInstance = new CMenuBar(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUnderpants");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMenuBar::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
