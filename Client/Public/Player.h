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
class CPlayer final : public CGameObject
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
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

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
	CNavigation*				m_pNavigationCom2 = nullptr;

	//CNavigation*				m_pNavigationCom2 = nullptr;
	vector<ACTIONINFODESC> m_AnimationInfo;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();

public:
	_bool Get_IsJump() { return m_bIsJump; }
	_bool Get_IsSlam() { return IsSlam; }
	_bool Get_IsFoot() { return IsFoot; }
	_bool Get_IsAttack2() { return m_isAttack2; }
	_bool Get_IsAttack() { return IsAttack; }
	void Set_Isrender(_bool _render) { m_bIsRender = _render; }
	HRESULT Set_Navigation();
	/*_uint Get_Hp() {return m_iHP;}
	void Set_Hp(_uint _hp) { m_iHP = _hp; }*/
	void Set_Shake(_bool _isShake) { m_IsShake = _isShake; }

	_bool Get_IsFlaying() { return m_Flying; }

	void Set_m_Info(_uint _m_Info) {
		m_Info = _m_Info;
	}

	_uint Get_Info() { return m_Info; }
	void Set_IsHit_Field() { IsHit_Field = true; }
	_bool Get_IsHit_Field() {return  IsHit_Field; }
	_float Get_SlowMotion() { return  m_iSlowMotionCount; }

	
	HRESULT BubbleCreate();
	//_float Get_Ypos
private:
	_bool m_bIsRender = false;
	_float GravityPower = 50;
	_float MaxSpeedPerSeconds = 10;
	_float m_FallSpeed = 0.f;
	//_uint m_iCurrentAnimIndex = 0;
	_bool m_bIsJump = false;

	_bool IsAttack = false;
	_bool IsIdle = false;
	_bool IsSlam = false;
	_bool IsHit = false;
	_bool IsHit_Field = false;
	_uint iHitCount =0;



	_bool IsFoot = false;
	_bool m_IsShake = false;
	_bool m_isAttack2 = false;

	_int m_iHP;

	_float m_fDown = 0.f;
	_float m_fSlowDown = 0.f;
	_bool m_bIsmove = false;
	_uint walkCount = 0;
	PATTERNDESC m_pattern;
	_uint m_iCurrentAnimIndex = 0;//현재 넘겨주고 있는 인덱스
	_uint nextAnim = 0;
	int next = 0;
	vector<_uint> tempPattern;

	_bool m_IsWhite = false;
	_uint m_iColorChage = 0;
	//_uint m_iColorChageTotal = 0;

	_bool m_Flying = false;
	_float m_floaGetY = 0.f;
	_uint m_Info;
	_uint m_FlyingCount = 0;

	_uint m_iBubbleCount = 0;

	_float m_iSlowMotionCount = 0;
public:
	void Gravity(_double TimeDelta);
public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
