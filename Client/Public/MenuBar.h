#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include <sqlite3.h>
BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END
BEGIN(Client)

class CMenuBar final : public CGameObject
{
	enum OBJ_CLIENTLEVELE { CLIENT_LEVELE_BOOS1, CLIENT_LEVELE_BOOS2, CLIENT_LEVELE_END };

	typedef struct ObjInfoDesc
	{
		//LEVEL toolLevel;
		OBJ_CLIENTLEVELE clientLevel;
		const _tchar * pObj;
		const _tchar * pLayerTag;
		const _tchar * pPrototypeTag;

		_uint iIndex;
		_uint iNumber;
		_float2 fPos;
		_float2 fScale;
		_uint pTextureInfo;
		_float3 fAxist;
		_float fRotation;
		sqlite3* db;
		string m_Table;

	}OBJINFODESC;

private:
	CMenuBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMenuBar(const CMenuBar& Prototype);
	virtual ~CMenuBar() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;
public:
	void Set_Pos(_float3 _pos) { m_fX = _pos.x, m_fY = _pos.y; }

private:
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CVIBuffer_Rect*				m_pVIBufferCom = nullptr;

private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;


public:
	_bool Get_Click() { return m_bIsClick; }
	
private:
	OBJINFODESC m_Info;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();
	_uint m_iTextureNum = 0;
	_uint m_iMove = 0;
	_bool m_bIsClick = false;

private:
	_bool m_bMove = false;
	_int m_iCount = 0;
//	_float m_fX = 0.004f;
	_uint m_frame = 0.f;
	_float m_fDeadFrame = 0.f;
	_vector m_vMenuBar;
public:
	static CMenuBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END