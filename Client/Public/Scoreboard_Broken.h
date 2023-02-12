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
class CScoreboard_Broken final : public CGameObject
{
public:
	enum COLLIDER { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
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

public:
	CScoreboard_Broken(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CScoreboard_Broken(const CScoreboard_Broken& Prototype);
	virtual ~CScoreboard_Broken() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;

	void Set_IsBroken() { m_IsBroken = true; }
	void Move(_double TimeDelta);
private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pVIBufferCom = nullptr;
	CCollider*					m_pColliderCom[TYPE_END] = { nullptr };
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	OBJINFODESC m_Info;
	_bool m_IsBroken = false;


public:
	static CScoreboard_Broken* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
