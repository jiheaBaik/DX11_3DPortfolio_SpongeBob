#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Boss_Sandy_Body.h"
#include "Boss_Sandy_Head.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END
BEGIN(Client)
class CBoss_Sandy final : public CGameObject
{
	enum SANDY_ACTION {
		SANDY_INTRO, SANDY_IDLE, SANDY_WALK, SANDY_RUN, SANDY_DEATH, SANDY_DEATH_STOP, SANDY_ATTACK_SPRING_START, SANDY_ATTACK_SPRING1, SANDY_ATTACK_SPRING2,
		SANDY_ATTACK_FALLDOWN, SANDY_ATTACK_JUMP_START, SANDY_ATTACK_JUMP1, SANDY_ATTACK_JUMP2, SANDY_ATTACK_HAND,
		SANDY_HIT_SIT_TOUCH_HEAD1, SANDY_HIT_SIT_TOUCH_HEAD2, SANDY_HIT_SIT_ELECTIVICITY, SANDY_HIT_UP_ELECTIVICITY, SANDY_HIT_SIT_ELECTIVICITY_90AGREE,
		SANDY_PROVOKE_SIT_THRUST_HEAD, SANDY_PROVOKE_SIT_THRUST_HEAD_SHAKE, SANDY_SIT_THRUST_HAND, SANDY_PROVOKE_WALK_THRUST_HEAD, SANDY_PROVOKE_HAND,
		SANDY_GET_HELMET, SANDY_FIXING_HELMET, SANDY_ACTION_END
	};

	typedef struct PatternDesc
	{
		vector<SANDY_ACTION> m_vPatternList1_1;

	}PATTERNDESC;
public:
	CBoss_Sandy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Sandy(const CBoss_Sandy& Prototype);
	virtual ~CBoss_Sandy() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;


	//CRITICAL_SECTION* Get_CriticalSection() {
	//	return &m_CriticalSection;
	//}

private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pVIBufferCom = nullptr;
	CBoss_Sandy_Body * pBoss_Sandy_Body = nullptr;
	CBoss_Sandy_Head * pBoss_Sandy_Head = nullptr;

	//CCollider*					m_pColliderCom = nullptr;
	_uint m_iCurrentAnimIndex = 0;

	PATTERNDESC m_pattern;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();
	_uint nextAnim = 0;
	_uint i = 0;

//private:
//	HANDLE				m_hThread = 0;
//	CRITICAL_SECTION	m_CriticalSection;


public:
	static CBoss_Sandy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
