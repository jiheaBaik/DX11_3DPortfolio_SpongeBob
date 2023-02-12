#pragma once

namespace Engine
{
	///////////////////////////////////////
	typedef struct ObjInfoDesc
	{
		//LEVEL toolLevel;
		//OBJ_CLIENTLEVELE clientLevel;
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

	typedef struct DirecotorCameraDesc
	{
		vector<OBJINFODESC> m_vFirstDirecotorCamera;
	}
	DIRECOTORCAMERADESC;
	////////////////////////////////////////////////////////////
	typedef struct tagKeyFrame
	{
		double		Time;

		XMFLOAT3	vScale;
		XMFLOAT4	vRotation;
		XMFLOAT4	vPosition;
	}KEYFRAME;

	typedef struct tagModelMaterial
	{
		class CTexture*	pMaterialTexture[AI_TEXTURE_TYPE_MAX] = { nullptr };

	}MODELMATERIALDESC;

	typedef struct tagGraphicDesc
	{
		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		HWND hWnd;
		unsigned int iWinCX;
		unsigned int iWinCY;
		WINMODE  eWindowed = MODE_END;
	}GRAPHICDESC;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE	eLightType = TYPE_END;
		_float3	vDirection;
		_float3 vPosition;
		_float  fRange;

		_float4 vDiffuse;
		_float4 vAmbient;
		_float4 vSpecular;

		_bool isCulling = true;
	}LIGHTDESC;

	typedef struct tagVertexRectInstance
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;
	}VTXRECTINSTANCE;

	typedef struct ENGINE_DLL tagVertexRectInstance_Declaration
	{
		static const unsigned int iNumElement = 6;
		static D3D11_INPUT_ELEMENT_DESC  Element[iNumElement];
	}VTXRECTINSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertexPointInstance_Declaration
	{
		static const unsigned int iNumElement = 6;
		static D3D11_INPUT_ELEMENT_DESC  Element[iNumElement];
	}VTXPOINTINSTANCE_DECLARATION;




	/*typedef struct tagMaterial
	{
	_float4 vDiffuse;
	_float4 vAmbient;
	_float4 vSpecular;
	};*/

	typedef struct tagVertexAnimModel
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
		XMFLOAT3			vTangent;

		XMUINT4				vBlendIndex; /* 현재 정점의 상태표현에 사용되는 뼈의 갯수.(최대 4개) */
		XMFLOAT4			vBlendWeight; /* 정점에 영향을 주는 각 뼈 상태의 비율.(최대 4개) */
	}VTXANIMMODEL;

	typedef struct ENGINE_DLL tagVertexAnimModel_Declaration
	{
		static const unsigned int iNumElement = 6;
		static D3D11_INPUT_ELEMENT_DESC  Element[iNumElement];
	}VTXANIMMODEL_DECLARATION;


	typedef struct tagVertexModel
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
		XMFLOAT3			vTangent;
	}VTXMODEL;

	typedef struct ENGINE_DLL tagVertexColor_Declaration
	{
		static const unsigned int iNumElement = 2;
		static D3D11_INPUT_ELEMENT_DESC  Element[iNumElement];
	}VTXCOL_DECLARATION;


	typedef struct ENGINE_DLL tagVertexCubeTexture_Declaration
	{
		static const unsigned int iNumElement = 2;
		static D3D11_INPUT_ELEMENT_DESC  Element[iNumElement];
	}VTXCUBETEX_DECLARATION;

	typedef struct tagVertexPoint
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vPSize;
	}VTXPOINT;

	typedef struct ENGINE_DLL tagVertexModel_Declaration
	{
		static const unsigned int iNumElement = 4;
		static D3D11_INPUT_ELEMENT_DESC  Element[iNumElement];
	}VTXMODEL_DECLARATION;

	typedef struct tagVertexColor
	{
		XMFLOAT3			vPosition;
		XMFLOAT4			vColor;
	}VTXCOL;


	typedef struct tagVertexTexture
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vTexUV;
	}VTXTEX;

	typedef struct ENGINE_DLL tagVertexTexture_Declaration
	{
		static const unsigned int iNumElement = 2;
		static D3D11_INPUT_ELEMENT_DESC  Element[iNumElement];
	}VTXTEX_DECLARATION;

	typedef struct tagVertexNormalTexture
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertexNormalTexture_Declaration
	{
		static const unsigned int iNumElement = 3;
		static D3D11_INPUT_ELEMENT_DESC  Element[iNumElement];
	}VTXNORTEX_DECLARATION;



	typedef struct tagFaceIndices16
	{
		unsigned short		_1, _2, _3;
	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long		_1, _2, _3;
	}FACEINDICES32;

	typedef struct tagLineIndices16
	{
		unsigned short		_1, _2;
	}LINEINDICES16;

	typedef struct tagLineIndices32
	{
		unsigned long		_1, _2;
	}LINEINDICES32;

	typedef struct tagVertex_Cube_Texture
	{
		XMFLOAT3		vPosition; /* 위치. */
		XMFLOAT3		vTexUV; /* 텍스쳐 좌표. */
	}VTXCUBETEX;


}