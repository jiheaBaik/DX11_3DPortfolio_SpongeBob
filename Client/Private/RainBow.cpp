#include "stdafx.h"
#include "..\Public\RainBow.h"



#include "GameInstance.h"

CRainBow::CRainBow(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CRainBow::CRainBow(const CRainBow & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CRainBow::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CRainBow::NativeConstruct(void * pArg)
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

void CRainBow::Tick(_double TimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	


	


		CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
		_vector PlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);


		m_pTransformCom->Set_State(CTransform::STATE_POSITION, PlayerPos +XMVectorSet(0, 1 ,0,1));


	//m_pColliderCom[TYPE_OBB]->Update(m_pTransformCom->Get_WorldMatrix());
	Safe_Release(pGameInstance);
}

void CRainBow::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_bisRender)
	{
		Angle = -0.1f;
		addAngle += -0.1f;
		m_iCount++;
		m_pTransformCom->Rotation(XMLoadFloat3(&_float3(0, 1, 0)), Angle);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);

		if (m_iCount > 25)
			m_bisRender = false;

	}
	else
	{
		m_iCount = 0;
		m_pTransformCom->Rotation(XMLoadFloat3(&_float3(0, 1, 0)), addAngle  * -1);
		addAngle = 0.f;

		
	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CRainBow::Render()
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

		m_pVIBufferCom->Render(i, m_pShaderCom, 9);
	}

//#ifdef _DEBUG
//
//	m_pRendererCom->Add_DebugGroup(m_pColliderCom[TYPE_OBB]);
//
//#endif // _DEBUG

	return S_OK;
}

HRESULT CRainBow::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_Model_RainBow"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	///* For.Com_Texture */
	//if (FAILED(__super::Add_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_GateTest"), (CComponent**)&m_pTextureCom)))
	//	return E_FAIL;


	///* For.Com_Collider_AABB */
	//CCollider::COLLIDERDESC		ColliderDesc;
	///*ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	//ColliderDesc.vScale = _float3(1.f, 2.f, 1.f);
	//ColliderDesc.vTranslation = _float3(0.0f, 1.0f, 0.0f);

	//if (FAILED(__super::Add_Component(TEXT("Com_Collider_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pColliderCom[TYPE_AABB], &ColliderDesc)))
	//	return E_FAIL;*/

	///* For.Com_Collider_OBB */
	//ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	//ColliderDesc.vScale = _float3(5.2f, 1.f, 1.2f);
	//ColliderDesc.vTranslation = _float3(0.0f, 1.f, 0.0f);

	//if (FAILED(__super::Add_Component(TEXT("Com_Collider_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pColliderCom[TYPE_OBB], &ColliderDesc)))
	//	return E_FAIL;

	///* For.Com_Collider_SPHERE */
	///*ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	//ColliderDesc.vScale = _float3(2.0f, 2.f, 2.f);
	//ColliderDesc.vTranslation = _float3(0.0f, 1.0f, 0.0f);

	//if (FAILED(__super::Add_Component(TEXT("Com_Collider_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pColliderCom[TYPE_SPHERE], &ColliderDesc)))
	//	return E_FAIL;*/



	//if (m_Info.fScale.x == 0.f || m_Info.fScale.x == NULL)
	//	m_Info.fScale = _float3(1.0f, 1.0f, 1.0f);
	//m_pTransformCom->Set_Scaled(_float3(m_Info.fScale.x, m_Info.fScale.y, m_Info.fScale.z));
	//if (m_Info.fPos.x == 0.f || m_Info.fScale.x == NULL)
	//	m_Info.fPos = _float3(0.f, 0.f, 0.f);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_Info.fPos.x, m_Info.fPos.y, m_Info.fPos.z, 1));
	//if (m_Info.fRotation != 0.f || m_Info.fRotation != NULL)
	//	m_pTransformCom->Rotation(XMLoadFloat3(&m_Info.fAxist), m_Info.fRotation);
	m_pTransformCom->Set_Scaled(_float3(7,7,7));
	return S_OK;
}

HRESULT CRainBow::SetUp_ConstantTable()
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

	/*if (FAILED(m_pTextureCom->SetUp_SRV(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;*/


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CRainBow * CRainBow::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRainBow*		pInstance = new CRainBow(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CGate");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRainBow::Clone(void * pArg)
{
	CRainBow*		pInstance = new CRainBow(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGate");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRainBow::Free()
{
	__super::Free();
	
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
