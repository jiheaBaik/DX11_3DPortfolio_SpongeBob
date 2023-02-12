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
//	GraphicDesc.isWindowed = true;
	
	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, g_hInst, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	/*if (FAILED(Ready_RenderState()))
		return E_FAIL;*/

	// Static 컴포넌트 프로토타입 추가
	if (FAILED(Ready_Prototype_Component_Static()))
		return E_FAIL;

	// Static 게임오브젝트 프로토타입 추가
	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	// imgui 매니저 초기화
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
#endif // _DEBUG

	m_pGameInstance->Tick_Engine(fTimeDelta);
}

HRESULT CMainTool::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;
	//m_pGameInstance->Clear_BackBuffer_View(_float4(0.5f, 0.5f, 0.5f, 1.f));

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.741f, 0.741f, 0.741f, 1.f));
	//
	//m_pGameInstance->Clear_BackBuffer_View(_float4(0.427f, 0.411f, 0.4f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pRenderer->Draw();
	
	m_pImgui_Manager->Frame();


	// 엔진 렌더링
	//m_pGameInstance->Render_Engine();

	// Imgui 렌더링
	//ImGui::ShowDemoWindow();
	
#ifdef _DEBUG
	++m_iNumRender;

	_tchar szFPS[MAX_PATH];
	if (m_fTimeAcc > 1.f) {
		wsprintf(szFPS, TEXT("FPS : %d \n"), m_iNumRender);
		SetWindowText(g_hWnd, szFPS);

		m_fTimeAcc = 0.f;
		m_iNumRender = 0;
	}
#endif // _DEBUG

	m_pImgui_Manager->Render();

	m_pGameInstance->Present();
	//m_pGameInstance->Render_Engine();
	return S_OK;
}

