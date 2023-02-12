#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CCollider;
END

BEGIN(Client)
class CPlayer_Slame final : public CGameObject
{
public:
	enum COLLIDER { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
private:
	CPlayer_Slame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Slame(const CPlayer_Slame& Prototype);
	virtual ~CPlayer_Slame() = default;

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
	CVIBuffer_Rect*				m_pVIBufferCom = nullptr;
	CCollider*					m_pColliderCom[TYPE_END] = { nullptr };
private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	_uint m_iTextureNum = 0;
	_bool m_Isrend = false;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();

public:
	static CPlayer_Slame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END