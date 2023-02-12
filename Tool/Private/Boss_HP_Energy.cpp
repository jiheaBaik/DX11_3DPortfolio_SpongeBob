#include "..\Public\Boss_HP_Energy.h"


#include "GameInstance.h"

CBoss_HP_Energy::CBoss_HP_Energy(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CBoss_HP_Energy::CBoss_HP_Energy(const CBoss_HP_Energy & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CBoss_HP_Energy::NativeConstruct_Prototype()
{


	return S_OK;
}

HRESULT CBoss_HP_Energy::NativeConstruct(void * pArg)
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
	m_fSizeX = 10;
	m_fSizeY = 30;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;

}

void CBoss_HP_Energy::Tick(_double TimeDelta)
{
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.0f, 1.f));
	if (GetKeyState(VK_RBUTTON) <0)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		POINT ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(g_hWnd, &ptCursor);

		if (ptCursor.x >= m_fX - m_fSizeX * 0.5f && ptCursor.x <= m_fX + m_fSizeX * 0.5f &&
			ptCursor.y <= m_fY + m_fSizeY * 0.5f && ptCursor.y >= m_fY - m_fSizeY * 0.5f)
		{
			int rc;
			char *err_msg = 0;
			sqlite3_stmt *res2 = nullptr;


			string sqlTemp = "Delete from " + m_Info.m_Table + "  where Pos_x   =  @m_fX";

			const char* sql2 = sqlTemp.c_str();

			rc = sqlite3_prepare_v2(m_Info.db, sql2, -1, &res2, NULL);
			int index = sqlite3_bind_parameter_index(res2, "@m_fX");
			sqlite3_bind_int(res2, index, m_fX);
			int step = sqlite3_step(res2);

			isDead = true;

		}

		Safe_Release(pGameInstance);
	}
	
	
}

void CBoss_HP_Energy::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CBoss_HP_Energy::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CBoss_HP_Energy::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Energy"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_HP_Energy::SetUp_SRV()
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

	if (FAILED(m_pTextureCom->SetUp_SRV(m_pShaderCom, "g_DiffuseTexture", m_Info.pTextureInfo)))
		return E_FAIL;

	return S_OK;
}

CBoss_HP_Energy * CBoss_HP_Energy::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBoss_HP_Energy*		pInstance = new CBoss_HP_Energy(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CUnderpants");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoss_HP_Energy::Clone(void * pArg)
{
	CBoss_HP_Energy*		pInstance = new CBoss_HP_Energy(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUnderpants");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_HP_Energy::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
