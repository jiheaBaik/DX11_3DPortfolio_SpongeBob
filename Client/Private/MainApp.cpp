#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "BackGround.h"
#include "MyMenu.h"
#include "Logo.h"
#include "Camera_Free.h"
#include "Player.h"

#include "SpongeBob_Foot_L.h"
#include "SpongeBob_Wand.h"
#include "Taxi.h"

#include "BubbleUI.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::NativeConstruct()
{
	GRAPHICDESC			GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.eWindowed = GRAPHICDESC::MODE_WIN;
	GraphicDesc.iWinCX = g_iWinSizeX;
	GraphicDesc.iWinCY = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, g_hInst, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	
	if (FAILED(Ready_Fonts()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Components()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_double TimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

#ifdef _DEBUG
	m_TimeAcc += TimeDelta;
#endif // _DEBUG

	m_pGameInstance->Tick_Engine(TimeDelta);
}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pRenderer->Draw();



	CTaxi* pCTaxi = static_cast<CTaxi*>(m_pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Taxi")));
	if (nullptr != pCTaxi)
	{
		_uint count  = pCTaxi->Get_FlowerCount();
		wsprintf(m_szFlowerCount, TEXT(" %d"), count);
		m_pGameInstance->Render_Font(TEXT("Arial"), m_szFlowerCount, _float2(1030, 44), XMVectorSet(1.f, 1.f, 1.f, 1.f), 1.5f);
		m_pGameInstance->Render_Font(TEXT("Arial"), m_szFlowerCount, _float2(1030, 44), XMVectorSet(1.f, 1.f, 1.f, 1.f), 1.5f);

		//m_pGameInstance->Render_Font(TEXT("Arial"), m_szFlowerCount, _float2(1049, 57), XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.68f);
		//m_pGameInstance->Render_Font(TEXT("Arial"), m_szFlowerCount, _float2(1039, 50), XMVectorSet(1.f, 1.f, 1.f, 1.f), 1.1f);

	}
//#ifdef _DEBUG
//	++m_iNumRender;
//
//	if (m_TimeAcc >= 1.0)
//	{
//		wsprintf(m_szFPS, TEXT("에프피에스 : %d"), m_iNumRender);
//
//		m_iNumRender = 0;
//
//		m_TimeAcc = 0.0;
//	}
//
//
//
//	// MakeSpriteFont "폰트이름" /FontSize:32 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 출력파일이름.spritefont
//	m_pGameInstance->Render_Font(TEXT("Font_Bazzi"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
//#endif // _DEBUG
//


	m_pGameInstance->Render_Engine();

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevel)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLevel))))
		return E_FAIL;


	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Components()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;



	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), 
		m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElement))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), 
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//쉐이더
	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElement))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElement))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Element, VTXANIMMODEL_DECLARATION::iNumElement))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_Point_Instance.hlsl"), VTXPOINTINSTANCE_DECLARATION::Element, VTXPOINTINSTANCE_DECLARATION::iNumElement))))
		return E_FAIL;
	///* For.Prototype_Component_Shader_VtxPointInstance*/
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_Rect_Instance.hlsl"), VTXPOINTINSTANCE_DECLARATION::Element, VTXPOINTINSTANCE_DECLARATION::iNumElement))))
	//	return E_FAIL;


	//Underpants
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Underpants"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Bubble_%d.dds"), 2))))
		return E_FAIL;
	//Fish
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Fish"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Fish/img%d.png"), 2))))
		return E_FAIL;


	// Boss_HPBar
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_HPBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/LongBar/Boss_HPBar%d.dds"), 2))))
		return E_FAIL;

	if(FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Logo_01.jpg"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loding"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Loding_01.jpg"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Menu"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Menu_01.jpg"), 1))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BubbleUI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Bubble_04.png"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_Boss_HPBar */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_BubbleUI"),
		CBubbleUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	//energy
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Energy"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/LongBar/boss_energy_%d.png"), 3))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_Effect */
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SandySmokeEffect"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/SandySmokeEffect/Smoke_%d.png"), 66))))
	//	return E_FAIL;

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Info"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/Info/img0.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SandySmokeEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/SandySmokeEffect/img%d.png"), 5))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/SmokeEffect/Smoke_%d.png"), 66))))
		return E_FAIL;



	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeEffect2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/SmokeEffect2/img%d.png"), 33))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Electirc"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/Electric/img%d.png"),5))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BubblePang"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/BubblePang/img%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Pang"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/Pang/img%d.png"), 23))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GrowBlue"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/GrowBlue/T_Blue_Glow_00.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GrowBlue2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/GrowBlue/T_Blue_Glow2.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Ray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/Ray/T_Ray_Velo_01.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EndPang"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/End_Pang/img.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Black"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/Black/Black.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bubble"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/Bubble/Bubble_05.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Flower_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/Flower/Flower_01.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Flower02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/Flower/Flower_02.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect_Poof */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Poof"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/Poof/img%d.png"), 31))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect_Twinkle */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Twinkle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/Twinkle/img%d.png"),31))))
		return E_FAIL;

	//Slame Snady
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SandyBoss_Slame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/BossSandy/t_sandy_slam_icon.dds"), 1))))
		return E_FAIL;

	//Slame Player
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Slame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Player/stomp_icon_%d.dds"), 2))))
		return E_FAIL;

	//wsprintf(m_szLoading, TEXT("로고용 모델를 로딩 중입니다."));
	//

	//wsprintf(m_szLoading, TEXT("로고용 객체원형 들을 생성 중입니다."));

	///* For.Prototype_GameObject_BackGround*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Logo"),
		CLogo::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	///* For.Prototype_GameObject_BackGround*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Loding"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;	///* For.Prototype_GameObject_BackGround*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Menu"),
		CMyMenu::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Camera_Free*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;


	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	_matrix	PivotMatrix = XMMatrixIdentity();

	/* For.Prototype_Component_Model_Player_SpongeBob */
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)* XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Melody"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/SpongeBob/", "SpongeBob_Player.fbx", PivotMatrix))))
		return E_FAIL;


	/* For.Prototype_GameObject_Player */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_SpongeBob_Foot_L */
	PivotMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f)* XMMatrixRotationY(XMConvertToRadians(176.f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SpongeBobFoot_L"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/SpongeBob/", "SpongeBobFoot.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_SpongeBob_Foot_L */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_SpongeBob_Foot_L"),
		CSpongeBob_Foot_L::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_SpongeBob_Wand */
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)* XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SpongeBobWand"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/SpongeBob/", "SpongeBobWand.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_SpongeBob_Wand */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_SpongeBob_Wand"),
		CSpongeBob_Wand::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	
	/* For.Prototype_Component_VIBuffer_Point_Instance */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, 600))))
		return E_FAIL;


	//Disolve
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Disolve"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Model/Jelly/Pink/Disolve.png"), 1))))
		return E_FAIL;


	//Disolve
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TakeCut"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Cut/img%d.dds"),8))))
		return E_FAIL;

	//Disolve
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TakeJellyCut"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/JellyCut/img%d.dds"), 2))))
		return E_FAIL;


	//Disolve
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UnderPang"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/UnderPang/img%d.png"),6))))
		return E_FAIL;
	

	///* For.Prototype_Component_Texture_Effect */
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SandySmokeEffect"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Effect/SandySmokeEffect/img%d.png"), 5))))
	//	return E_FAIL;
	Safe_AddRef(m_pRenderer);

	return S_OK;
}

