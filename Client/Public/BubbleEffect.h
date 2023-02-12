#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CBubbleEffect final : public CGameObject
{
public:
	typedef struct tagSmokeDesc
	{
		LEVEL			eLevel;
		const _tchar*	pLayerTag;
		_uint			iIndex;
		const _tchar*	pModelComTag;
		const char*		pBoneName;
	}SMOKEDESC;


private:
	CBubbleEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBubbleEffect(const CBubbleEffect& Prototype);
	virtual ~CBubbleEffect() = default;

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
	SMOKEDESC		mSmokeDesc;
private:
	_float					m_fFrame = 0.f;
	_float4x4					m_BoneOffsetMatrix;
	_float4x4*					m_pBoneMatrix = nullptr;
	_float4x4					m_PivotMatrix;

	_float4x4					m_SocketMatrix;
	_uint m_Info;

	_bool m_IsTurn = false;
	_bool m_IsFirst = false;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();
	//HRESULT SetUp_SocketMatrixPtr();
	_bool m_IsRender = false;
	_float m_fAngle = 0.f;
public:
	void IsRender() { m_IsRender = true; }
public:
	static CBubbleEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END