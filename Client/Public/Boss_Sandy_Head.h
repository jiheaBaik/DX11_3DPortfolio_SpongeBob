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
class CBoss_Sandy_Head : public CGameObject
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

public:
	CBoss_Sandy_Head(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Sandy_Head(const CBoss_Sandy_Head& Prototype);
	virtual ~CBoss_Sandy_Head() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;
	_bool Get_IsRender() { return m_isRender; }
	void Set_IsRender(_bool isrender) { m_isRender = isrender; }

private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pVIBufferCom = nullptr;
	CCollider*					m_pColliderCom[TYPE_END] = { nullptr };
	_uint m_iCurrentAnimIndex = 0;
	_uint m_iReal_Time_tAnimIndex = 0;//현재 움직이고 있는 인덱스
	vector<ACTIONINFODESC> m_AnimationInfo;

	_float m_iDeletaTimeEdit = 1.f;
	_bool IsHit = false;
	_int m_iHP = 8;
	_bool m_isRender = true;
public:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();
	_uint nextAnim = 0;
	_uint i = 0;
	_bool m_IsScorBoxHit = false;
	void Set_IsScorBoxHit(_bool IsColl) { m_IsScorBoxHit = IsColl; }
	_uint iFootCount = 0;

private:
	int g = 1;
	int vx = 10;
	int vy = -20;


public:
	static CBoss_Sandy_Head* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END