HRESULT CMainApp::Ready_Gara()
{
	_ulong		dwByte = 0;
	HANDLE		hFile = CreateFile(TEXT("../Bin/Data/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3			vPoints[3];

	ZeroMemory(vPoints, sizeof(_float3));
	vPoints[0] = _float3(0.0f, 0.f, 0.f);
	vPoints[1] = _float3(0.f, 0.f, 10.f);
	vPoints[2] = _float3(10.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoints, sizeof(_float3));
	vPoints[0] = _float3(0.0f, 0.f, 10.f);
	vPoints[1] = _float3(10.f, 0.f, 10.f);
	vPoints[2] = _float3(10.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoints, sizeof(_float3));
	vPoints[0] = _float3(0.0f, 0.f, 20.f);
	vPoints[1] = _float3(10.f, 0.f, 10.f);
	vPoints[2] = _float3(00.f, 0.f, 10.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	ZeroMemory(vPoints, sizeof(_float3));
	vPoints[0] = _float3(10.0f, 0.f, 10.f);
	vPoints[1] = _float3(20.f, 0.f, 00.f);
	vPoints[2] = _float3(10.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CMainApp::Ready_Fonts()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Arial"), TEXT("../Bin/Resources/Fonts/Arial.spritefont"))))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	// D3D11_SAMPLER_DESC
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();
}

