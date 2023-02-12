#include "..\Public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pInput_Device(CInput_Device::Get_Instance())
	, m_pLevel_Manager(CLevel_Manager::Get_Instance())
	, m_pTimer_Manager(CTimer_Manager::Get_Instance())
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pPipeLine(CPipeLine::Get_Instance())
	, m_pLight_Manager(CLight_Manager::Get_Instance())
	, m_pPicking(CPicking::Get_Instance())
	, m_pFrustum(CFrustum::Get_Instance())
	, m_pPlayer_manager(CPlayer_Management::Get_Instance())
	, m_pHp_Management(CHp_Management::Get_Instance())
	, m_pFont_Manager(CFont_Manager::Get_Instance())
	, m_pDirector_Management(CDirector_Management::Get_Instance())
	, m_pTarget_Manager(CTarget_Manager::Get_Instance())
	, m_pSoundManager(CSoundMgr::Get_Instance())
{
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pFrustum);
	Safe_AddRef(m_pFont_Manager);
	Safe_AddRef(m_pPicking);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pPlayer_manager);
	Safe_AddRef(m_pHp_Management);
	Safe_AddRef(m_pDirector_Management);
	Safe_AddRef(m_pSoundManager);

}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, HINSTANCE hInst, const GRAPHICDESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 그래픽 디바이스 초기화. */
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWindowed, GraphicDesc.iWinCX, GraphicDesc.iWinCY, ppDeviceOut, ppContextOut)))
		return E_FAIL;


	/* 입력 디바이스 초기화. */
	if (FAILED(m_pInput_Device->Initialize_Input_Device(hInst, GraphicDesc.hWnd)))
		return E_FAIL;

	/* 사운드 디바이스 초기화. */
	if (FAILED(m_pSoundManager->Initialize()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Initialize(*ppDeviceOut, *ppContextOut)))
		return E_FAIL;


	if (FAILED(m_pFrustum->NativeConstruct()))
		return E_FAIL;

	/* 오브젝트 매니져 준비 */
	if (FAILED(m_pObject_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;



	/* 컴포넌트 매니져 준비 */
	if (FAILED(m_pComponent_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;
	if (FAILED(m_pPicking->Initialize(GraphicDesc.hWnd, ppDeviceOut, ppContextOut)))
		return E_FAIL;
	return S_OK;
}

void CGameInstance::Tick_Engine(_double TimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pObject_Manager ||
		nullptr == m_pInput_Device)
		return;



	/* 입력장치의 상태 조사. */
	m_pInput_Device->Update();


	/* 오브젝트들의 업데이트를 수행한다. */
	m_pObject_Manager->Tick(TimeDelta);

	m_pPipeLine->Tick();

	m_pLevel_Manager->Tick(TimeDelta);

	m_pFrustum->Transform_ToWorldSpace();

	m_pPicking->Compute_RayInWorldSpace();


	m_pObject_Manager->LateTick(TimeDelta);
	m_pLevel_Manager->LateTick(TimeDelta);
}

HRESULT CGameInstance::Render_Engine()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pLevel_Manager->Render();

	return S_OK;
}

_bool CGameInstance::Picking(CTransform * pTranform, CVIBuffer * pVIBuffer, _float3 * pOut)
{
	if (nullptr == m_pPicking)
		return false;

	return m_pPicking->Picking(pTranform, pVIBuffer, pOut);
}

_float CGameInstance::Get_Pos(class CTransform* pTransform, class CTransform* pPlayerTransform, class CVIBuffer* pVIBuffer)
{
	if (nullptr == m_pPicking)
		return false;

	return m_pPicking->Get_Pos(pTransform, pPlayerTransform, pVIBuffer);
}

int CGameInstance::VolumeUp(_uint eID, _float _vol)
{
	if (m_pSoundManager == nullptr)
		return -1;

	return m_pSoundManager->VolumeUp((CSoundMgr::CHANNELID)eID, _vol);
}

int CGameInstance::VolumeDown(_uint eID, _float _vol)
{
	if (m_pSoundManager == nullptr)
		return -1;

	return m_pSoundManager->VolumeDown((CSoundMgr::CHANNELID)eID, _vol);
}

int CGameInstance::BGMVolumeUp(_float _vol)
{
	if (m_pSoundManager == nullptr)
		return -1;

	return m_pSoundManager->BGMVolumeUp(_vol);
}

int CGameInstance::BGMVolumeDown(_float _vol)
{
	if (m_pSoundManager == nullptr)
		return -1;

	return m_pSoundManager->BGMVolumeDown(_vol);
}

int CGameInstance::Pause(_uint eID)
{
	if (m_pSoundManager == nullptr)
		return -1;

	return m_pSoundManager->Pause((CSoundMgr::CHANNELID)eID);
}

void CGameInstance::PlayMySound(TCHAR * pSoundKey, _uint eID, _float _vol)
{
	if (m_pSoundManager == nullptr)
		return;

	m_pSoundManager->PlaySound(pSoundKey, (CSoundMgr::CHANNELID)eID, _vol);
}

void CGameInstance::PlayBGM(TCHAR * pSoundKey, _float fVol)
{
	if (m_pSoundManager == nullptr)
		return;

	m_pSoundManager->PlayBGM(pSoundKey, fVol);
}

void CGameInstance::StopSound(_uint eID)
{
	if (m_pSoundManager == nullptr)
		return;

	m_pSoundManager->StopSound((CSoundMgr::CHANNELID)eID);
}

void CGameInstance::StopAll()
{
	if (m_pSoundManager == nullptr)
		return;

	m_pSoundManager->StopAll();
}

ID3D11ShaderResourceView * CGameInstance::Get_RTSRV(const _tchar * pRenderTargetTag) const
{
	if (nullptr == m_pTarget_Manager)
		return nullptr;

	return m_pTarget_Manager->Get_SRV(pRenderTargetTag);
}

HRESULT CGameInstance::Clear_Container(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return E_FAIL;

	m_pObject_Manager->Clear(iLevelIndex);
	m_pComponent_Manager->Clear(iLevelIndex);


	return S_OK;
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);

	return S_OK;
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Present();

	return S_OK;
}



