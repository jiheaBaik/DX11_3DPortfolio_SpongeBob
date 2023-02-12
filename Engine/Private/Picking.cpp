#include "..\Public\Picking.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "GameInstance.h"
#include "Engine_Defines.h"
#include <DirectXCollision.h>
//#include <DirectXMath.h>
//#include "d3dx11.h"
//#include <DirectXCollision.h>
//#include <DirectXCollision.h>
//#include "qgloal.h"
//
//#include "d3dxGlobal.h"
//
//#include "xnamath.h"
//mathhelper::Infinity
//using namespace DirectX;
//using namespace DirectX::SimpleMath;
IMPLEMENT_SINGLETON(CPicking)

CPicking::CPicking()
{
}

HRESULT CPicking::Initialize(HWND hWnd , ID3D11Device**pDevice, ID3D11DeviceContext **pContext)
{

	m_pDevice = *pDevice;
	m_pContext = *pContext;
	m_hWnd = hWnd;

	//Safe_AddRef(m_pDevice);
	//Safe_AddRef(m_pContext);

	return S_OK;
}

void CPicking::Compute_RayInWorldSpace()
{
	/*CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	_uint viewPortNum = 1;
	m_pContext->RSGetViewports(&viewPortNum, &ViewPortDesc);

	_float4		vPosition;
	vPosition.x = ptMouse.x / (ViewPortDesc.Width * 0.5f) - 1.f;
	vPosition.y = ptMouse.y / (ViewPortDesc.Height * -0.5f) + 1.f;
	vPosition.z = 0.f;
	vPosition.w = 1.f;


	_matrix world, view;
	
	view = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	m_vRayDir = XMVector3Transform(XMLoadFloat4(&vPosition), view);
	m_vRayPos = _vector{ 0, 0, 0, 1 };

	world = XMMatrixInverse(nullptr, view);
	m_vRayPos = XMVector3TransformCoord(m_vRayPos, world);
	m_vRayDir = XMVector3TransformNormal(m_vRayDir, world);
	

	Safe_Release(pGameInstance);*/

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(m_hWnd, &ptCursor);

	UINT uintNum = 1;
	D3D11_VIEWPORT            ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof(D3D11_VIEWPORT));
	m_pContext->RSGetViewports(&uintNum, &ViewportDesc);

	_float4 vPosition;
	vPosition.x = ptCursor.x / (ViewportDesc.Width * 0.5f) - 1.f;
	vPosition.y = ptCursor.y / (ViewportDesc.Height * -0.5f) + 1.f;
	vPosition.z = 0.f;
	vPosition.w = 1.f;

	_matrix    ProjMatrix = pGameInstance->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ);
	ProjMatrix = XMMatrixInverse(nullptr, ProjMatrix);

	m_vRayDir = XMVector3Transform(XMLoadFloat4(&vPosition), ProjMatrix);
	m_vRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	_matrix ViewMatrix = pGameInstance->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);

	m_vRayDir = XMVector3TransformNormal(m_vRayDir, ViewMatrix);
	m_vRayPos = XMVector3TransformCoord(m_vRayPos, ViewMatrix);


	Safe_Release(pGameInstance);


}

_float CPicking::Get_Pos(class CTransform* pTransform, class CTransform* pPlayerTransform, class CVIBuffer* pVIBuffer)
{



	const _float3*	pVerticesPos = pVIBuffer->Get_VerticesPos();
	_uint		iNumFaces = pVIBuffer->Get_NumPrimitive();



	_vector vPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 fpos;
	XMStoreFloat3(&fpos, vPos);
	for (_uint i = 0; i < iNumFaces; ++i)
	{
		if (pVerticesPos->x - 0.2 <= fpos.x  && pVerticesPos->x + 0.2 >= fpos.x
			&& pVerticesPos->z - 0.2 <= fpos.z  && pVerticesPos->z + 0.2 >= fpos.z)
		{
			return pVerticesPos->y;
		}
	}
}

_bool CPicking::Picking(class CTransform* pTransform, class CVIBuffer* pVIBuffer, _float3* pOut)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_vector		vRayDirInLocal, vRayPosInLocal;
	
	_matrix world, Local;
	world = pTransform->Get_WorldMatrix();
	Local = XMMatrixInverse(nullptr, world);
	
	vRayPosInLocal = XMVector3TransformCoord(m_vRayPos, Local);
	vRayDirInLocal = XMVector3TransformNormal(m_vRayDir, Local);

	vRayDirInLocal = XMVector3Normalize(vRayDirInLocal);

	_uint		iNumFaces = pVIBuffer->Get_NumPrimitive();
	const _float3*	pVerticesPos = pVIBuffer->Get_VerticesPos();

	const void*		pIndices = pVIBuffer->Get_Indices();
	DXGI_FORMAT		eFormat = pVIBuffer->Get_IndexFormat();

	_uint			iSize = 0;

	if (eFormat == DXGI_FORMAT_R16_UINT)
		iSize = sizeof(FACEINDICES16);
	else
		iSize = sizeof(FACEINDICES32);


	_float	 fDist;

	for (_uint i = 0; i < iNumFaces; ++i)
	{
		_uint		iIndices[3];
	//	memcpy(&iIndices, (_byte*)pIndices + iSize * i, iSize);
		iIndices[0] = ((FACEINDICES32*)pIndices)[i]._1;
		iIndices[1] = ((FACEINDICES32*)pIndices)[i]._2;
		iIndices[2] = ((FACEINDICES32*)pIndices)[i]._3;


	/*	FXMVECTOR Pos0 = XMLoadFloat3(&pVerticesPos[iIndices[0]]);
		GXMVECTOR Pos1 = XMLoadFloat3(&pVerticesPos[iIndices[1]]);
		HXMVECTOR Pos2 = XMLoadFloat3(&pVerticesPos[iIndices[2]]);*/

		FXMVECTOR Pos0 = XMLoadFloat3(&pVerticesPos[iIndices[0]]);
		GXMVECTOR Pos1 = XMLoadFloat3(&pVerticesPos[iIndices[1]]);
		HXMVECTOR Pos2 = XMLoadFloat3(&pVerticesPos[iIndices[2]]);

		if (true == TriangleTests::Intersects((FXMVECTOR)vRayPosInLocal,(FXMVECTOR)vRayDirInLocal, Pos0, Pos1, Pos2, fDist))
		{
			_vector		vColPos;

			if (nullptr != pOut)
			{
				world = pTransform->Get_WorldMatrix();

				//_vector		vDist = XMLoadFloat(&fDist);
				vColPos =  vRayPosInLocal + vRayDirInLocal * fDist;
				// 충돌 위치 계산
				 vColPos = XMVector3TransformCoord(vColPos, world);
				
				XMStoreFloat3(pOut, vColPos);
			}
			Safe_Release(pGameInstance);
			return true;
		}
	}
	Safe_Release(pGameInstance);
	return false;
}


void CPicking::Free()
{
	/*Safe_Release(m_pDevice);
	Safe_Release(m_pContext);*/

}
