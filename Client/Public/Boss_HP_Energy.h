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


class CBoss_HP_Energy final : public CGameObject
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
	CBoss_HP_Energy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_HP_Energy(const CBoss_HP_Energy& Prototype);
	virtual ~CBoss_HP_Energy() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;
	void Move();
public:
	void Set_Pos(_float3 _pos) { m_fX = _pos.x, m_fY = _pos.y; }
	void Set_IsEnd() { m_bIsEnd = true; }
private:
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CVIBuffer_Rect*				m_pVIBufferCom = nullptr;

private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	_bool m_bIsEnd = false;
	_uint count = 0;
	_float angle = 0.1;
private:
	OBJINFODESC m_Info;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();

public:
	static CBoss_HP_Energy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END