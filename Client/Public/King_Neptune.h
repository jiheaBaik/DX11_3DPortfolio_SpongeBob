#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END
BEGIN(Client)
class CKing_Neptune final : public CGameObject
{
public:
	CKing_Neptune(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKing_Neptune(const CKing_Neptune& Prototype);
	virtual ~CKing_Neptune() = default;
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
	//CCollider*					m_pColliderCom = nullptr;


private:
	HRESULT SetUp_Components();
	HRESULT SetUp_SRV();

public:
	static CKing_Neptune* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
