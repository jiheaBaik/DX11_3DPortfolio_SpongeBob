#include "stdafx.h"
#include "..\Public\Scoreboard.h"

#include <iostream>

#include "GameInstance.h"

CScoreboard::CScoreboard(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CScoreboard::CScoreboard(const CScoreboard & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CScoreboard::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CScoreboard::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (static_cast<OBJINFODESC*>(pArg) != nullptr)
		m_Info = *(OBJINFODESC*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void CScoreboard::Tick(_double TimeDelta)
{
}

void CScoreboard::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;
	//CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	//if (true == pGameInstance->isIn_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 2.f))
	//{
	//	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	//}
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	//RELEASE_INSTANCE(CGameInstance);
}

HRESULT CScoreboard::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pVIBufferCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		//if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		//	return E_FAIL;
		//if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		//	return E_FAIL;

		//	m_pShaderCom->Begin(0);

		m_pVIBufferCom->Render(i, m_pShaderCom, 0);
	}



	return S_OK;
}


void CScoreboard::Move(_double TimeDelta)
{

	m_iCount++;
	if (m_iCount < 50)
		Turn_R();
	else if (m_iCount > 50 && m_iCount < 150)
	{
		Turn_L();
		
	}
	else
		m_IsShakeEnd = true;
	//else if(m_iCount > 150 && m_iCount < 200)

	if (!m_IsFall)
	{

		_vector m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 fPos;
		XMStoreFloat3(&fPos, m_vPos);



		if (m_IsShakeEnd)
		{
			if (fPos.y <= 0)
			{
				m_IsFall = true;
				//m_pTransformCom->Rotation(XMLoadFloat3(&_float3(0.f, 1.f, 0.f)), 1);
			}
			else
			{
				//m_pTransformCom->Chase(XMVectorSet(44,0.f,45, 1), TimeDelta *0.01f, 2);
				//반올림때문에 격차가 크게 발생한다
				_vector		vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				//https://kldp.org/node/40084 참고 하기 따라서 밑에처럼 하면 안됨
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + XMVectorSet(0, -0.05f + m_iadd, 0, 1));
		

		_float3 fPlayerLook;
		XMStoreFloat3(&fPlayerLook, vPos);

		//cout << "m_vPos : " << fPlayerLook.y << endl;
				//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos + XMVectorSet(0, -0.01f , 0, 1));
				//
				m_iadd -= 0.005f;
			}
		}


	}


}
void CScoreboard::Turn_R()
{

	m_pTransformCom->Rotation(XMLoadFloat3(&_float3(0.f, 0.f, 1.f)), 0.005f);



}
void CScoreboard::Turn_L()
{
	m_pTransformCom->Rotation(XMLoadFloat3(&_float3(0.f, 0.f, 1.f)), -0.005f);
}




HRESULT CScoreboard::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Scoreboard"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (m_Info.fScale.x == 0.f || m_Info.fScale.x == NULL)
		m_Info.fScale = _float3(1.0f, 1.0f, 1.0f);
	m_pTransformCom->Set_Scaled(_float3(m_Info.fScale.x, m_Info.fScale.y, m_Info.fScale.z));
	if (m_Info.fPos.x == 0.f || m_Info.fScale.x == NULL)
		m_Info.fPos = _float3(0.f, 0.f, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_Info.fPos.x, m_Info.fPos.y, m_Info.fPos.z, 1));
	if (m_Info.fRotation != 0.f || m_Info.fRotation != NULL)
		m_pTransformCom->Rotation(XMLoadFloat3(&m_Info.fAxist), m_Info.fRotation);
	return S_OK;
}

HRESULT CScoreboard::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_Transformfloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_Transformfloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CScoreboard * CScoreboard::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CScoreboard*		pInstance = new CScoreboard(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CScoreboard");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CScoreboard::Clone(void * pArg)
{
	CScoreboard*		pInstance = new CScoreboard(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CScoreboard");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CScoreboard::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
