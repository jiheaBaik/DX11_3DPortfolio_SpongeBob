#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Player_Typedef.h"


BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CNavigation;
class CCollider;
END
BEGIN(Client)
class CPatrick_Player final : public CGameObject
{
	typedef struct PatternDesc
	{
		vector<_uint> m_vPatternList1_1_Intro;




	}PATTERNDESC;

public:
	enum COLLIDER { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

	typedef struct ActionInfoDesc
	{
		_int ActionLevel;
		_int ActionIndex;

	}ACTIONINFODESC;
public:
	CPatrick_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPatrick_Player(const CPatrick_Player& Prototype);
	virtual ~CPatrick_Player() = default;
	
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;
	
private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pVIBufferCom = nullptr;
	CCollider*					m_pColliderCom[TYPE_END] = { nullptr };
	CNavigation*				m_pNavigationCom = nullptr;
	vector<ACTIONINFODESC> m_AnimationInfo;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();

public:
	_bool Get_IsJump() { return m_bIsJump; }
	_bool Get_IsSlam() { return IsSlam; }
	_bool Get_IsFoot() { return IsFoot; }
	_bool Get_IsAttack2() { return m_isAttack2; }
	//_uint Get_Hp() {return m_iHP;}
	//void Set_Hp(_uint _hp) { m_iHP = _hp; }
	void Set_Shake(_bool _isShake) { m_IsShake = _isShake; }

	_bool Get_IsFlaying() { return m_Flying; }

private:
	_float GravityPower = 50;
	_float MaxSpeedPerSeconds = 10;
	_float m_FallSpeed = 0.f;
	//_uint m_iCurrentAnimIndex = 0;
	_bool m_bIsJump = false;
	_uint walkCount = 0;

	_bool IsAttack = false;
	_bool IsIdle = false;
	_bool IsSlam = false;
	_bool IsHit = false;
	_bool IsFoot = false;
	_bool m_IsShake = false;

	//_int m_iHP;
	_bool m_isAttack2 = false;

	_float m_fDown = 0.f;
	_float m_fSlowDown = 0.f;

	PATTERNDESC m_pattern;
	_uint m_iCurrentAnimIndex = 0;//현재 넘겨주고 있는 인덱스
	_uint nextAnim = 0;
	int next = 0;
	vector<_uint> tempPattern;

	_bool m_isPickUp_SandyHead = false;
	_bool m_isPickUpStart_SandyHead = false;
	_bool m_isThrow_SandyHead = false;

	_bool m_IsWhite = false;
	_uint m_iColorChage = 0;
	_bool m_Flying = false;
	_uint m_FlyingCount = 0;

public:
	void Gravity(_double TimeDelta);
public:
	static CPatrick_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
