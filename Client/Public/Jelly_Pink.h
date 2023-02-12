#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
END
BEGIN(Client)
class CJelly_Pink final : public CGameObject
{
public:
	enum OBJ_CLIENTLEVELE { CLIENT_LEVELE_BOOS1, CLIENT_LEVELE_BOOS2, CLIENT_LEVELE_END };
	typedef struct ObjInfoDesc
	{
		//LEVEL toolLevel;
		OBJ_CLIENTLEVELE clientLevel;
		const _tchar * pObj;
		const _tchar * pLayerTag;
		const _tchar * pPrototypeTag;
		const _tchar * pObj_Kind;
		_uint iIndex;
		_uint iNumber;
		_float3 fPos;
		_float3 fScale;
		const _tchar * pTextureInfo;
		_float3 fAxist;
		_float fRotation;

	}OBJINFODESC;


	enum COLLIDER { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
	enum MOTION { IDLE, ATTACK, HIT, CHASE };

public:
	CJelly_Pink(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CJelly_Pink(const CJelly_Pink& Prototype);
	virtual ~CJelly_Pink() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;
	void SettingMotion(_double TimeDelt);
	HRESULT IsDie(_double TimeDelta);
private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pVIBufferCom = nullptr;
	CCollider*					m_pColliderCom[TYPE_END] = { nullptr };
	CCollider*					m_pColliderCom2[TYPE_END] = { nullptr };



	_bool m_IsRed = false;
	_uint m_iColorChage = 0;
	_uint m_iHp = 2;
	_uint m_iHitCount = 0;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();
	_vector m_pos;
	MOTION m_motion;
	_float m_iSlowMotionCount = 0;
private:
	OBJINFODESC m_Info;

	_float  m_Dvalue = 0.f;
	_float m_distroyValue = 0.f;
	_bool m_bIsDisolve = false;

public:
	static CJelly_Pink* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
