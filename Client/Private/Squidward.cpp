#include "stdafx.h"
#include "..\Public\Squidward.h"


#include "GameInstance.h"
#include "Jelly_Pink.h"

#include "Camera_Free.h"
#include "JellyCut.h"



CSquidward::CSquidward(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CSquidward::CSquidward(const CSquidward & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CSquidward::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CSquidward::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc(1.f, XMConvertToRadians(90.0f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (static_cast<OBJINFODESC*>(pArg) != nullptr)
		m_Info = *(OBJINFODESC*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_pVIBufferCom->Set_CurrentAnimIndex(1);

	CGameInstance::Get_Instance()->PlayMySound(L"SQBC007_b.ogg", CSoundMgr::NPC, 0.1f);


	return S_OK;
}

void CSquidward::Tick(_double TimeDelta)
{
	static _float slow = 1;
	m_pVIBufferCom->Update_Animation(TimeDelta * slow);
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


		CCollider*	pCSquidwardCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_FIELD, TEXT("Layer_Jelly_Pink"), TEXT("Com_Collider_OBB"),2);
		if (nullptr == pCSquidwardCollider)
			return;
		static _uint count = 0;
		count++;
		if (count > 250)
		{
			m_bIsDisolve = true;

		CGameInstance::Get_Instance()->StopSound(CSoundMgr::NPC);


		}
	

		if (m_pColliderCom[TYPE_OBB]->Collision(pCSquidwardCollider))
		{
			
		//	m_bIsDisolve = true;

		}

	


	
		CCamera_Free* pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Camera")));

	if (m_bIsDisolve)
	{
		slow = 0;
		m_distroyValue += 0.001f;
		m_Dvalue += (0.001f + m_distroyValue)* TimeDelta;
		//m_pVIBufferCom->Set_CurrentAnimIndex(0);

		if (m_Dvalue <= 0.003f)
		{
			pCamera->Set_Shake(true);
		}
		else
		{
			pCamera->Set_Shake(false);
		}
		//if (m_Dvalue >= 0.01f)
		//{
		////	CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		////	CGameInstance::Get_Instance()->PlayMySound(L"SBG01066_c.ogg", CSoundMgr::PLAYER, 0.1f);
		////	CGameInstance::Get_Instance()->PlayMySound(L"SBG01066_c.ogg", CSoundMgr::PLAYER, 0.1f);

		//}
		if (m_Dvalue >= 0.1f)
		{
			m_bIsDisolveEnd = true;
			pCamera->Set_Shake(false);
			isDead = true;

			CJellyCut* pJellyCut = static_cast<CJellyCut*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_JellyCut")));
			pJellyCut->Set_TexutureNum(1);
			pJellyCut->Set_AlPha();
		//	CGameInstance::Get_Instance()->PlayBGM(L"MUS_JellyfishFieldsTheme.ogg", 0.1f);

			
		}


	}
	else
	{
		_vector		vpos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector		vLook = m_pTransformCom->Get_State(CTransform::STATE_RIGHT) * -1;

		vpos += XMVector3Normalize(vLook) * _float(5 * TimeDelta);


		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vpos);
		m_pColliderCom[TYPE_OBB]->Update(m_pTransformCom->Get_WorldMatrix());
	}
	//_vector		vLook = m_pTransformCom->Get_State(CTransform::STATE_RIGHT) /** -1*/;
	//vLook = XMVector3Normalize(vLook);
	//_float3 fLook;
	//XMStoreFloat3(&fLook, vLook);

	//_vector		vpos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_float3 fpos;
	//XMStoreFloat3(&fpos, vpos);


	//fdistance = 0.01f;

	//vpos = XMVectorSet(fpos.x + (fLook.x * fdistance *TimeDelta), fpos.y + (fLook.y * fdistance*TimeDelta), fpos.z + fLook.z + (fLook.z * fdistance*TimeDelta), 1);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vpos);
	
	//Set_State(CTransform::STATE_POSITION, vpos);

	Safe_Release(pGameInstance);
	//m_pTransformCom->Go_Straight(TimeDelta);
}

void CSquidward::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CSquidward::Render()
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
		if (!m_bIsDisolve)
			m_pVIBufferCom->Render(i, m_pShaderCom, 0);
		else
			m_pVIBufferCom->Render(i, m_pShaderCom, 5);
	}


#ifdef _DEBUG

	m_pRendererCom->Add_DebugGroup(m_pColliderCom[TYPE_OBB]);

#endif // _DEBUG


	return S_OK;
}

void CSquidward::Set_Dead()
{
	m_bIsDisolve = true;


}

HRESULT CSquidward::SetUp_Components()
{
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_FIELD, TEXT("Prototype_Component_Model_Squidward"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Disolve"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Collider_AABB */
	CCollider::COLLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 2.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.0f, 1.0f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pColliderCom[TYPE_AABB], &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Collider_OBB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.2f, 1.f, 1.2f);
	ColliderDesc.vTranslation = _float3(0.0f, 1.f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pColliderCom[TYPE_OBB], &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Collider_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(2.0f, 2.f, 2.f);
	ColliderDesc.vTranslation = _float3(0.0f, 1.0f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pColliderCom[TYPE_SPHERE], &ColliderDesc)))
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

HRESULT CSquidward::SetUp_ConstantTable()
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

	if (FAILED(m_pTextureCom->SetUp_SRV(m_pShaderCom, "g_DiffuseTexture2", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_Dvalue", &m_Dvalue, sizeof(_float))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

CSquidward * CSquidward::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSquidward*		pInstance = new CSquidward(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CRobo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSquidward::Clone(void * pArg)
{
	CSquidward*		pInstance = new CSquidward(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CRobo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSquidward::Free()
{
	__super::Free();
	for (auto& pColliderCom : m_pColliderCom)
		Safe_Release(pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
