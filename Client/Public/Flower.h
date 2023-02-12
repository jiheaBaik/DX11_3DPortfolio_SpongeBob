#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;


END

BEGIN(Client)
class CFlower final : public CGameObject
{
	enum COLLIDER { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

public:
	typedef struct FlowerDesc
	{
		_vector vLook;
		_vector pos;
		_vector vColor;
	}FLOWERDESC;

public:
	CFlower(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFlower(const CFlower& CRing_Top);
	virtual ~CFlower() = default;

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
	CCollider*					m_pColliderCom[TYPE_END] = { nullptr };
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	FLOWERDESC m_flowrDesc;
	_float g = 0.1;
	_float vy = 0;
	_float addY = 0.002;
	_float addY2 = 0.002;

	_bool m_isUnder = false;
	_bool m_isMove = true;

private:
	OBJINFODESC m_Info;
public:
	static CFlower* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
