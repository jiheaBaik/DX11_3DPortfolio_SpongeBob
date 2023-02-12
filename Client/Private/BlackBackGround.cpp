#include "stdafx.h"
#include "..\Public\BlackBackGround.h"


#include "GameInstance.h"

CBlackBackGround::CBlackBackGround(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CBlackBackGround::CBlackBackGround(const CBlackBackGround & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CBlackBackGround::NativeConstruct_Prototype()
{
	

	return S_OK;
}

HRESULT CBlackBackGround::NativeConstruct(void * pArg)
{

	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (static_cast<_bool*>(pArg) != nullptr)
		m_Info = *(_bool*)pArg;



	if (m_Info == true)
		m_fY = 0 - 310;
	else
		m_fY = 720 + 310;

	m_fX = 640;
	m_fSizeX = 1280;
	m_fSizeY = 720;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	

	return S_OK;

}

void CBlackBackGround::Tick(_double TimeDelta)
{
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.0f, 1.f));

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_fDistance = 1.f;
	//dista = 0;
	if (GetAsyncKeyState('J') & 0x0001)
	{
		m_IsRender = true;
	}


	

	if (m_IsRender)
	{
		if (m_Info == true)
		{
			if (m_fY <= 155.f)
			{
				m_iCount += 0.003;
				m_fY += m_iCount;

			}


		}
		else
		{

			if (m_fY >= 570.f)
			{
				m_iCount += 0.003;
				m_fY -= m_iCount;
			}


		}

	//	pGameInstance->BGMVolumeDown(m_iCount);
		/*if (!m_isFirst)
		{
			for (int i = 0; i < 60; i++)
			{
				if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_BubbleUI", TEXT("Prototype_GameObject_BubbleUI"))));
			}

			m_isFirst = true;
		}*/
		
	}



	Safe_Release(pGameInstance);
	
}

void CBlackBackGround::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;
	if (m_IsRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

}

HRESULT CBlackBackGround::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CBlackBackGround::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Black"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBlackBackGround::SetUp_SRV()
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

	if (FAILED(m_pTextureCom->SetUp_SRV(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CBlackBackGround * CBlackBackGround::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBlackBackGround*		pInstance = new CBlackBackGround(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CBlackBackGround");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBlackBackGround::Clone(void * pArg)
{
	CBlackBackGround*		pInstance = new CBlackBackGround(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBlackBackGround");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBlackBackGround::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
