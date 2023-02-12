#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END
BEGIN(Client)
class CMenu_SpongeBob final : public CGameObject
{
public:
	typedef struct PatrickDesc
	{
		_float fAngle;
		_vector pos;
		_float3 vAxit;
		_float3 fScale;

	}PATRICKDESC;
public:
	CMenu_SpongeBob(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMenu_SpongeBob(const CMenu_SpongeBob& Prototype);
	virtual ~CMenu_SpongeBob() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT  Render() override;
	void Set_Isrender(_bool _isRender) { m_isrender = _isRender; }
private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pVIBufferCom = nullptr;
	//CCollider*					m_pColliderCom = nullptr;

	_float4x4			m_ProjMatrix;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();
	PATRICKDESC m_Info;
	_bool m_isrender = true;
public:
	static CMenu_SpongeBob* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

