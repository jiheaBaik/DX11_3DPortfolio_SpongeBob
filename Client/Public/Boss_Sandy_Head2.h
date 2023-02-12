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
class CBoss_Sandy_Head2 : public CGameObject
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
	CBoss_Sandy_Head2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Sandy_Head2(const CBoss_Sandy_Head2& Prototype);
	virtual ~CBoss_Sandy_Head2() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;
	_bool Get_IsRender() { return m_isRender; }
	void Set_IsRender(_bool isrender) { m_isRender = isrender; }

	void Set_IsShaderNum(_uint _shaderNum) { m_iShaderNumber = _shaderNum; }
	
private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pVIBufferCom = nullptr;
	CCollider*					m_pColliderCom[TYPE_END] = { nullptr };


public:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();
	void Set_Move();// { m_isMoveEnd = false; }
	

private:
	_float g = 0.1;
	_float vx = 0.1;
	_float vy = 0;
	_float addY = 0.01;
	_float addX= +0.008;

	_bool m_isRender = true;
	_bool m_isUnder = false;
	_bool m_isMoveEnd = false;
	_uint count = 0;

	_bool m_isCillision = false;
	_bool m_isCillision2 = false;
	_bool IsFirstHit = false;

	_uint m_iShaderNumber = 0;
public:
	static CBoss_Sandy_Head2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END