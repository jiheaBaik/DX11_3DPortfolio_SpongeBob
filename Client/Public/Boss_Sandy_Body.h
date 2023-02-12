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
class CBoss_Sandy_Body : public CGameObject
{
public:
	enum COLLIDER { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

	enum SANDY_ACTION {
		SANDY_INTRO, SANDY_IDLE, SANDY_WALK, SANDY_RUN, SANDY_DEATH, SANDY_DEATH_STOP, SANDY_ATTACK_SPRING_START, SANDY_ATTACK_SPRING1, SANDY_ATTACK_SPRING2, 
		SANDY_ATTACK_FALLDOWN, SANDY_ATTACK_JUMP_START, SANDY_ATTACK_JUMP1, SANDY_ATTACK_JUMP2, SANDY_ATTACK_HAND,
		SANDY_HIT_SIT_TOUCH_HEAD1, SANDY_HIT_SIT_TOUCH_HEAD2, SANDY_HIT_SIT_ELECTIVICITY, SANDY_HIT_UP_ELECTIVICITY, SANDY_HIT_SIT_ELECTIVICITY_90AGREE,
		SANDY_PROVOKE_SIT_THRUST_HEAD, SANDY_PROVOKE_SIT_THRUST_HEAD_SHAKE, SANDY_SIT_THRUST_HAND, SANDY_PROVOKE_WALK_THRUST_HEAD, SANDY_PROVOKE_HAND,
		SANDY_GET_HELMET, SANDY_FIXING_HELMET, SANDY_ATTACK_SPRING3, SANDY_ATTACK_SPRING4, SANDY_ACTION_END
	};
	typedef struct ActionInfoDesc
	{
		_int ActionLevel;
		_int ActionIndex;

	}ACTIONINFODESC;
	typedef struct PatternDesc
	{
		vector<_uint> m_vPatternList1_1_Attack;
		vector<_uint> m_vPatternList1_1_Hit;
		vector<_uint> m_vPatternList1_2_Hit;
		vector<_uint> m_vPatternList1_3_Hit;
		vector<_uint> m_vPatternList1_4_Hit;
		vector<_uint> m_vPatternList1_5_Hit;


		vector<_uint> m_vPatternList1_2_Attack;
		vector<_uint> m_vPatternList1_3_Attack;
		vector<_uint> m_vPatternList1_4_Attack;



	}PATTERNDESC;
public:
	CBoss_Sandy_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Sandy_Body(const CBoss_Sandy_Body& Prototype);
	virtual ~CBoss_Sandy_Body() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;

	_uint Get_CurrentAnimIndex() {
		return m_iCurrentAnimIndex;
	}

	_uint Get_CurrentNextdex() {
		return nextAnim;
	}
	_uint Get_NextNumber() {
		return i;
	}
	_uint Get_totalAnimNumber() {
		return totalAnim;
	}
	void  Set_CurrentAnimIndex(_uint _animIndex) {
		m_iCurrentAnimIndex = _animIndex;
	}
	_bool Get_IsAnim() {
		return m_bIsAnim;
	}
	_bool Get_IsJumpMotion2() {
		return IsJumpMotion2;
	}
	_bool Get_IsSlamAttack() {
		return IsSlamAttack;
	}

	_bool Get_IsSpringAttack() {
		return IsSpringAttack;
	}
	_bool Get_IsSpringAttack_Ing() {
		return IsSpringAttacking;
	}
	_bool Get_IsTouchHead() {
		return IsTouchHead;
	}

	_bool Get_IsHandAttack() {
		return 	IsHandAttack;
	}

	_bool Get_IsSlamAttackCollision() {
		return 	IsSlamAttackCollision;
	}

	void Set_IsSlamAttackCollision();

	void Set_IsScoreBoxCollision(_bool _isCollision) {
		m_IsScoreBoxCollision = _isCollision;
	}

	_bool Get_IsScoreBoxCollision() {
		return m_IsScoreBoxCollision;
	}

	void IntroShake();
	//}
private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pVIBufferCom = nullptr;
	vector<_uint> tempPattern;
	CCollider*					m_pColliderCom[TYPE_END] = { nullptr };
	CCollider*					m_pColliderComSpring[TYPE_END] = { nullptr };
	CCollider*					m_pColliderComSpring2[TYPE_END] = { nullptr };
	CCollider*					m_pColliderComFallDown[TYPE_END] = { nullptr };

	
	
	//CCollider*					m_pColliderCom = nullptr;

	_uint m_iCurrentAnimIndex = 0;//현재 넘겨주고 있는 인덱스
	_uint m_iReal_Time_tAnimIndex = 0;//현재 움직이고 있는 인덱스

	 _uint i = 0;
	PATTERNDESC m_pattern;
	_bool m_bIsAnim = false;
	_uint nextAnim = 0;
	_uint totalAnim = 0;
	vector<ACTIONINFODESC> m_AnimationInfo;

	_bool IsJumpMotion2 = false;
	_float m_iDeletaTimeEdit = 1.f;
	_bool IsSlamAttack = false;
	_bool IsSlamAttackCollision = false;

	_bool IsSpringAttack = false;
	_bool IsSpringAttacking = false;
	_bool IsHandAttack = false;

	_bool IsScorBoxAttack = false;

	//_bool IsSpringAttackEnd = false;
	_bool IsTouchHead = false;

	_float m_fXVAlue = 0.f;

	_bool move = false;
	_int stomps = 0;
	_int direction = 1;

	_bool m_IsWhite = false;
	_uint m_iColorChage = 0;

	_float fAttack_WalkTime = 0.f;
	_float fAttack_RunTime = 0.f;

	_bool  m_IsScoreBoxCollision = false;
	_uint _JumpAttackcount = 0;

	_bool _isIntroEnd = false;
	_uint _introcount = 0;
public:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();
	void ChangePattern(_uint _animPattern);
	void Set_Last_Anim(_int _addNum);// (_uint _i) { i += _i; }
	void Set_First_Anim(_int _addNum);// (_uint _i) { i += _i; }
	void Set_Free_Anim(_int select, _int _addNum);// (_uint _i) { i += _i; }
	void Set_IsNotRed() { m_IsWhite = false; }
public:
	static CBoss_Sandy_Body* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END


