#pragma once

#include "Tool_Defines.h"
#include "Level.h"
#include "TerrainManager.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Tool)

class CLevel_UI final : public CLevel
{
	enum OBJ_CLIENTLEVELE { CLIENT_LEVELE_BOSS1, CLIENT_LEVELE_BOSS2, CLIENT_LEVELE_MENU, CLIENT_LEVELE_END };
	enum OBJ_IMFO { UNDERPANTS, BOSS_HP_BAR, GAME_START, EXIT, MENU_BAR, ENERGY, OBJ_END };

	
	typedef struct ObjInfoDesc
	{
		//LEVEL toolLevel;
		OBJ_CLIENTLEVELE clientLevel;
		const _tchar * pObj;
		const _tchar * pLayerTag;
		const _tchar * pPrototypeTag;

		_uint iIndex;
		_uint iNumber;
		_float2 fPos;
		_float2 fScale;
		_uint pTextureInfo;
		_float3 fAxist;
		_float fRotation;
		sqlite3* db;
		string m_Table;

	}OBJINFODESC;

	typedef struct ObjCLickCountDesc
	{
		_uint Underpants;
		_uint Boss_HPBar;

	}OBJCLICKCOUNTDESC;
private:
	CLevel_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_UI() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render() override;

private:
	HRESULT	Ready_Prototype_Component();
	HRESULT	Ready_Clone_GameObject();

	HRESULT DBConnection();
	HRESULT Create();
private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	_bool m_IsConnection = false;
	string m_sTable;
	sqlite3* db;
	OBJINFODESC  m_objInfo;
	_bool m_bIsFirst = false;
	OBJCLICKCOUNTDESC m_Obj_ClickCount;
	vector<string> m_vecStrListTemp;
	_int	m_iObj_current = 0;
public:
	HRESULT SelectTable();
	HRESULT Load();
	void  InsertObj();
public:
	static CLevel_UI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END