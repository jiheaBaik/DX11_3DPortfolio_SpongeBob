#include "..\Public\Light_Manager.h"
#include "Light.h"
#include "GameInstance.h"
IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

const LIGHTDESC * CLight_Manager::Get_LightDesc(_uint iIndex) const
{
	auto	iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_LightDesc();
}

HRESULT CLight_Manager::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const LIGHTDESC & LightDesc)
{
	CLight*		pLight = CLight::Create(pDevice, pContext, LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);

	return S_OK;
}

HRESULT CLight_Manager::Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer)
{
	for (auto& pLight : m_Lights)
	{
		if (nullptr != pLight)
		{
			_float4x4			m_WorldMatrix;
			_float3 fpos = pLight->Get_LightDesc()->vPosition;
			XMStoreFloat3((_float3*)&m_WorldMatrix.m[3][0], XMVectorSet(fpos.x, fpos.y, fpos.z, 1));
			CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
			if (false == pLight->Get_LightDesc()->isCulling || true == pGameInstance->isIn_Frustum_World(XMLoadFloat4((_float4*)&m_WorldMatrix.m[3][0]), 2.f))
			{
				pLight->Render(pShader, pVIBuffer);
			}
			else
				int a = 0;
			RELEASE_INSTANCE(CGameInstance);
		}

			
			
	}

	return S_OK;
}

HRESULT CLight_Manager::Light_Remove()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
	return S_OK;
}


void CLight_Manager::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}
