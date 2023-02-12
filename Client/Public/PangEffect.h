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

class CPangEffect final : public CGameObject
{
public:
	typedef struct tagPangeDesc
	{
		_float scale;
		_float addPosition;
		_float interpolation;
		_uint shaderAlpha;

		_float distance;
	}PANGDESC;


private:
	CPangEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPangEffect(const CPangEffect& Prototype);
	virtual ~CPangEffect() = default;

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
	PANGDESC		mPangDesc;

	_int		miLook;

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
	_vector prePos;
public:
	void IsRender(_bool _isRender) { m_IsRender = _isRender; }
public:
	static CPangEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END