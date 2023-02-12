#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
//class CNavigation;

END
BEGIN(Client)
class CRobo_3 final : public CGameObject
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
	CRobo_3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRobo_3(const CRobo_3& Prototype);
	virtual ~CRobo_3() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;
	HRESULT IsDie();

private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pVIBufferCom = nullptr;
	//CCollider*					m_pColliderCom = nullptr;
	CCollider*					m_pColliderCom[TYPE_END] = { nullptr };
//	CNavigation*				m_pNavigationCom = nullptr;
	_uint m_imveCount = 0;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();
	_vector m_pos;
	MOTION m_motion;
	void SettingMotion(_double TimeDelt);

	CCollider*					m_pColliderCom2[TYPE_END] = { nullptr };

	_bool m_IsRed = false;
	_uint m_iColorChage = 0;
	_uint m_iHp = 2;
	_uint m_iHitCount = 0;
private:
	OBJINFODESC m_Info;
public:
	static CRobo_3* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
