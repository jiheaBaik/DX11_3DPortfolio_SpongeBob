#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include <sqlite3.h>
BEGIN(Client)
class CLevel_Menu final : public CLevel
{
	enum OBJ_CLIENTLEVELE { CLIENT_LEVELE_BOSS1, CLIENT_LEVELE_BOSS2, CLIENT_LEVELE_MENU, CLIENT_LEVELE_END };


	typedef struct ObjInfoDescUI
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

	}OBJINFODESCUI;
private:
	CLevel_Menu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Menu() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_SpongeBob(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	sqlite3* db;
	OBJINFODESCUI obj_uiInfo;

	_bool m_bIsClick = false;
public:
	static CLevel_Menu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END