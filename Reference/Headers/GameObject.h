#pragma once

#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;


public:
	class CComponent* Get_Component(const _tchar* pComponentTag);

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const CTransform::TRANSFORMDESC* pTransformDesc = nullptr);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT  Render();

public:
	/* 부모에 있는 m_Components에도 데이터를 추가. 
	자식에 있는 멤버변수에도 데이터를 추가한다. */
	HRESULT Add_Component(const _tchar* pComponentTag, _uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent** ppOut, void* pArg = nullptr);
	_float	Get_Distance() const { return m_fDistance; }
protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

protected:	
	static const _tchar*	m_pTransformTag;
	class CTransform*		m_pTransformCom = nullptr;

protected:
	map<const _tchar*, class CComponent*>			m_Components;
	typedef map<const _tchar*, class CComponent*>	COMPONENTS;
	_float					m_fDistance = 0.f;
private:
	class CComponent* Find_Component(const _tchar* pComponentTag);

public:
	bool	Get_Dead() { return isDead; }
	void	Set_Dead() { isDead = true; }
protected:
	bool					isDead = false;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END