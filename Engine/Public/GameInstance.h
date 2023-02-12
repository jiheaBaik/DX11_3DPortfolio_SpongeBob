#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "PipeLine.h"
#include "Light_Manager.h"
#include "Picking.h"
#include "Frustum.h"
#include "Player_Management.h"
#include "Hp_Management.h"
#include "Director_Management.h"
#include "Target_Manager.h"
#include "Font_Manager.h"
#include "SoundMgr.h"


/* 클라이언트로 정말 보여줘야할 함수만 모아놓는다. */
/* ㄴ클라이언트 입장에서는 엔진에 기능을 쓰고싶으면 GameIsntance만 생각하면된다. */

BEGIN(Engine)
class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.GameInstance */
	HRESULT Initialize_Engine(_uint iNumLevels, HINSTANCE hInst, const GRAPHICDESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut); /* 엔진의 기능을 사용하기위해 초기화가 필요한 녀석들을 초기화한다. */
	void Tick_Engine(_double TimeDelta); /* 매프레임마다 갱신이 필요한 매니져들의 동작. */
	HRESULT Render_Engine();

	HRESULT Clear_Container(_uint iLevelIndex);

public: /* For.Graphic_Device */
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iCurrentLevelIndex, class CLevel* pNextLevel);

public: /* For.Timer_Manager */
	_float Get_TimeDelta(const _tchar* pTimerTag);
	HRESULT Add_Timer(const _tchar* pTimerTag);
	void Update_Timer(const _tchar* pTimerTag);

public: /*For.Object_Manager */
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pGameObject);
	HRESULT Clone_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);
	HRESULT Distroy_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _bool IsAll, _uint iIndex = 0);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex = 0);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

	//public: /* For.Renderer */
	//	HRESULT Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	//
public: /* For.Input_Device */
	_byte Get_DIKeyState(_ubyte eKeyID);
	_long Get_DIMMoveState(CInput_Device::DIMOUSEMOVE eMouseMove);
	_byte Get_DIMButtonState(CInput_Device::DIMOUSEBUTTON eMouseButton);

public: /* For.PipeLine */
	_vector Get_CamPosition();
	_float3 Get_CamFloat3();
	_matrix Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eState);
	_float4x4 Get_Transformfloat4x4_TP(CPipeLine::TRANSFORMSTATE eState);
	void Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix StateMatrix);

public:
	_uint Get_Player_Info();
	void Set_Player(_uint _player);
	_int Get_Hp();
	void Set_Hp(_int _hp);
	void Inital_Hp();

	DIRECOTORCAMERADESC Get_DirecotorCamera();
public: /* For.Light_Manager */
	const LIGHTDESC* Get_LightDesc(_uint iIndex = 0) const;
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	HRESULT Light_Remove();

public: /* For.font_Manager */
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar*  pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2& vPosition, _fvector vColor, float scale);


public: /* For.Frustum */
	_bool isIn_Frustum_World(_fvector vPoint, _float fRange = 0.f);
public: /* For.Picking */
	_bool Picking(class CTransform* pTranform, class CVIBuffer* pVIBuffer, _float3* pOut = nullptr);
	_float	Get_Pos(class CTransform* pTransform, class CTransform* pPlayerTransform, class CVIBuffer* pVIBuffer);


public: /* For. Sound */
	int  VolumeUp(_uint eID, _float _vol);
	int  VolumeDown(_uint eID, _float _vol);
	int  BGMVolumeUp(_float _vol);
	int  BGMVolumeDown(_float _vol);
	int  Pause(_uint eID);
	void PlayMySound(TCHAR* pSoundKey, _uint eID, _float _vol);
	void PlayBGM(TCHAR* pSoundKey, _float fVol);
	void StopSound(_uint eID);
	void StopAll();
	//void sqlite3_open();

public: /* For.Target_Manager */
	ID3D11ShaderResourceView* Get_RTSRV(const _tchar* pRenderTargetTag) const;
private:
	CGraphic_Device*				m_pGraphic_Device = nullptr;
	CInput_Device*					m_pInput_Device = nullptr;
	CLevel_Manager*					m_pLevel_Manager = nullptr;
	CTimer_Manager*					m_pTimer_Manager = nullptr;
	CObject_Manager*				m_pObject_Manager = nullptr;
	CComponent_Manager*				m_pComponent_Manager = nullptr;
	CPipeLine*						m_pPipeLine = nullptr;
	CLight_Manager*					m_pLight_Manager = nullptr;
	CPicking*						m_pPicking = nullptr;
	CFrustum*						m_pFrustum = nullptr;
	CPlayer_Management*				m_pPlayer_manager = nullptr;
	CHp_Management*				m_pHp_Management = nullptr;
	CDirector_Management*		m_pDirector_Management = nullptr;
	CFont_Manager*					m_pFont_Manager = nullptr;
	CTarget_Manager*				m_pTarget_Manager = nullptr;
	CSoundMgr*						m_pSoundManager = nullptr;

public:
	static void Release_Engine(); /* 엔진에서 사용된 각종 매니져의 메모리 정리. */

public:
	virtual void Free() override;
};

END