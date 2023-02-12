#pragma once

#include <sqlite3.h>
#include "Base.h"
BEGIN(Engine)
class CDirector_Management final : public CBase
{
	DECLARE_SINGLETON(CDirector_Management)

	//typedef struct ObjInfoDesc
	//{
	//	//LEVEL toolLevel;
	//	//OBJ_CLIENTLEVELE clientLevel;
	//	const _tchar * pObj;
	//	const _tchar * pLayerTag;
	//	const _tchar * pPrototypeTag;
	//	const _tchar * pObj_Kind;
	//	_uint iIndex;
	//	_uint iNumber;
	//	_float3 fPos;
	//	_float3 fScale;
	//	const _tchar * pTextureInfo;
	//	_float3 fAxist;
	//	_float fRotation;

	//}OBJINFODESC;

	//typedef struct DirecotorCameraDesc
	//{
	//	vector<OBJINFODESC> m_vFirstDirecotorCamera;
	//}
	//DIRECOTORCAMERADESC;

public:
	CDirector_Management();
	virtual ~CDirector_Management() = default;

	void ManageMent();
	DIRECOTORCAMERADESC Get_FirstDirecotorCamera() { return Direcotor_cameradesc; }
private:
	sqlite3* db;
	OBJINFODESC objInfo;
	DIRECOTORCAMERADESC Direcotor_cameradesc;
public:
	virtual void Free() override;
};

END