HRESULT CGameInstance::Open_Level(_uint iCurrentLevelIndex, CLevel * pNextLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Change_Level(iCurrentLevelIndex, pNextLevel);
}

_float CGameInstance::Get_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.f;

	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(pTimerTag);
}

void CGameInstance::Update_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return;

	m_pTimer_Manager->Update(pTimerTag);
}
_bool CGameInstance::isIn_Frustum_World(_fvector vPoint, _float fRange)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isIn_Frustum_World(vPoint, fRange);
}



CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pGameObject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pGameObject);
}

HRESULT CGameInstance::Clone_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Clone_GameObject(iLevelIndex, pLayerTag, pPrototypeTag, pArg);
}

HRESULT CGameInstance::Distroy_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, _bool IsAll, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;
	return m_pObject_Manager->Distroy_GameObject(iLevelIndex, pLayerTag, IsAll, iIndex);
}

CGameObject * CGameInstance::Get_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_GameObject(iLevelIndex, pLayerTag, iIndex);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

_byte CGameInstance::Get_DIKeyState(_ubyte eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(eKeyID);
}

_long CGameInstance::Get_DIMMoveState(CInput_Device::DIMOUSEMOVE eMouseMove)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMMoveState(eMouseMove);
}

_byte CGameInstance::Get_DIMButtonState(CInput_Device::DIMOUSEBUTTON eMouseButton)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMButtonState(eMouseButton);
}

_vector CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return XMVectorZero();

	return m_pPipeLine->Get_CamPosition();
}

_float3 CGameInstance::Get_CamFloat3()
{
	if (nullptr == m_pPipeLine)
		return _float3(0.0f, 0.f, 0.f);

	return m_pPipeLine->Get_CamFloat3();
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix(eState);
}

_float4x4 CGameInstance::Get_Transformfloat4x4_TP(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_Transformfloat4x4_TP(eState);
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix StateMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	return m_pPipeLine->Set_Transform(eState, StateMatrix);

}

const LIGHTDESC * CGameInstance::Get_LightDesc(_uint iIndex) const
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const LIGHTDESC & LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(pDevice, pContext, LightDesc);
}

HRESULT CGameInstance::Light_Remove()
{

	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Light_Remove();

}
HRESULT CGameInstance::Add_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pFontTag, const _tchar * pFontFilePath)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Add_Font(pDevice, pContext, pFontTag, pFontFilePath);
}

HRESULT CGameInstance::Render_Font(const _tchar * pFontTag, const _tchar * pString, const _float2 & vPosition, _fvector vColor, float scale)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Render_Font(pFontTag, pString, vPosition, vColor, scale);
}



_uint  CGameInstance::Get_Player_Info()
{
	if (nullptr == m_pPlayer_manager)
		return E_FAIL;

	return m_pPlayer_manager->Get_Player();
}
void  CGameInstance::Set_Player(_uint _player)
{
	if (nullptr == m_pPlayer_manager)
		return;

	return m_pPlayer_manager->Set_Player(_player);
}

_int CGameInstance::Get_Hp()
{
	if (nullptr == m_pHp_Management)
		return E_FAIL;
	return m_pHp_Management->Get_Hp();
}

void CGameInstance::Set_Hp(_int _hp)
{
	if (nullptr == m_pHp_Management)
		return;

	return m_pHp_Management->Set_Hp(_hp);
}

void CGameInstance::Inital_Hp()
{
	if (nullptr == m_pHp_Management)
		return;

	return m_pHp_Management->Inital_Hp();
}

DIRECOTORCAMERADESC CGameInstance::Get_DirecotorCamera()
{
	m_pDirector_Management->ManageMent();
	return m_pDirector_Management->Get_FirstDirecotorCamera();
}

void CGameInstance::Release_Engine()
{
	CGameInstance::Get_Instance()->Destroy_Instance();

	CPicking::Get_Instance()->Destroy_Instance();

	CPlayer_Management::Get_Instance()->Destroy_Instance();
	CHp_Management::Get_Instance()->Destroy_Instance();
	CDirector_Management::Get_Instance()->Destroy_Instance();

	CFont_Manager::Get_Instance()->Destroy_Instance();


	CObject_Manager::Get_Instance()->Destroy_Instance();

	CComponent_Manager::Get_Instance()->Destroy_Instance();

	CLevel_Manager::Get_Instance()->Destroy_Instance();

	CTimer_Manager::Get_Instance()->Destroy_Instance();

	CFrustum::Get_Instance()->Destroy_Instance();

	CTarget_Manager::Get_Instance()->Destroy_Instance();

	CLight_Manager::Get_Instance()->Destroy_Instance();

	CSoundMgr::Get_Instance()->Destroy_Instance();

	CPipeLine::Get_Instance()->Destroy_Instance();

	CInput_Device::Get_Instance()->Destroy_Instance();

	CGraphic_Device::Get_Instance()->Destroy_Instance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pFont_Manager);

	Safe_Release(m_pPicking);
	Safe_Release(m_pPlayer_manager);
	Safe_Release(m_pHp_Management);
	Safe_Release(m_pDirector_Management);


	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pSoundManager);
	Safe_Release(m_pGraphic_Device);
}



