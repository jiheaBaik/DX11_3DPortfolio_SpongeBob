#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHABLEND, RENDER_NONLIGHT, RENDER_ALPHABLEND, RENDER_UI, RENDER_END };
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Add_DebugGroup(class CComponent* pDebugCom) {
		m_DebugComs.push_back(pDebugCom);
		Safe_AddRef(pDebugCom);
		return S_OK;
	}

public:
	HRESULT Draw();

private:
	list<class CGameObject*>					m_RenderGroup[RENDER_END];
	typedef list<class CGameObject*>			RENDERGROUPS;

private:
	list<class CComponent*>						m_DebugComs;

private:
	class CTarget_Manager*						m_pTarget_Manager = nullptr;
	class CVIBuffer_Rect*						m_pVIBuffer = nullptr;
	class CShader*								m_pShader = nullptr;
	class CLight_Manager*						m_pLight_Manager = nullptr;
	_float4x4									m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

private:
	HRESULT Render_Priority();

	HRESULT Render_NonAlphaBlend();

	/* 빛을 그린다.(빛 연산을 통해서 연산의 결과물을 기록한다.(셰이더타겟에다가. ) ). */
	HRESULT Render_Lights();

	HRESULT Render_Blend();

	HRESULT Render_NonLight();

	HRESULT Render_AlphaBlend();

	HRESULT Render_UI();
#ifdef _DEBUG
	HRESULT Render_Debug();
#endif 

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END