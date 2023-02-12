#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)
class CSpongeBob_Wand final : public CGameObject
{
public:
	typedef struct tagWandDesc
	{
		LEVEL			eLevel;
		const _tchar*	pLayerTag;
		_uint			iIndex;
		const _tchar*	pModelComTag;
		const char*		pBoneName;
	}WANDDESC;
private:
	CSpongeBob_Wand(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpongeBob_Wand(const CSpongeBob_Wand& Prototype);
	virtual ~CSpongeBob_Wand() = default;

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

	WANDDESC					m_WandDesc;


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
private:
	_bool m_bIsRender = false;
public:
	static CSpongeBob_Wand* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

