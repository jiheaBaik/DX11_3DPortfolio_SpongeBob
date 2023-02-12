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

class CSmokeEffect2 final : public CGameObject
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
	CSmokeEffect2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSmokeEffect2(const CSmokeEffect2& Prototype);
	virtual ~CSmokeEffect2() = default;

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
	_vector pos;
	_bool m_IsTurn = false;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();
	_bool m_IsRender = false;
public:
	void IsRender() { m_IsRender = true; }

	//HRESULT SetUp_SocketMatrixPtr();
public:
	static CSmokeEffect2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END