#include "stdafx.h"
#include "..\Public\Player_Slame.h"
#include "GameInstance.h"
#include "Boss_Sandy_Body.h"
#include "Player.h"

CPlayer_Slame::CPlayer_Slame(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CPlayer_Slame::CPlayer_Slame(const CPlayer_Slame & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CPlayer_Slame::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Slame::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc(5.f, XMConvertToRadians(90.f));

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	/*if (static_cast<_uint*>(pArg) != nullptr)
		m_iTextureNum = *(_uint*)pArg;
	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;
	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;*/

	//XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 0.5f));
	m_pTransformCom->Rotation(XMLoadFloat3(&_float3(1.f, 0.f, 0.f)), XMConvertToRadians( 90.f));
	return S_OK;
}

void CPlayer_Slame::Tick(_double TimeDelta)
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//보스 정보 얻기
	CBoss_Sandy_Body* pBoss_Sandy_Body = static_cast<CBoss_Sandy_Body*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body")));
	CTransform* m_pSnadyTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body"), TEXT("Com_Transform"));
	
	_vector m_vSandyPos = m_pSnadyTransform->Get_State(CTransform::STATE_POSITION);
	_float3 SnadyPos;
	XMStoreFloat3(&SnadyPos, m_vSandyPos);


	//플레이어 정보 얻기 
	CPlayer* pPlayer = nullptr;
	CTransform* pPlayerTransform = nullptr;
	if (pGameInstance->Get_Player_Info() == 0)
	{
		pPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_Player")));
		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	}
	else
	{
		pPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer")));
		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer"), TEXT("Com_Transform"));
	}

	/*CPlayer* pPlayer = nullptr;
	pPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));*/


//	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector PlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 fplayerPos;
	XMStoreFloat3(&fplayerPos, PlayerPos);


	m_pTransformCom->Set_Scaled(_float3(3, 3, 1.f));
	//m_pTransformCom->LookAt(PlayerPos);


	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector		vSsndyLook = m_pSnadyTransform->Get_State(CTransform::STATE_LOOK);

	_vector vLookPosition = XMVector3Normalize(vSsndyLook);// *7;
	_float3 Pos;
	XMStoreFloat3(&Pos, vLookPosition);

	if (pBoss_Sandy_Body->Get_IsTouchHead())//보스 공격할수 있는 타이밍이라면 플레이어맞춰 이동
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fplayerPos.x, 0.3, fplayerPos.z, 1));
		m_pColliderCom[TYPE_OBB]->Update(m_pTransformCom->Get_WorldMatrix());

	}

	
	

	if (m_iTextureNum % 5 == 0)
		m_Isrend = true;
	else
	{
		m_Isrend = false;
		
	}
		
	if (m_iTextureNum == 10)
		m_iTextureNum = 0;
	else
		m_iTextureNum++;



	Safe_Release(pGameInstance);
}

void CPlayer_Slame::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CPlayer_Slame::Render()
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	
	//보스 정보 얻기
	CBoss_Sandy_Body* pBoss_Sandy_Body = static_cast<CBoss_Sandy_Body*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body")));


	//충돌체크
	CCollider*	pSlamCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Collider_OBB"));
	
	_bool IsCollision = false;

	if (m_pColliderCom[TYPE_OBB]->Collision_OBB(pSlamCollider))
		IsCollision = true;

	if (pBoss_Sandy_Body->Get_IsTouchHead() && IsCollision == false)//슬램프 공격이라면 렌더
	{
		if (nullptr == m_pVIBufferCom)
			return E_FAIL;

		if (FAILED(SetUp_SRV()))
			return E_FAIL;

		if(m_Isrend == true)
			m_pShaderCom->Begin(0);
		else
			m_pShaderCom->Begin(3);


		m_pVIBufferCom->Render();

#ifdef _DEBUG
		m_pColliderCom[TYPE_OBB]->Render();
#endif // _DEBUG
	}

	Safe_Release(pGameInstance);



	return S_OK;
}

HRESULT CPlayer_Slame::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Slame"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
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

	ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vTranslation = _float3(0.0f, 0.5f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pColliderCom[TYPE_OBB], &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Collider_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(2.0f, 2.f, 2.f);
	ColliderDesc.vTranslation = _float3(0.0f, 1.0f, 0.0f);

	if (FAILED(__super::Add_Component(TEXT("Com_Collider_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pColliderCom[TYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer_Slame::SetUp_SRV()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_float4x4		ViewMatrix;

	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	/*if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
	return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
	return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
	return E_FAIL;*/

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_Transformfloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_Transformfloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	if (FAILED(m_pTextureCom->SetUp_SRV(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CPlayer_Slame * CPlayer_Slame::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayer_Slame*		pInstance = new CPlayer_Slame(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CBackGround");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer_Slame::Clone(void * pArg)
{
	CPlayer_Slame*		pInstance = new CPlayer_Slame(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBackGround");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Slame::Free()
{
	__super::Free();
	for (auto& pColliderCom : m_pColliderCom)
		Safe_Release(pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}