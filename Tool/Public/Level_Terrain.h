#pragma once

#include "Tool_Defines.h"
#include "Level.h"
#include "TerrainManager.h"

BEGIN(Engine)
class CTransform;
END


BEGIN(Tool)

class CLevel_Terrain final : public CLevel
{
private:
	CLevel_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Terrain() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render() override;

private:
	HRESULT	Ready_Prototype_Component();
	HRESULT	Ready_Clone_GameObject();
	/*CCube_Manager* m_pCubeMgrInstance = nullptr;
	CToolObject_Manager* m_pMonsterMgrInstance = nullptr;*/
	HRESULT DBConnection();
private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	CTerrainManager* m_pTerrainMgrInstance = nullptr;
	_bool m_IsConnection = false;
	string m_sTable;
	sqlite3* db;
public:
	static CLevel_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END