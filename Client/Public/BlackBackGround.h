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
class CBlackBackGround final : public CGameObject
{
	enum OBJ_CLIENTLEVELE { CLIENT_LEVELE_BOOS1, CLIENT_LEVELE_BOOS2, CLIENT_LEVELE_END };

	

private:
	CBlackBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBlackBackGround(const CBlackBackGround& Prototype);
	virtual ~CBlackBackGround() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;
public:
	void Set_Pos(_float3 _pos) {  m_fX = _pos.x,  m_fY = _pos.y; }
	void Set_TexutureNum(_uint _num) { m_iTextureNum = _num; }
	void Set_IsRender(_bool _iRender) { m_IsRender = _iRender; }

private:
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CVIBuffer_Rect*				m_pVIBufferCom = nullptr;

private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

	_double m_dTotalTime = 0.f;;
	_bool m_isUnder = false;


	_bool m_IsRender = false;
	_bool m_isFirst = false;

//	_bool m_isRender = false;

	_uint m_iTextureNum = 0;
	_float m_iCount = 0;

private:
	_bool m_Info;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();
	

public:
	static CBlackBackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END