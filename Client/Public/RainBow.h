#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
//class CCollider;
class CTexture;
class CModel;
END

BEGIN(Client)
class CRainBow final : public CGameObject
{
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

public:
	CRainBow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRainBow(const CRainBow& CRainBow);
	virtual ~CRainBow() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;
	void set_isRender(_bool _isrneder) { m_bisRender = _isrneder; }

private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pVIBufferCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
//	CCollider*					m_pColliderCom[TYPE_END] = { nullptr };
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	_uint m_iCount = 0;
	_float Angle = 0;
	_float addAngle = 0;

	
	_bool m_bisRender = false;
private:
private:
	OBJINFODESC m_Info;
public:
	static CRainBow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
