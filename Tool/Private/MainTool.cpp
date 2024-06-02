#include "..\Public\MainTool.h"
#include "GameInstance.h"
#include <DirectXMath.h>

#include "Camera_Free.h"
#include "Terrain.h"
#include "Cube.h"



#include "BackGround.h"
#include "Terrain.h"
#include "Camera_Free.h"
#include "Cube.h"
#include "SpongeBob.h"
#include "Ground.h"
#include "Ground2.h"
#include "Ring_Top.h"
#include "Rope_Top.h"
#include "Ring_Pillar.h"
#include "Light_Tube.h"
#include "Scoreboard.h"
#include "Scoreboard_Broken.h"

#include "Big_Pillar.h"
#include "Big_Pillar_Attachment.h"
#include "Big_Cloth.h"
#include "Pos_Light.h"
#include "Stairs_02.h"
#include "Stairs_04.h"
#include "SM_PD_Banner_01.h"
#include "Poster.h"
#include "Poster2.h"
#include "SM_Light_Cone_02.h"
#include "Light_Tube.h"
#include "Fish_A.h"
#include "Fish_B.h"
#include "Fish_C.h"
#include "Fish_D.h"
#include "Fish_E.h"
#include "Fish_G.h"
#include "Fish_H.h"
#include "Fish_I.h"
#include "Fish_J.h"
#include "Fish_K.h"
#include "Fish_L.h"
#include "Scoreboard_Ropes.h"
//UI
#include "Underpants.h"
#include "Boss_HPBar.h"
#include "MenuBar.h"
#include "Disable_Exit.h"
#include "Disable_Geme_Start.h"

#include "Boss_HP_Energy.h"


#include "Navigation.h"

#include "Boss_Sandy.h"
#include "Boss_Sandy_Body.h"


#include "Light_Cone_Pink.h"
#include "Light_Cone_Blue.h"
#include "Light_Cone_Green.h"

#include "Light_Cone_Blue2.h"
#include "Light_Cone_Yellow.h"


#include "Bubble.h"
#include "Patrick.h"

#include "BossSandy_NonAnim.h"
#include "Camera_DirectorMesh.h"
#include "King_NonAnim.h"
#include "SpongeBob_NonAnim.h"

#include "Patrick_NonAnim.h"
#include "Field.h"
#include "Taxi.h"
#include "Gate.h"
#include "Robo.h"
#include "Robo2.h"
#include "Robo3.h"
#include "Jelly_Pink.h"
#include "Jelly_Blue.h"
#include "VIBuffer_Point_Instance.h"
#include "GrassInstance.h"
#include "Squidward.h"
#include "Man.h"

#include "Tree.h"




CMainTool::CMainTool()
	:m_pGameInstance(CGameInstance::Get_Instance())
	,m_pImgui_Manager(CImgui_Manager::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pImgui_Manager);
}

HRESULT CMainTool::NativeConstruct()
{
	GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.eWindowed = GRAPHICDESC::MODE_WIN;
	GraphicDesc.iWinCX = g_iWinSizeX;
	GraphicDesc.iWinCY = g_iWinSizeY;
	
	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, g_hInst, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component_Static()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	if (FAILED(m_pImgui_Manager->NativeConstruct(m_pDevice, m_pContext)))
		return E_FAIL;

	return S_OK;
}

void CMainTool::Tick(float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;
#endif

	m_pGameInstance->Tick_Engine(fTimeDelta);
}

HRESULT CMainTool::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.741f, 0.741f, 0.741f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pRenderer->Draw();
	m_pImgui_Manager->Frame();

#ifdef _DEBUG
	++m_iNumRender;

	_tchar szFPS[MAX_PATH];
	if (m_fTimeAcc > 1.f) {
		wsprintf(szFPS, TEXT("FPS : %d \n"), m_iNumRender);
		SetWindowText(g_hWnd, szFPS);

		m_fTimeAcc = 0.f;
		m_iNumRender = 0;
	}
