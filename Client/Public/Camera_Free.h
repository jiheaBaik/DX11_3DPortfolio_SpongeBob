#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)
//typedef struct ObjInfoDesc
//{
//	//LEVEL toolLevel;
//	//OBJ_CLIENTLEVELE clientLevel;
//	const _tchar * pObj;
//	const _tchar * pLayerTag;
//	const _tchar * pPrototypeTag;
//	const _tchar * pObj_Kind;
//	_uint iIndex;
//	_uint iNumber;
//	_float3 fPos;
//	_float3 fScale;
//	const _tchar * pTextureInfo;
//	_float3 fAxist;
//	_float fRotation;
//
//}OBJINFODESC;
//
//typedef struct DirecotorCameraDesc
//{
//	vector<OBJINFODESC> m_vFirstDirecotorCamera;
//}
//DIRECOTORCAMERADESC;
class CCamera_Free final : public CCamera
{

	

private:
	CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Free(const CCamera_Free& Prototype);
	virtual ~CCamera_Free() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;
	void Set_IsGamePlay(_bool _play) { m_bIsGameplaye = _play; }
	void Set_ShakeInital() { m_iShackCount = 0; }


	_bool Get_m_bIsDirector1() { return m_bIsDirector1; }
	void Set_m_bIsDirector2() { m_bIsDirector2 = true; }
	_bool Get_m_bIsDirector4() { return m_bIsDirector4; }

	_bool Get_m_bIsDirector_End() { return m_bIsDirector_End; }
	void Set_m_bIsDirector_End(_bool _isEnd) { m_bIsDirector_End = _isEnd; }

	_uint Get_m_bIsDirectorCount() { return m_iDirectorCount; }
	void Set_m_bIsDirectorCount() { m_iDirectorCount += 1; }
	void Set_Shake(_bool _isShake) { m_IsShake = _isShake; }
	void Set_Level(_uint _iLevel) { m_iLevel = _iLevel; }

public:
	static CCamera_Free* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


private:
	void Boss1_DirectorCount_1(_double TimeDelta);
	void Boss1_DirectorCount_2(_double TimeDelta);
	void Boss1_DirectorCount_3(_double TimeDelta);
	void Boss1_DirectorCount_4(_double TimeDelta);

private:
	_bool m_bIsGameplaye = false;
	_bool m_bIsDirector1 = false;
	_bool m_bIsDirector2 = false;
	_bool m_bIsDirector3 = false;
	_bool m_bIsDirector4 = false;

	_bool m_bIsDirector_End = false;

	_uint m_iClise_Up = 0;
	_bool m_bIsDirector2_Close_Up_Start = false;
	_bool m_bIsDirector2_Close_Up1 = false;
	_bool m_bIsDirector2_Close_Up2 = false;
	_bool m_bIsDirector2_Close_Up3 = false;

	_uint m_iDirectorCount = 0;

	DIRECOTORCAMERADESC Direcotor_cameradesc;

	_bool m_IsShake = false;
	_bool m_bIsmove = false;
	_float m_fX = 0.03f;
	_float m_fY = 0.1f;
	int m_iShackCount = 0;
	_uint m_iLevel = 0;

	_float fLookYdistance = 4.f;

};

END

