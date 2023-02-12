#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect_Instance;
END


BEGIN(Tool)

class CGrassInstance final : public CGameObject
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

private:
	CGrassInstance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGrassInstance(const CGrassInstance& Prototype);
	virtual ~CGrassInstance() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;

private:
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CVIBuffer_Rect_Instance*	m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();


public:
	void Reset();
	void IsRender(_bool _isRender) { m_IsRender = _isRender; }
	_bool m_IsRender = false;
	_uint m_icount = 0;
	_uint m_iTextureNum = 0;
private:
	OBJINFODESC m_Info;
public:
	static CGrassInstance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