#endif

	m_pImgui_Manager->Render();

	m_pGameInstance->Present();
	return S_OK;
}

HRESULT CMainTool::Ready_Prototype_Component_Static()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElement))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElement))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElement))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Element, VTXANIMMODEL_DECLARATION::iNumElement))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"),
		CCube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../Client/Bin/Data/Navigation.dat")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation2"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../Client/Bin/Data/Navigation2.dat")))))
		return E_FAIL;
	

	_matrix PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ground"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_Ground_01/", "SM_Ground_01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Ground"),
		CGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.012f, 0.012f, 0.012f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ground2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_Ground_02/", "SM_Ground_02.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Ground2"),
		CGround2::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring_Top"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_Ring_Top/", "SM_Ring_Top.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Ring_Top"),
		CRing_Top::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rope_Top"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_PD_Rope_Top/", "SM_PD_Rope_Top_01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Rope_Top"),
		CRope_Top::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring_Pillar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Ring_Pillar/", "SM_PD_Ring_Pillar.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Ring_Pillar"),
		CRing_Pillar::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Light_Tube"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Light_Tube/", "SM_PD_Light_Tube_01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Light_Tube"),
		CLight_Tube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Scoreboard"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Scoreboard/", "SM_PD_Scoreboard_01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Scoreboard"),
		CScoreboard::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Scoreboard_Broken"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Scoreboard/", "SM_PD_Scoreboard_Broken.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Mesh_scoreboard_broken"),
		CScoreboard_Broken::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Big_Pillar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Big_Pillar/", "SM_Big_Pillar_01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Big_Pillar"),
		CBig_Pillar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Big_Pillar_Attachment"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Big_Pillar_Attachment/", "SM_Big_Pillar_Attachment_01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Big_Pillar_Attachment"),
		CBig_Pillar_Attachment::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Big_Cloth"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Big_Cloth/", "SM_big_cloth_01.props.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Big_Cloth"),
		CBig_Cloth::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Pos_Light"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Pos_Light/", "SM_Pos_Light.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Pos_Light"),
		CPos_Light::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Stairs_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Stairs_02/", "SM_Stairs_02.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Stairs_02"),
		CStairs_02::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Stairs_04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Stairs_04/", "SM_Stairs_04.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Stairs_04"),
		CStairs_04::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_SM_PD_Banner_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_PD_Banner_01/", "SM_PD_Banner_01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_SM_PD_Banner_01"),
		CSM_PD_Banner_01::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Poster"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Poster_01/", "SM_PD_Poster_01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Poster"),
		CPoster::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Poster2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Poster_02/", "SM_PD_Poster_02.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Poster2"),
		CPoster2::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 200, 200))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(173.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Taxi"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Taxi/", "TaxiStop.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_TaxiStop"),
		CTaxi::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(173.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Gate"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Gate/", "Gate.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Gate"),
		CGate::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Robo"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Robo/", "Robo.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Robo"),
		CRobo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Robo2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Robo_2/", "Robo2.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Robo2"),
		CRobo2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Robo3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Robo_3/", "Robo3.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Robo3"),
		CRobo3::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Jelly_Pink"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Jelly/Pink/", "Jelly_Pink.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Jelly_Pink"),
		CJelly_Pink::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Jelly_Blue"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Jelly/Blue/", "Jelly_Blue.fbx", PivotMatrix))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Jelly_Blue"),
		CJelly_Blue::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Squidward"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Npc/", "Squidward.fbx", PivotMatrix))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Squidward"),
		CSquidward::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Man"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Npc/", "Man.fbx", PivotMatrix))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Man"),
		CMan::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Scoreboard_Ropes"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Scoreboard_Ropes/", "Scoreboard_Ropes.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Scoreboard_Ropes"),
		CScoreboard_Ropes::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Light_Cone_Pink"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/LightCone/", "LightCone_Piink.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Light_Cone_Pink"),
		CLight_Cone_Pink::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Light_Cone_Blue"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/LightCone/", "LightCone_Blue.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Light_Cone_Blue"),
		CLight_Cone_Blue::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Light_Cone_Blue2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/LightCon2/", "Cone2Blue.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Light_Cone_Blue2"),
		CLight_Cone_Blue2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Light_Cone_Yellow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/LightCon2/", "Cone2Yellow.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Light_Cone_Yellow"),
		CLight_Cone_Yellow::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Light_Cone_Green"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/LightCone/", "LightCone_Green.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Light_Cone_Green"),
		CLight_Cone_Green::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Grass"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Model/Grass/Grass.dds"), 1))))
		return E_FAIL;
	//UI
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Underpants"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Bubble_0.dds"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Underpants"),
		CUnderpants::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_HPBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/LongBar/Boss_HPBar%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_HPBar"),
		CBoss_HPBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Energy"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/LongBar/boss_energy_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Energy"),
		CBoss_HP_Energy::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Menu/MenuBar_01.dds"), 1))))
		return E_FAIL;

	
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_MenuBar"),
		CMenuBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Disable_Exit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Menu/Endable_Exit0.png"), 1))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Disable_Exit"),
		CDisable_Exit::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Disable_Geme_Start"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Menu/Endable_Geme_Start0.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Disable_Geme_Start"),
		CDisable_Geme_Start::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//Animaion
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(0.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SpongeBob"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/SpongeBob/", "SpongeBob_Player.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_SpongeBob"),
		CSpongeBob::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(0.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Sandy_Head"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Boss_Sandy/", "Boss_Sandy_Head.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Sandy_Head"),
		CBoss_Sandy::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(0.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Sandy_Body"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Boss_Sandy/", "Boss_Sandy_Body.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Sandy_Body"),
		CBoss_Sandy_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(0.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Patrick"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Patrick/", "Patrick_Player.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Patrick"),
		CPatrick::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bubble"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Bubble/", "Bubble2.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Bubble"),
		CBubble::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_BossSandy_NonAnim"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Boss_Sandy/", "Boss_Sandy_NonAnim.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Mesh_noanimnsandy"),
		CBossSandy_NonAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Camera_DirectorMesh"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Camera/", "camera.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Mesh_camera"),
		CCamera_DirectorMesh::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_King_NonAnim"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/King_Neptune/", "King_Neptune_NonAnim.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Mesh_noanimnking"),
		CKing_NonAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SpongeBob_NonAnim"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/SpongeBob/", "SpongeBob.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Mesh_noanimnspongebob"),
		CSpongeBob_NonAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Patrick_NonAnim"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Patrick/", "Patrick_NonAnim.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Mesh_noanimnpatrick"),
		CPatrick_NonAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.012f, 0.012f, 0.012f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Field"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Jellifish/", "Field.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Field"),
		CField::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_Point_Instance.hlsl"), VTXPOINTINSTANCE_DECLARATION::Element, VTXPOINTINSTANCE_DECLARATION::iNumElement))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, 100))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxRectInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_Rect_Instance.hlsl"), VTXRECTINSTANCE_DECLARATION::Element, VTXRECTINSTANCE_DECLARATION::iNumElement))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Instance"),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pContext, 20))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Grass"),
		CGrassInstance::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Tree"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Tree/", "Tree.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	Safe_AddRef(m_pRenderer);

	return S_OK;
}


HRESULT CMainTool::Ready_Prototype_GameObject()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	return S_OK;
}

CMainTool * CMainTool::Create()
{
	CMainTool* pInstance = new CMainTool();

	if (FAILED(pInstance->NativeConstruct())) 
	{
		MSG_BOX("Failed to Created : CMainTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainTool::Free()
{
	__super::Free();
	Safe_Release(m_pRenderer);
	Safe_Release(m_pGameInstance);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pImgui_Manager);

	CImgui_Manager::Destroy_Instance();
	CGameInstance::Release_Engine();
}
