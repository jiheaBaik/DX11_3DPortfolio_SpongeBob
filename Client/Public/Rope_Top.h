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
class CRope_Top final : public CGameObject
{
public:
	enum COLLIDER { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };


	enum OBJ_CLIENTLEVELE { CLIENT_LEVELE_BOOS1, CLIENT_LEVELE_BOOS2, CLIENT_LEVELE_END };

	typedef struct ObjInfoDesc
	{
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

public:
	CRope_Top(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRope_Top(const CRope_Top& CRing_Top);
	virtual ~CRope_Top() = default;

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
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
public:
	void Set_IsAction(_bool _isAction) { m_IsAction = _isAction; }
	void Set_IsBroken(_bool _isBroken) { m_IsBroken = _isBroken; }

private:
private:
	OBJINFODESC m_Info;
	_bool m_IsAction = false;
	_bool m_IsBroken = false;
public:
	static CRope_Top* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END


