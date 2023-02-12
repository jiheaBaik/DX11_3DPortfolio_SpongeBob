#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)
class CSpongeBob_Foot_L  final : public CGameObject
{

public:
	typedef struct tagFootDesc
	{
		LEVEL			eLevel;
		const _tchar*	pLayerTag;
		_uint			iIndex;
		const _tchar*	pModelComTag;
		const char*		pBoneName;
	}FOOTDESC;
private:
	CSpongeBob_Foot_L(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpongeBob_Foot_L(const CSpongeBob_Foot_L& Prototype);
	virtual ~CSpongeBob_Foot_L() = default;

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

	FOOTDESC					m_FootDesc;


	_float4x4					m_BoneOffsetMatrix;
	_float4x4*					m_pBoneMatrix = nullptr;
	_float4x4					m_PivotMatrix;

	_float4x4					m_SocketMatrix;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();
	HRESULT SetUp_SocketMatrixPtr();

public:
	void Set_IsRender(_bool _isRender) { m_bIsRender = _isRender; }
	_bool Get_IsRender() { return m_bIsRender; }

private:
	_bool m_bIsRender = false;
public:
	static CSpongeBob_Foot_L* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

