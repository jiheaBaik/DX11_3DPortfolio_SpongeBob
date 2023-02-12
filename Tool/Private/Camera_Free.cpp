#include "..\Public\Camera_Free.h"
#include "GameInstance.h"

CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{

}

CCamera_Free::CCamera_Free(const CCamera_Free & Prototype)
	: CCamera(Prototype)
{
}

HRESULT CCamera_Free::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Free::Tick(_double TimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
		m_pTransformCom->Go_Straight(TimeDelta);
	if (pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
		m_pTransformCom->Go_Left(TimeDelta);
	if (pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
		m_pTransformCom->Go_Right(TimeDelta);
	if (pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
		m_pTransformCom->Go_Backward(TimeDelta);

	_long		MouseMove = 0;
	if (GetKeyState(VK_MBUTTON) < 0)
	{
		if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * MouseMove * 0.1f);

		}

		if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), TimeDelta * MouseMove * 0.1f);
		}
	}

	__super::Tick(TimeDelta);

	Safe_Release(pGameInstance);
}

void CCamera_Free::LateTick(_double TimeDelta)
{
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

CCamera_Free * CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Free*		pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Free");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCamera_Free::Clone(void * pArg)
{
	CCamera_Free*		pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Created : CCamera_Free");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();
}
