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

class CFlowerEffect final : public CGameObject
{
public:
	typedef struct FlowerDesc
	{
		_bool isTop;
		_bool isBottom;
		_bool isLeft;
		_bool isRight;

		_vector pos;
	}FLOWERDESC;


private:
	CFlowerEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFlowerEffect(const CFlowerEffect& Prototype);
	virtual ~CFlowerEffect() = default;

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
	FLOWERDESC		m_FlowerDesc;
private:
	_float					m_fFrame = 0.f;
	_float4x4					m_BoneOffsetMatrix;
	_float4x4*					m_pBoneMatrix = nullptr;
	_float4x4					m_PivotMatrix;

	_float4x4					m_SocketMatrix;
	

	_bool m_IsTurn = false;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();
	//HRESULT SetUp_SocketMatrixPtr();
	_bool m_IsRender = false;


	_float g = 0.1;
	_float vx = 0.1;
	_float vy = 0;
	_float addY = 0.01;
	_float addX = +0.008;

public:
	void IsRender() { m_IsRender = true; }
public:
	static CFlowerEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END