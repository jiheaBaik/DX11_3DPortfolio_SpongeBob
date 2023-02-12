#include "stdafx.h"
#include "..\Public\Robo_2.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Player.h"
#include "Flower.h"
#include "TwinkleEffect.h"


CRobo_2::CRobo_2(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CRobo_2::CRobo_2(const CRobo_2 & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CRobo_2::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CRobo_2::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC TransformDesc(5.f, XMConvertToRadians(90.f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;
	if (static_cast<OBJINFODESC*>(pArg) != nullptr)
		m_Info = *(OBJINFODESC*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pos);
	//m_pVIBufferCom->Set_CurrentAnimIndex(1);

//	m_pTransformCom->Rotation(XMLoadFloat3(&_float3(0.f, 1.f, 0.f)), -1.4f);


	return S_OK;
}

void CRobo_2::Tick(_double TimeDelta)
{
	if (m_motion != HIT)
	{
		if (m_pVIBufferCom->Get_NextFinish())
			m_iHitCount = 0;//한번만 피 깎이도록 

	}

	m_IsRed = false;
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();


	Safe_AddRef(pGameInstance);


	CCollider*	pPlayerCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Collider_OBB"));
	if (nullptr == pPlayerCollider)
		return;

	m_motion = IDLE;
	//	m_pVIBufferCom->Set_CurrentAnimIndex(0);
	//if (m_pVIBufferCom->Get_NextFinish())
	//	m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta, 0, false, 1);
	CCamera_Free* pCCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Camera")));
	if (pCCamera->Get_m_bIsDirector_End())
	{
		if (m_pColliderCom2[TYPE_SPHERE]->Collision(pPlayerCollider))
		{
			m_motion = CHASE;

			if (m_pColliderCom[TYPE_SPHERE]->Collision(pPlayerCollider))
			{
				CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_Player")));
				if (pPlayer->Get_IsAttack())
					m_motion = HIT;

			}

		}

		if (m_pColliderCom[TYPE_OBB]->Collision(pPlayerCollider))
		{
			CGameInstance::Get_Instance()->PlayMySound(L"SFX_Tubelet_Pop_003.ogg", CSoundMgr::MONSTER, 0.3f);

			CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_Player")));
			pPlayer->Set_IsHit_Field();

		}
	}
	//_float  m_Time = (rand() % 5  + 7)* 0.1;
	/*m_pVIBufferCom->Set_CurrentAnimIndex(1);
	m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelta , 1, false, 1);*/

	m_pColliderCom[TYPE_OBB]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[TYPE_SPHERE]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom2[TYPE_SPHERE]->Update(m_pTransformCom->Get_WorldMatrix());


	//if(m_firseMotion)
	SettingMotion(TimeDelta);

	

	

	Safe_Release(pGameInstance);
			//6,3
	//Safe_Release(pGameInstance);
}

void CRobo_2::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	
	if (0 == m_iHp)
		IsDie();
}

HRESULT CRobo_2::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_SRV()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pVIBufferCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pVIBufferCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (m_IsRed)
			m_pVIBufferCom->Render(i, m_pShaderCom, 2);
		else
			m_pVIBufferCom->Render(i, m_pShaderCom, 0);
	}

#ifdef _DEBUG

	m_pRendererCom->Add_DebugGroup(m_pColliderCom[TYPE_OBB]);
	m_pRendererCom->Add_DebugGroup(m_pColliderCom[TYPE_SPHERE]);
	m_pRendererCom->Add_DebugGroup(m_pColliderCom2[TYPE_SPHERE]);

	//m_pRendererCom->Add_DebugGroup(m_pNavigationCom);

#endif // _DEBUG

	return S_OK;
}

HRESULT CRobo_2::IsDie()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_vector pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector look = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector look2 = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	CFlower::FLOWERDESC		flowerDesc;
	ZeroMemory(&flowerDesc, sizeof(CFlower::FLOWERDESC));

	flowerDesc.pos = pos + XMVectorSet(0, 1, 0, 1);
	flowerDesc.vLook = look;
	flowerDesc.vColor = XMVectorSet(0.929, 0.368, 0.16, 1);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_Flower"), TEXT("Prototype_GameObject_Flower"), &flowerDesc)))
		return E_FAIL;

	flowerDesc.pos = pos + XMVectorSet(0, 1, 0, 1);
	flowerDesc.vLook = look * -1;
	//	flowerDesc.vColor = XMVectorSet(0.156, 0.639, 1, 1);
	flowerDesc.vColor = XMVectorSet(0.513, 0.2, 0.8223, 1);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_Flower"), TEXT("Prototype_GameObject_Flower"), &flowerDesc)))
		return E_FAIL;

	flowerDesc.pos = pos + XMVectorSet(0, 1, 0, 1);
	flowerDesc.vLook = look2;
	flowerDesc.vColor = XMVectorSet(1, 0.768, 0.074, 1);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_Flower"), TEXT("Prototype_GameObject_Flower"), &flowerDesc)))
		return E_FAIL;

	flowerDesc.pos = pos + XMVectorSet(0, 1, 0, 1);
	flowerDesc.vLook = look2 * -1;
	flowerDesc.vColor = XMVectorSet(0.929, 0.435, 0.435, 1);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_Flower"), TEXT("Prototype_GameObject_Flower"), &flowerDesc)))
		return E_FAIL;


	Safe_Release(pGameInstance);

	isDead = true;
	return S_OK;
}

