#pragma once
#include "Tool_Defines.h"
#include "GameObject.h"
BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Tool)
class CBig_Cloth final : public CGameObject
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
	CBig_Cloth(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBig_Cloth(const CBig_Cloth& CRing_Top);
	virtual ~CBig_Cloth() = default;

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

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
private:
	OBJINFODESC m_Info;
public:
	static CBig_Cloth* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