// Static 레벨의 컴포넌트 프로토타입
HRESULT CMainTool::Ready_Prototype_Component_Static()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElement))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElement))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElement))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Shader_VtxAnimModel */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Element, VTXANIMMODEL_DECLARATION::iNumElement))))
		return E_FAIL;


	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_CVIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	//terrainLevel
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"),
		CCube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../Client/Bin/Data/Navigation.dat")))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation2"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../Client/Bin/Data/Navigation2.dat")))))
		return E_FAIL;
	

	/* For.Prototype_Component_Model_Ground*/
	_matrix PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ground"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_Ground_01/", "SM_Ground_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ground */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Ground"),
		CGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Ground2*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.012f, 0.012f, 0.012f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ground2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_Ground_02/", "SM_Ground_02.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ground2 */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Ground2"),
		CGround2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	


	/* For.Prototype_Component_Model_Ring_Top*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring_Top"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_Ring_Top/", "SM_Ring_Top.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ring_Top */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Ring_Top"),
		CRing_Top::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//

	/* For.Prototype_Component_Model_Rope_Top*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rope_Top"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_PD_Rope_Top/", "SM_PD_Rope_Top_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rope_Top */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Rope_Top"),
		CRope_Top::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Ring_Pillar*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring_Pillar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Ring_Pillar/", "SM_PD_Ring_Pillar.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ring_Pillar */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Ring_Pillar"),
		CRing_Pillar::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Light_Tube*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Light_Tube"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Light_Tube/", "SM_PD_Light_Tube_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Light_Tube */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Light_Tube"),
		CLight_Tube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Scoreboard*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Scoreboard"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Scoreboard/", "SM_PD_Scoreboard_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Scoreboard */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Scoreboard"),
		CScoreboard::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_SM_PD_Scoreboard_Broken*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Scoreboard_Broken"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Scoreboard/", "SM_PD_Scoreboard_Broken.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_SM_PD_Scoreboard_Broken*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Mesh_scoreboard_broken"),
		CScoreboard_Broken::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Light_Tube*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Big_Pillar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Big_Pillar/", "SM_Big_Pillar_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Light_Tube */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Big_Pillar"),
		CBig_Pillar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Big_Pillar_Attachment*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Big_Pillar_Attachment"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Big_Pillar_Attachment/", "SM_Big_Pillar_Attachment_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Big_Pillar_Attachment */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Big_Pillar_Attachment"),
		CBig_Pillar_Attachment::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Big_Cloth*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Big_Cloth"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Big_Cloth/", "SM_big_cloth_01.props.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Big_Pillar_Attachment */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Big_Cloth"),
		CBig_Cloth::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Pos_Light*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Pos_Light"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Pos_Light/", "SM_Pos_Light.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Big_Pillar_Pos_Light */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Pos_Light"),
		CPos_Light::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Stairs_02*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Stairs_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Stairs_02/", "SM_Stairs_02.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Big_Pillar_Stairs_02 */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Stairs_02"),
		CStairs_02::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Stairs_04*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Stairs_04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Stairs_04/", "SM_Stairs_04.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Big_Pillar_Stairs_04 */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Stairs_04"),
		CStairs_04::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_ModelSM_PD_Banner_01*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_SM_PD_Banner_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_PD_Banner_01/", "SM_PD_Banner_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_SM_PD_Banner_01 */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_SM_PD_Banner_01"),
		CSM_PD_Banner_01::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_ModelSM_PD_Poster*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Poster"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Poster_01/", "SM_PD_Poster_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Poster */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Poster"),
		CPoster::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_ModelSM_PD_Poster2*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Poster2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Poster_02/", "SM_PD_Poster_02.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Poster2 */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Poster2"),
		CPoster2::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 200, 200))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Taxi*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(173.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Taxi"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Taxi/", "TaxiStop.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Taxi */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_TaxiStop"),
		CTaxi::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Taxi*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(173.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Gate"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Gate/", "Gate.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Taxi */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Gate"),
		CGate::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Robo*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Robo"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Robo/", "Robo.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObjectRobo */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Robo"),
		CRobo::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Robo2*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Robo2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Robo_2/", "Robo2.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObjectRobo */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Robo2"),
		CRobo2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Robo2*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Robo3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Robo_3/", "Robo3.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObjectRobo */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Robo3"),
		CRobo3::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Jelly_Pink*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Jelly_Pink"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Jelly/Pink/", "Jelly_Pink.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObjectJelly_Pink */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Jelly_Pink"),
		CJelly_Pink::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_JellyBlue*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Jelly_Blue"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Jelly/Blue/", "Jelly_Blue.fbx", PivotMatrix))))
		return E_FAIL;


	/* For.Prototype_GameObjectJelly_Pink */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Jelly_Blue"),
		CJelly_Blue::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_JellyBlue*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Squidward"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Npc/", "Squidward.fbx", PivotMatrix))))
		return E_FAIL;


	/* For.Prototype_GameObjectJelly_Pink */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Squidward"),
		CSquidward::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_JellyBlue*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Man"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Npc/", "Man.fbx", PivotMatrix))))
		return E_FAIL;


	/* For.Prototype_GameObjectJelly_Pink */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Man"),
		CMan::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_Component_Model_Fish_A*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_TERRAIN, TEXT("Prototype_Component_Model_Fish_A"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_A/", "Fish_A.fbx", PivotMatrix))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Fish_A */
	////if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_A"),
	////	CFish_A::Create(m_pDevice, m_pContext))))
	////	return E_FAIL;

	///* For.Prototype_Component_Model_Fish_B*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_TERRAIN, TEXT("Prototype_Component_Model_Fish_B"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_B/", "Fish_B.fbx", PivotMatrix))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Fish_B */
	////if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_B"),
	////	CFish_B::Create(m_pDevice, m_pContext))))
	////	return E_FAIL;

	///* For.Prototype_Component_Model_Fish_C*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_TERRAIN, TEXT("Prototype_Component_Model_Fish_C"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_C/", "Fish_C.fbx", PivotMatrix))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Fish_C */
	////if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_C"),
	////	CFish_C::Create(m_pDevice, m_pContext))))
	////	return E_FAIL;

	///* For.Prototype_Component_Model_Fish_D*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_TERRAIN, TEXT("Prototype_Component_Model_Fish_D"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_D/", "Fish_D.fbx", PivotMatrix))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Fish_D*/
	////if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_D"),
	////	CFish_D::Create(m_pDevice, m_pContext))))
	////	return E_FAIL;

	///* For.Prototype_Component_Model_Fish_E*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_TERRAIN, TEXT("Prototype_Component_Model_Fish_E"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_E/", "Fish_E.fbx", PivotMatrix))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Fish_E */
	////if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_E"),
	////	CFish_E::Create(m_pDevice, m_pContext))))
	////	return E_FAIL;

	///* For.Prototype_Component_Model_Fish_G*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_TERRAIN, TEXT("Prototype_Component_Model_Fish_G"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_G/", "Fish_G.fbx", PivotMatrix))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Fish_G */
	////if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_G"),
	////	CFish_G::Create(m_pDevice, m_pContext))))
	////	return E_FAIL;


	///* For.Prototype_Component_Model_Fish_H*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_TERRAIN, TEXT("Prototype_Component_Model_Fish_H"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_H/", "Fish_H.fbx", PivotMatrix))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Fish_H */
	////if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_H"),
	////	CFish_H::Create(m_pDevice, m_pContext))))
	////	return E_FAIL;

	///* For.Prototype_Component_Model_Fish_I*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_TERRAIN, TEXT("Prototype_Component_Model_Fish_I"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_I/", "Fish_I.fbx", PivotMatrix))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Fish_I */
	////if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_I"),
	////	CFish_I::Create(m_pDevice, m_pContext))))
	////	return E_FAIL;

	///* For.Prototype_Component_Model_Fish_J*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_TERRAIN, TEXT("Prototype_Component_Model_Fish_J"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_J/", "Fish_J.fbx", PivotMatrix))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Fish_J */
	////if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_J"),
	////	CFish_J::Create(m_pDevice, m_pContext))))
	////	return E_FAIL;

	///* For.Prototype_Component_Model_Fish_K*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_TERRAIN, TEXT("Prototype_Component_Model_Fish_K"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_K/", "Fish_K.fbx", PivotMatrix))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Fish_K */
	////if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_K"),
	////	CFish_K::Create(m_pDevice, m_pContext))))
	////	return E_FAIL;

	///* For.Prototype_Component_Model_Fish_L*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_TERRAIN, TEXT("Prototype_Component_Model_Fish_L"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_L/", "Fish_L.fbx", PivotMatrix))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Fish_L */
	////if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_L"),
	////	CFish_L::Create(m_pDevice, m_pContext))))
	////	return E_FAIL;


//	/* For.Prototype_Component_VIBuffer_Rect_Instance */
//if (FAILED((CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Instance"),
//	CVIBuffer_Rect_Instance::Create(m_pDevice, m_pContext, 20))))
//	return E_FAIL;


	/* For.Prototype_Component_Model_Light_Tube*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Scoreboard_Ropes"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Scoreboard_Ropes/", "Scoreboard_Ropes.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Light_Tube */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Scoreboard_Ropes"),
		CScoreboard_Ropes::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_CLight_Cone_Pink*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Light_Cone_Pink"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/LightCone/", "LightCone_Piink.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_CLight_Cone_Pink */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Light_Cone_Pink"),
		CLight_Cone_Pink::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_CLight_Cone_Blue*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Light_Cone_Blue"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/LightCone/", "LightCone_Blue.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_CLight_Cone_Pink_Blue */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Light_Cone_Blue"),
		CLight_Cone_Blue::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//
	/* For.Prototype_Component_Model_CLight_Cone_Blue*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Light_Cone_Blue2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/LightCon2/", "Cone2Blue.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_CLight_Cone_Pink_Blue */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Light_Cone_Blue2"),
		CLight_Cone_Blue2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_CLight_Cone_Blue*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Light_Cone_Yellow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/LightCon2/", "Cone2Yellow.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_CLight_Cone_Pink_Blue */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Light_Cone_Yellow"),
		CLight_Cone_Yellow::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//

	/* For.Prototype_Component_Model_CLight_Cone_Green*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Light_Cone_Green"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/LightCone/", "LightCone_Green.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_CLight_Cone_Green*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Light_Cone_Green"),
		CLight_Cone_Green::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Grass"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Model/Grass/Grass.dds"), 1))))
		return E_FAIL;

	///* For.Prototype_Component_Model_Boss_Sandy*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Sandy"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Boss_Sandy/", "Boss_Sandy_Head.fbx", PivotMatrix))))
	//	return E_FAIL;

	///* For.Prototype_GameObject Boss_Sandy */
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Sandy"),
	//	CBoss_Sandy::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	//UI
	//Underpants"
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Underpants"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Bubble_0.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_LUnderpants" */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Underpants"),
		CUnderpants::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_HPBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/LongBar/Boss_HPBar%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_GameObject_Boss_HPBar */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_HPBar"),
		CBoss_HPBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Energy"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/LongBar/boss_energy_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_GameObject_Boss_HP_Energy */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Energy"),
		CBoss_HP_Energy::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Boss_MenuBar */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Menu/MenuBar_01.dds"), 1))))
		return E_FAIL;

	
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_MenuBar"),
		CMenuBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Disable_Exit01*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Disable_Exit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Menu/Endable_Exit0.png"), 1))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Disable_Exit"),
		CDisable_Exit::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Disable_Geme_Start01 */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Disable_Geme_Start"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Menu/Endable_Geme_Start0.png"), 1))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Disable_Geme_Start"),
		CDisable_Geme_Start::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//Animaion
	/* For.Prototype_Component_Model_SpongeBob*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(0.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SpongeBob"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/SpongeBob/", "SpongeBob_Player.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_SpongeBob */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_SpongeBob"),
		CSpongeBob::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Boss_Sandy_Head*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(0.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Sandy_Head"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Boss_Sandy/", "Boss_Sandy_Head.fbx", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_GameObject Boss_Sandy_Head*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Sandy_Head"),
		CBoss_Sandy::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Boss_Sandy_Body*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(0.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Sandy_Body"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Boss_Sandy/", "Boss_Sandy_Body.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject Boss_Sandy_Body*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Sandy_Body"),
		CBoss_Sandy_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Patrick*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(0.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Patrick"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Patrick/", "Patrick_Player.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject Boss__Patrick*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Patrick"),
		CPatrick::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Bubble*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bubble"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Bubble/", "Bubble2.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bubble*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Bubble"),
		CBubble::Create(m_pDevice, m_pContext))))
		return E_FAIL;




	/* For.Prototype_Component_Model_Bubble*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_BossSandy_NonAnim"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Boss_Sandy/", "Boss_Sandy_NonAnim.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bubble*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Mesh_noanimnsandy"),
		CBossSandy_NonAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Bubble*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Camera_DirectorMesh"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Camera/", "camera.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bubble*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Mesh_camera"),
		CCamera_DirectorMesh::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Bubble*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_King_NonAnim"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/King_Neptune/", "King_Neptune_NonAnim.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bubble*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Mesh_noanimnking"),
		CKing_NonAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Bubble*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SpongeBob_NonAnim"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/SpongeBob/", "SpongeBob.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bubble*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Mesh_noanimnspongebob"),
		CSpongeBob_NonAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Bubble*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Patrick_NonAnim"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Patrick/", "Patrick_NonAnim.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bubble*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Mesh_noanimnpatrick"),
		CPatrick_NonAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/* For.Prototype_Component_Model_Field*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.012f, 0.012f, 0.012f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Field"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Jellifish/", "Field.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Field */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Field"),
		CField::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_Point_Instance.hlsl"), VTXPOINTINSTANCE_DECLARATION::Element, VTXPOINTINSTANCE_DECLARATION::iNumElement))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Point_Instance */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, 100))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxRectInstance*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxRectInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_Rect_Instance.hlsl"), VTXRECTINSTANCE_DECLARATION::Element, VTXRECTINSTANCE_DECLARATION::iNumElement))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect_Instance */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Instance"),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pContext, 20))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Grass"),
		CGrassInstance::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Light_Tube*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Tree"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Tree/", "Tree.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Light_Tube */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Grass"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Grass/", "Grass.fbx", PivotMatrix))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Field */
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Grass"),
	//	CGrass::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	Safe_AddRef(m_pRenderer);

	return S_OK;
}