HRESULT CRobo_2::SetUp_Components()
{
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_FIELD, TEXT("Prototype_Component_Model_Robo2"), (CComponent**)&m_pVIBufferCom)))
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
	ColliderDesc.vTranslation = _float3(0.0f, 0.5f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pColliderCom[TYPE_OBB], &ColliderDesc)))
		return E_FAIL;




	/* For.Com_Collider_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(4, 4, 4);
	ColliderDesc.vTranslation = _float3(0.0f, 0.5f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pColliderCom[TYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;
	
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(10, 10, 10);
	ColliderDesc.vTranslation = _float3(0.0f, 0.5f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_SPHERE_Chase"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pColliderCom2[TYPE_SPHERE], &ColliderDesc)))
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

HRESULT CRobo_2::SetUp_SRV()
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

void CRobo_2::SettingMotion(_double TimeDelt)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector PlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);


	_vector pos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
	_float3 fpos;
	XMStoreFloat3(&fpos, pos);
	_float m_floaGetY; 
	_vector look;
	CCamera_Free* pCamera = nullptr;
	CPlayer* pPlayer = nullptr;
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	//3번 춤
	//0번\아이들
	//114번 hit
	//1번 공격
	switch (m_motion)
	{
	case CHASE:
		m_pTransformCom->Chase(PlayerPos, TimeDelt * 0.7f, 1.f);

		m_pVIBufferCom->Set_CurrentAnimIndex(3);
		//if (m_pVIBufferCom->Get_NextFinish())
		m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelt * 0.7f, 3, false, 1);
		break;

	case IDLE:
		m_pVIBufferCom->Set_CurrentAnimIndex(2);

		m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelt, 2, false, 1);

		
	
		break;
	case HIT:
		m_pVIBufferCom->Set_CurrentAnimIndex(1);
		m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelt, 1, false, 1);
		CGameInstance::Get_Instance()->PlayMySound(L"SFX_Tubelet_Pop_002.ogg", CSoundMgr::MONSTER, 0.3f);

		pPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_Player")));
		m_pVIBufferCom->Update_Boss_Sandy_Animation(TimeDelt * pPlayer->Get_SlowMotion(), 1, false, 1);
	//	pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Camera")));
	//	pCamera->Set_Shake(true);

		m_IsRed = true;

		if (m_iColorChage % 8 == 0)
			m_IsRed = true;
		else
			m_IsRed = false;


		if (m_iColorChage == 16)
			m_iColorChage = 0;
		else
			m_iColorChage++;

		m_pTransformCom->Go_Backward(TimeDelt * 0.2);

		if (m_iHitCount == 0 /*&& pGameInstance->Get_Hp() != -1*/)
		{
			CTwinkleEffect::TWINKLEDESC		m_TwinkleDesc;
			ZeroMemory(&m_TwinkleDesc, sizeof(CTwinkleEffect::TWINKLEDESC));
			m_TwinkleDesc.pos = vPos;
			m_TwinkleDesc.vLook = vLook;
			if (FAILED(pGameInstance->Clone_GameObject(LEVEL_FIELD, TEXT("Layer_TwinkleEffect"), TEXT("Prototype_GameObject_Effect_TwinkleEffect"), &m_TwinkleDesc)));
			m_iHp -= 1;

		}


		m_iHitCount++;
		break;
	default:
		//	m_pVIBufferCom->Set_CurrentAnimIndex(0);
		
		break;
	}


	Safe_Release(pGameInstance);
}

CRobo_2 * CRobo_2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRobo_2*		pInstance = new CRobo_2(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CRobo2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRobo_2::Clone(void * pArg)
{
	CRobo_2*		pInstance = new CRobo_2(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CRobo2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRobo_2::Free()
{
	__super::Free();


	for (auto& pColliderCom : m_pColliderCom)
		Safe_Release(pColliderCom);
	for (auto& pColliderCom : m_pColliderCom2)
		Safe_Release(pColliderCom);
//	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
