#pragma once
#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
class CTexture;


END

BEGIN(Client)
class CSquidward final : public CGameObject
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
	CSquidward(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSquidward(const CSquidward& Prototype);
	virtual ~CSquidward() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;
	void Set_Dead();
	_bool Get_End() {
		return m_bIsDisolveEnd
			;
	}
private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pVIBufferCom = nullptr;
	CCollider*					m_pColliderCom[TYPE_END] = { nullptr };
	CTexture*					m_pTextureCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	_float fdistance = 0.f;
	_bool m_bIsDisolve = false;
	_bool m_bIsDisolveEnd = false;


	_float  m_Dvalue = 0.f;
	_float m_distroyValue = 0.f;
private:
	OBJINFODESC m_Info;
public:
	static CSquidward* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
