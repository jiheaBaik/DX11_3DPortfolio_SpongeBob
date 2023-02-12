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
class CTaxi final : public CGameObject
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
	CTaxi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTaxi(const CTaxi& CRing_Top);
	virtual ~CTaxi() = default;
	void Set_FlowerCount() { m_iFlowerCount += 1; }
	_uint Get_FlowerCount() { return m_iFlowerCount; }
	_float fdistance = 0.f;
	_uint m_iStopCount = 0;
	_uint Get_Count() { return count; }
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
	_tchar					m_szFlowerCount[MAX_PATH] = TEXT("");
	_bool m_bisCol = false;
	_uint count = 0;
private:
private:
	_vector m_vpos;
	_uint m_iFlowerCount = 0;
public:
	static CTaxi* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
