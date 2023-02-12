#pragma once
#include "Tool_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Tool)
class CPatrick final : public CGameObject
{
	enum OBJ_CLIENTLEVELE { CLIENT_LEVELE_BOOS1, CLIENT_LEVELE_BOOS2, CLIENT_LEVELE_END };

	typedef struct ObjInfoDesc
	{
		LEVEL toolLevel;
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
	enum TEXTURETYPE { TYPE_DIFFUSE, TYPE_FILTER, TYPE_BRUSH, TYPE_END };
public:
	CPatrick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPatrick(const CPatrick& Prototype);
	virtual ~CPatrick() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;
	void Set_Render(_bool _isRender) { m_isRender = _isRender; }
private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pVIBufferCom = nullptr;

public:
	void SetActionIndex(_uint _index) { m_ActionIndex = _index; }
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	OBJINFODESC m_Info;
	_uint m_ActionIndex = 7;
	_bool m_isRender = false;
public:
	static CPatrick* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