// 모든 레벨의 게임 오브젝트 프로토타입
HRESULT CMainTool::Ready_Prototype_GameObject()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;
	

	///* Prototype_GameObject_Camera_Free */
	//if (FAILED(m_pGameInstance->Add_ObjectPrototype(L"Prototype_GameObject_Camera_Free", CCamera_Free::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	///* Prototype_GameObject_BackGround */
	//if (FAILED(m_pGameInstance->Add_ObjectPrototype(L"Prototype_GameObject_BackGround", CBackGround::Create(m_pGraphic_Device))))
	//	return E_FAIL;


	///* Prototype_GameObject_Weapon */
	//if (FAILED(m_pGameInstance->Add_ObjectPrototype(L"Prototype_GameObject_Weapon", CWeapon::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	////2022.05.28 jhBaik : 터레인 추가
	///* For.Prototype_GameObject_Terrain */
	//if (FAILED(m_pGameInstance->Add_ObjectPrototype(L"Prototype_GameObject_Terrain", CTerrain::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	////2022.05.28 jhBaik : 벽 추가
	///* Prototype_GameObject_BackGround */
	//if (FAILED(m_pGameInstance->Add_ObjectPrototype(L"Prototype_GameObject_Wall", CWall::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	//// 2022.05.28 jhBaik : 큐브 추가
	///* For.Prototype_GameObject_Cube */
	//if (FAILED(m_pGameInstance->Add_ObjectPrototype(L"Prototype_GameObject_Cube", CCube::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	////2022.06.06 jhBaik : 몬스터 추가
	///* Prototype_GameObject_BackGround */
	//if (FAILED(m_pGameInstance->Add_ObjectPrototype(L"Prototype_GameObject_Monster", CMonsters::Create(m_pGraphic_Device))))
	//	return E_FAIL;

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
