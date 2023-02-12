#pragma once
#include "Tool_Defines.h"
#include "Base.h"
#include "GameObject.h"
#include <sqlite3.h>
#include <set>


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CVIBuffer_Terrain;
class CMeshContainer;


END
BEGIN(Tool)
class CTerrainManager final : public CBase
{
	DECLARE_SINGLETON(CTerrainManager);
public:
	enum OBJ_KIND { OBJ_CUBE, OBJ_RECT, OBJ_MESH, OBJ_COLLIDER, OBJ_END };
	enum OBJ_CUBETAG { OBJ_BASIC_CUBE , CUBE_END };

	enum OBJ_CLIENTLEVELE { CLIENT_LEVELE_BOSS1 , CLIENT_LEVELE_BOSS2, CLIENT_LEVELE_END };
	enum OBJ_MESH_KIND { MESH_SPONGBOB, MESH_GROUND, MESH_GROUND2, MESH_RING_TOP, MESH_ROPE_TOP, MESH_RING_PILLAR, MESH_LIGHT_TUBE,
						 MESH_SCOREBOARD, MESH_BIG_PILLAR, MESH_BIG_PILLAR_ATTACHMENT, MESH_BIG_CLOTH, POS_LIGHT, STAIRS_02, STAIRS_04,
						 MESH_SM_PD_BANNER_01, MESH_SM_PD_POSTER, MESH_SM_PD_POSTER2, MESH_FISH_A, MESH_FISH_B, MESH_FISH_C, MESH_FISH_D,
						 MESH_FISH_E, MESH_FISH_G, MESH_FISH_H, MESH_FISH_I, MESH_FISH_J, MESH_FISH_K, MESH_FISH_L, MESH_SCOREBOARD_ROPES,/*MESH_SM_PD_SM_LIGHT_CONE_02*/
					     MESH_LIGHT_CONE_PINK,  MESH_LIGHT_CONE_BLUE ,MESH_LIGHT_CONE_GREEN, MESH_BUBBLE, MESH_NOANIMNSPONGEBOB, MESH_CAMERA, MESH_NOANIMNKING, MESH_NOANIMNSANDY, MESH_NOANIMPATRICK, MESH_SCOREBOARD_BROKEN,
		MESH_LIGHT_CONE_YELLOW, MESH_LIGHT_CONE_BLUE2, MESH_FIELD, MESH_TAXI, MESH_GATE, MESH_ROBO, MESH_ROBO2, MESH_ROBO3, MESH_JELLY_PINK, MESH_JELLY_BLUE, MESH_GRASS,
		MESH_SQUIDWARD, MESH_MAN, MESH_TREE, OBJ_MESH_KIND_END };
	
	enum OBJ_ROTATION{ ROLL, PITCH, YAW, ROTATION_END };

	typedef struct ObjInfoDesc
	{
		LEVEL toolLevel;
		OBJ_CLIENTLEVELE clientLevel;
		const _tchar * pObj;
		const _tchar * pLayerTag;
		const _tchar * pPrototypeTag;
		const _tchar * pObj_Kind;

		_uint iIndex;
		_uint iNumber;
		_float3 fPos;
		_float3 fScale;
		const _tchar * pTextureInfo;
		_float3 fAxist;
		_float fRotation;

	}OBJINFODESC;

	typedef struct ObjCLickCountDesc
	{
		_bool isCreate;
		_uint Cube;
		_uint Rect;
		_uint SpongeBob;
		_uint Ground;
		_uint Ground2;
		_uint Ring_Top;
		_uint Rope_Top;
		_uint Ring_Pillar;
		_uint Light_Tube;
		_uint Scoreboard;
		

		_uint Big_Pillar;
		_uint Big_Pillar_Attachment;
		_uint Big_Cloth;
		_uint Pos_Light;
		_uint Stairs_02;
		_uint Stairs_04;
		_uint SM_PD_Banner_01;
		_uint Poster;
		_uint Poster2;
		_uint Fish_A;
		_uint Fish_B;
		_uint Fish_C;
		_uint Fish_D;
		_uint Fish_E;
		_uint Fish_G;
		_uint Fish_H;
		_uint Fish_I;
		_uint Fish_J;
		_uint Fish_K;
		_uint Fish_L;
		_uint Scoreboard_Ropes;
		_uint Light_Cone_Pink;
		_uint Light_Cone_Blue;
		_uint Light_Cone_Green;
		_uint Bubble;
		_uint NonAnimSpongeBob;
		_uint Camera;
		_uint NoneAnimKing;
		_uint NoneAnimSandy;
		_uint NoneAnimPatrick;
		_uint Scoreboard_Broken;
		_uint Light_Cone_Yellow;
		_uint Light_Cone_Blue2;
		_uint Field;
		_uint Taxi;
		_uint Gate;
		_uint Robo;
		_uint Robo2;
		_uint Robo3;
		_uint JellyPink;
		_uint JellyBlue;
		_uint Grass;
		_uint Squidward;
		_uint Man;
		_uint Tree;

		//_uint CSM_Light_Cone_02;

	}OBJCLICKCOUNTDESC;

	static _bool SortData_X(const _float3 &a, const _float3 &b)
	{
		if (a.x < b.x)
			return true;
		else
			return false;
	}
	static _bool SortData_Y(const _float3 &a, const _float3 &b)
	{
		if (a.y < b.y)
			return true;
		else
			return false;
	}


	CTerrainManager();
	CTerrainManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, sqlite3* db);
	virtual~CTerrainManager() = default;

public:
	HRESULT NativeConstruct_Prototype();
	HRESULT NativeConstruct();
	HRESULT Render();
	HRESULT	Get_TerrainComponent();
	
private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
//	bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture);
	CVIBuffer_Terrain*	m_pTerrainVeffer;
	CTransform*	m_pTerrainTransform;
	CNavigation* m_pObjNavigation;
	OBJCLICKCOUNTDESC m_Obj_ClickCount;
private:
	HRESULT DBConnection();
	HRESULT SelectTable();
	HRESULT EditObj();
	HRESULT Load();
	HRESULT Create();
	HRESULT Delete();
	HRESULT Picking();
	vector<string> ChangeListInfo(const _tchar * _objkind);
	void ChangeObjInfo();
	void ChangeLevelInfo();
	_bool CurruntCheck();
	void ObjSelect();
	void ClickCount(_bool _isCreate);
	void CCWCheck(_float3 _MeshPos);
	
private:
	wchar_t pTextureInfo[256];
	OBJINFODESC objInfo;

	//flag
	_bool m_bIsFirst = false;
	_bool m_IsEdit = false;
	_bool m_IsConnection = false;
	_bool isPicking = false;
	_bool m_bIsSelect = false;

	//IMGUI
	_int	m_iObj_current = 0;
	 _int m_iObj_Listcurrent = 0;
	 _int m_ClientLevel = 0;
	 vector<string> m_vecStrListTemp;
	 set<const _tchar*> m_setObjName;
	//DB
	sqlite3* m_db;
	string m_sTable;
	_float3 vPos;

	
	
	vector<_float3> m_vNavigation;
	_bool isNavigation = false;
	
public:
	static CTerrainManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, sqlite3* db);
	virtual void Free() override;
};
END