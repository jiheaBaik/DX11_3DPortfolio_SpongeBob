#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Camera_Free.h"
#include "Ring_Top.h"



//#include "Monster.h"
#include "Terrain.h"

#include "Patrick_Player.h"


#include "Ground.h"
//#include "SpongeBob.h"
#include "Ring_Pillar.h"
#include "Rope_Top.h"
#include "Light_Tube.h"
#include "Scoreboard.h"
#include "Big_Pillar.h"
#include "Big_Pillar_Attachment.h"
#include "King_Neptune.h"
#include "Big_Cloth.h"
#include "Ground2.h"
#include "Pos_Light.h"
#include "Stairs_02.h"
#include "Stairs_04.h"
#include "SM_PD_Banner_01.h"
#include "Poster.h"
#include "Poster2.h"
#include "Fish_A.h"
#include "Fish_B.h"
#include "Fish_B2.h"

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
#include "Rope_Top_Broekn.h"


#include "Underpants.h"
#include "Fish.h"


#include "Boss_HPBar.h"
#include "MenuBar.h"
#include "Disable_Exit.h"
#include "Disable_Geme_Start.h"

#include "Boss_Sandy.h"
#include "Boss_Sandy_Head.h"
#include "Boss_Sandy_Body.h"

#include "Menu_SpongeBob.h"

#include "Sandy_Slame.h"

#include "Player_Slame.h"

#include "Boss_Sandy_Head_Glass.h"



#include "SkyBox_Poeisdom.h"
#include "Boss_HP_Energy.h"

#include "Light_Cone_Pink.h"
#include "Light_Cone_Blue.h"
#include "Light_Cone_Green.h"

#include "Bubble.h"
#include "Scoreboard_Broken.h"

#include "Boss_Sandy_Head_Glass2.h"
#include "Boss_Sandy_Head2.h"
#include "SmokeEffect.h"
#include "SmokeEffect2.h"
#include "Electric.h"
#include "BubblePangEffect.h"
#include "GlowBlue.h"

#include "GlowBlue2.h"
#include "Effect_Point.h"
#include "SnadySmokeEffect.h"
#include "Flame.h"
#include "PangEffect.h"
#include "EndPangEffect_Point.h"

#include "BlackBackGround.h"
#include "Field.h"
#include "Jelly_Pink.h"
#include "Jelly_Blue.h"
#include "JellyGround.h"
#include "Robo.h"
#include "Robo_2.h"
#include "Robo_3.h"
#include "Taxi.h"
#include "Flower.h"
#include "BubbleEffect.h"
#include "Flower_UI.h"
#include "Sky.h"
#include "TaxiStop.h"
#include "FlowerEffect.h"
#include "Gate.h"
#include "PoofEffect.h"
#include "TwinkleEffect.h"
#include "Squidward.h"
#include "Man.h"
#include "Tree.h"
#include "Cut.h"
#include "JellyCut.h"

#include "RainBow.h"
#include "InfoEffect.h"
#include "UnderPangEffect.h"


//#includMane "Effect.h"
//#include "Sky.h"
//#include "UI_Default.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}


/*
typedef unsigned (__stdcall  *_beginthreadex_proc_type)(void*);
*/

unsigned int APIENTRY ThreadEntryFunc(void* pArg)
{


	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(pLoader->Get_CriticalSection());

	/* 자원로드할꺼야. */
	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_MENU:
		pLoader->Loading_ForMenuLevel();
		break;
	case LEVEL_FIELD:
		pLoader->Loading_ForFieldLevel();
		break;
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlayLevel();
		break;
	}

	LeaveCriticalSection(pLoader->Get_CriticalSection());

	return 0;
}




HRESULT CLoader::NativeConstruct(LEVEL eNextLevel)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	//_uint textuerNum = 0;
	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_LOGO, L"Layer_BackGround", TEXT("Prototype_GameObject_BackGround"),&textuerNum/*, &CGameObject::GAMEOBJECTDESC(5.f, XMConvertToRadians(90.0f))*/)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	int a = 10;

	lstrcpy(m_szLoading, TEXT("로고용텍스쳐를 로딩 중입니다"));

	// wsprintf(m_szLoading, TEXT("로고용 텍스쳐를 로딩 중입니다."));
	/* For.Prototype_Component_Texture_Logo */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
	//	return E_FAIL;

	////wsprintf(m_szLoading, TEXT("로고용 모델를 로딩 중입니다."));
	////

	////wsprintf(m_szLoading, TEXT("로고용 객체원형 들을 생성 중입니다."));

	/////* For.Prototype_GameObject_BackGround*/
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
	//	CBackGround::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;


	//wsprintf(m_szLoading, TEXT("로딩이 완료되었습니다. \n"));
	//
	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForFieldLevel()
{
	_matrix	PivotMatrix = XMMatrixIdentity();

	lstrcpy(m_szLoading, TEXT("네비게이션 로딩 중입니다."));
	/* For.Prototype_Component_Navigation */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation2"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Data/Navigation2.dat")))))
		return E_FAIL;



	/* For.Prototype_Component_Model_Field*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.012f, 0.012f, 0.012f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Model_Field"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Jellifish/", "Field.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Field */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Field"),
		CField::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Ground*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.12f, 0.012f, 0.12f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_ModelJellyGround"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Jellifish/", "Ground.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ground */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_JellyGround"),
		CJellyGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Jelly_Pink*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Model_Jelly_Pink"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Jelly/Pink/", "Jelly_Pink.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObjectJelly_Pink */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Jelly_Pink"),
		CJelly_Pink::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_JellyBlue*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Model_Jelly_Blue"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Jelly/Blue/", "Jelly_Blue.fbx", PivotMatrix))))
		return E_FAIL;
		

	/* For.Prototype_GameObjectJelly_Pink */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Jelly_Blue"),
		CJelly_Blue::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/* For.Prototype_Component_Model_Robo*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Model_Robo"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Robo/", "Robo.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObjectRobo */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Robo"),
		CRobo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Robo2*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Model_Robo2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Robo_2/", "Robo2.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObjectRobo */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Robo2"),
		CRobo_2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Robo2*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Model_Robo3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Robo_3/", "Robo3.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObjectRobo */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Robo3"),
		CRobo_3::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Taxi*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(-7.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Model_Taxi"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Taxi/", "Taxi.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Taxi */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Taxi"),
		CTaxi::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_TaxiStop*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(173.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Model_TaxiStop"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Taxi/", "TaxiStop.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_TaxiStop */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_TaxiStop"),
		CTaxiStop::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Flower*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Model_Flower"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Flower/", "Flower.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Flower*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Flower"),
		CFlower::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Effect_Point */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Effect_Bubble"),
		CBubbleEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Point */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Flower_UI"),
		CFlower_UI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_SkyBox_Poeisdom*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(0));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Model_Sky"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Sky/", "SkyBox.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_SkyBox_Poeisdom */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	
	/* For.Prototype_GameObject_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_EndPangEffect"),
		CEndPangEffect_Point::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_FlowerEffect"),
		CFlowerEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Info"),
		CInfoEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Taxi*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(173.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Model_Gate"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Gate/", "Gate.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Taxi */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Gate"),
		CGate::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_RainBow*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(173.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RainBow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/RainBow/", "RainBow.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Taxi */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_RainBow"),
		CRainBow::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	

	/* For.Prototype_GameObject_Underpants" */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Underpants"),
		CUnderpants::Create(m_pDevice, m_pContext))))
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

	/* For.Prototype_GameObject_Effect_Point */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Effect_PoofEffect"),
		CPoofEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Point */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Effect_TwinkleEffect"),
		CTwinkleEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Model_Squidward"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Npc/", "Squidward.fbx", PivotMatrix))))
		return E_FAIL;


	/* For.Prototype_GameObjectJelly_Pink */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Squidward"),
		CSquidward::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.013f, 0.013f, 0.013f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Model_Man"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Npc/", "Man.fbx", PivotMatrix))))
		return E_FAIL;


	/* For.Prototype_GameObjectJelly_Pink */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Man"),
		CMan::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.013f, 0.013f, 0.013f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Model_Tree"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Tree/", "Tree.fbx", PivotMatrix))))
		return E_FAIL;


	/* For.Prototype_GameObjectJelly_Pink */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Underpants" */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_JellyCut"),
		CJellyCut::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_Component_Model_Bubble*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATICK, TEXT("Prototype_Component_Model_Bubble"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Bubble/", "Bubble2.fbx", PivotMatrix))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Bubble*/
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Bubble"),
	//	CBubble::Create(m_pDevice, m_pContext))))

	//	return E_FAIL;

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{


	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	lstrcpy(m_szLoading, TEXT("셰이더 로딩 중입니다."));
	///* For.Prototype_GameObject_Camera_Free*/
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
	//	CCamera_Free::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	//lstrcpy(m_szLoading, TEXT("셰이더 로딩 중입니다."));
	///* For.Prototype_Component_VIBuffer_Point_Instance */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
	//	CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, 600))))
	//	return E_FAIL;
	///* For.Prototype_Component_VIBuffer_Point_Instance */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
	//	CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, 600))))
	//	return E_FAIL;

	///* For.Prototype_Component_VIBuffer_Point_Instance */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
	//	CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, 1))))
	//	return E_FAIL;
	///* For.Prototype_Component_Shader_VtxNorTex */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElement))))
	//	return E_FAIL;

	///* For.Prototype_Component_Shader_VtxModel */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElement))))
	//	return E_FAIL;

	///* For.Prototype_Component_Shader_VtxAnimModel */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Element, VTXANIMMODEL_DECLARATION::iNumElement))))
	//	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("게임플레이용 텍스쳐를 로딩 중입니다."));

	///* For.Prototype_Component_Texture_Terrain */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.dds"), 2))))
	//	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("네비게이션 로딩 중입니다."));
	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Data/Navigation.dat")))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Filter"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.bmp"), 1))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_Brush */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
	//	return E_FAIL;

	

	/* For.Prototype_GameObject_Effect_Point */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Effect_Point"),
		CEffect_Point::Create(m_pDevice, m_pContext))))
		return E_FAIL;




	///* For.Prototype_Component_Texture_Explosion */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Explosion"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Explosion/Explosion%d.png"), 90))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_Sky */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	//	return E_FAIL;
	//
	lstrcpy(m_szLoading, TEXT("게임플레이용 모델를 로딩 중입니다."));

	///* For.Prototype_Component_VIBuffer_Terrain */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
	//	CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
	//	return E_FAIL;
	//_matrix		PivotMatrix = XMMatrixIdentity();

	///* For.Prototype_Component_Model_Melody */
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)* XMMatrixRotationY(XMConvertToRadians(-90.0f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Melody"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Melody/", "Melody.fbx", PivotMatrix))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Player */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
	//	CPlayer::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;


	/* For.Prototype_Component_Model_Ring_Top*/
	_matrix	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ring_Top"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_Ring_Top/", "SM_Ring_Top.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ring_Top */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Ring_Top"),
		CRing_Top::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	///* For.Prototype_Component_Model_SpongeBob*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SpongeBob"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/SpongeBob/", "SpongeBob.fbx", PivotMatrix))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_SpongeBob */
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_SpongeBob"),
	//	CSpongeBob::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_Component_Model_Ground*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ground"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_Ground_01/", "SM_Ground_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ground */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Ground"),
		CGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Ground2*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ground2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_Ground_02/", "SM_Ground_02.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ground2 */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Ground2"),
		CGround2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Rope_Top*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.008f, 0.008f, 0.008f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rope_Top"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/SM_PD_Rope_Top/", "SM_PD_Rope_Top_02.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rope_Top */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Rope_Top"),
		CRope_Top::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_SM_PD_Rope_Top_Broken*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.008f, 0.008f, 0.008f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rope_Top_Broken"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_PD_Rope_Top/", "SM_PD_Rope_Top_Broken.fbx", PivotMatrix))))
		return E_FAIL;


	/* For.Prototype_GameObject_Rope_Top */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Rope_Top_Broekn"),
		CRope_Top_Broekn::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/* For.Prototype_Component_Model_Ring_Pillar*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ring_Pillar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Ring_Pillar/", "SM_PD_Ring_Pillar.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ring_Pillar */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Ring_Pillar"),
		CRing_Pillar::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Light_Tube*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Light_Tube"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Light_Tube/", "SM_PD_Light_Tube_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Light_Tube */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Light_Tube"),
		CLight_Tube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Light_Tube*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Scoreboard"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Scoreboard/", "SM_PD_Scoreboard_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Light_Tube */
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



	/* For.Prototype_Component_Model_Big_Pillar*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Big_Pillar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Big_Pillar/", "SM_Big_Pillar_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Big_Pillar */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Big_Pillar"),
		CBig_Pillar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Big_Pillar_Attachment*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Big_Pillar_Attachment"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Big_Pillar_Attachment/", "SM_Big_Pillar_Attachment_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Big_Pillar_Attachment */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Big_Pillar_Attachment"),
		CBig_Pillar_Attachment::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_CKing_Neptune*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_King_Neptune"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/King_Neptune/", "King_Neptune.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObjectCKing_Neptune */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_King_Neptune"),
		CKing_Neptune::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Big_Cloth*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Big_Cloth"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Big_Cloth/", "SM_big_cloth_01.props.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Big_Cloth */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Big_Cloth"),
		CBig_Cloth::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Pos_Light*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Pos_Light"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Pos_Light/", "SM_Pos_Light.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Pos_Light*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Pos_Light"),
		CPos_Light::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	//
	/* For.Prototype_Component_Model_Stairs_02*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stairs_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Stairs_02/", "SM_Stairs_02.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Stairs_02*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Stairs_02"),
		CStairs_02::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//
	/* For.Prototype_Component_Model_Stairs_04*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stairs_04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Stairs_04/", "SM_Stairs_04.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Stairs_04*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Stairs_04"),
		CStairs_04::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_SM_PD_Banner_01*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_PD_Banner_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SM_PD_Banner_01/", "SM_PD_Banner_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_SM_PD_Banner_01*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_SM_PD_Banner_01"),
		CSM_PD_Banner_01::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Poster_01*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Poster"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Poster_01/", "SM_PD_Poster_01.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Poster_01*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Poster"),
		CPoster::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Poster_02*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Poster2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Poster_02/", "SM_PD_Poster_02.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Poster_02*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Poster2"),
		CPoster2::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Poster_02*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Boss_Sandy_Head_Glass"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Boss_Sandy/", "Boss_Sandy_Head_Glass.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Poster_02*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Sandy_Head_Glass"),
		CBoss_Sandy_Head_Glass::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Poster_02*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Boss_Sandy_Head_Glass2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Boss_Sandy/", "Boss_Sandy_Head_Glass.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Poster_02*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Sandy_Head_Glass2"),
		CBoss_Sandy_Head_Glass2::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Poster_02*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Boss_Sandy_Flame"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Flame/", "Flame.fbx", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_GameObjectFlame*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Flame"),
		CFlame::Create(m_pDevice, m_pContext))))
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

	/* For.Prototype_Component_Model_SkyBox_Poeisdom*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(1,1,1) * XMMatrixRotationY(XMConvertToRadians(0));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SkyBox_Poeisdom"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/SkyBox/", "SkyBox.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_SkyBox_Poeisdom */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_SkyBox_Poeisdom"),
		CSkyBox_Poeisdom::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Fish_A*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fish_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_A/", "Fish_A.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fish_A */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_A"),
		CFish_A::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Fish_B*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fish_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_B/", "Fish_B.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fish_B */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_B"),
		CFish_B::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_Component_Model_Fish_B*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fish_B2"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_B/", "Fish_B2.fbx", PivotMatrix))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Fish_B */
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_B2"),
	//	CFish_B2::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_Component_Model_Fish_C*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fish_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_C/", "Fish_C.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fish_C */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_C"),
		CFish_C::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Fish_D*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fish_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_D/", "Fish_D.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fish_D*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_D"),
		CFish_D::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Fish_E*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fish_E"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_E/", "Fish_E.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fish_E */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_E"),
		CFish_E::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Fish_G*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fish_G"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_G/", "Fish_G.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fish_G */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_G"),
		CFish_G::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Fish_H*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fish_H"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_H/", "Fish_H.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fish_H */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_H"),
		CFish_H::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Fish_I*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fish_I"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_I/", "Fish_I.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fish_I */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_I"),
		CFish_I::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_Component_Model_Fish_J*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fish_J"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_J/", "Fish_J.fbx", PivotMatrix))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Fish_J */
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_J"),
	//	CFish_J::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_Component_Model_Fish_K*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fish_K"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_K/", "Fish_K.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fish_K */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_K"),
		CFish_K::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Fish_L*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fish_L"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Fish_L/", "Fish_L.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fish_L */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish_L"),
		CFish_L::Create(m_pDevice, m_pContext))))
		return E_FAIL; 


	/* For.Prototype_Component_Model_Scoreboard_Ropes*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Scoreboard_Ropes"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Poseidome/Scoreboard_Ropes/", "Scoreboard_Ropes.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Scoreboard_Ropes*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Scoreboard_Ropes"),
		CScoreboard_Ropes::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	

	///* For.Prototype_GameObject_Underpants" */
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Underpants"),
	//	CUnderpants::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;


	/* For.Prototype_GameObject_Underpants" */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Fish"),
		CFish::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Boss_HPBar */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_HPBar"),
		CBoss_HPBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Underpants" */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Cut"),
		CCut::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_Component_Model_Bubble*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bubble"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Model/Bubble/", "Bubble2.fbx", PivotMatrix))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Bubble*/
	////if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Bubble"),
	////	CBubble::Create(m_pDevice, m_pContext))))
	////	return E_FAIL;
	
	///* For.Prototype_Component_Model_ForkLift*/
	//PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/ForkLift/", "ForkLift.fbx", PivotMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Fiona*/
	//PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Fiona/", "Fiona.fbx", PivotMatrix))))
	//	return E_FAIL;

	


	///* For.Prototype_Component_VIBuffer_Cube */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
	//	CVIBuffer_Cube::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	//lstrcpy(m_szLoading, TEXT("콜라이더 로딩 중입니다."));
	///* For.Prototype_Component_Collider_AABB */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
	//	CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
	//	return E_FAIL;


	



	lstrcpy(m_szLoading, TEXT("게임플레이용 객체들을 생성 중입니다."));

	///* For.Prototype_GameObject_Terrain */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
	//	CTerrain::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_ForkLift */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ForkLift"),
	//	CForkLift::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	

	


	

	/* For.Prototype_Component_Model_Player_Patrick */
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)* XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Patrick_Player"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Patrick/", "Patrick_Player.fbx", PivotMatrix))))
		return E_FAIL;
	


		/* For.Prototype_GameObject_Player */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Patrick_Player"),
			CPatrick_Player::Create(m_pDevice, m_pContext))))
			return E_FAIL;

			/* For.Prototype_Component_Model_Player_SpongeBob */
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)* XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Melody"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/SpongeBob/", "SpongeBob_Player.fbx", PivotMatrix))))
		return E_FAIL;


	///*  For.Prototype_GameObject Boss_Sandy */
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Sandy"),
	//	CBoss_Sandy::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_Component_Model_Boss_Sandy_Body*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Sandy_Body"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Boss_Sandy/", "Boss_Sandy_Body.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject Boss_Sandy_Body*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Sandy_Body"),
		CBoss_Sandy_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Boss_Sandy_Head*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Sandy_Head"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Boss_Sandy/", "Boss_Sandy_Head.fbx", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_GameObject Boss_Sandy_Head*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Sandy_Head"),
		CBoss_Sandy_Head::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/* For.Prototype_Component_Model_Boss_Sandy_Head*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Sandy_Head2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Poseidome/Boss_Sandy/", "Boss_Sandy_Head2.fbx", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_GameObject Boss_Sandy_Head*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Sandy_Head2"),
		CBoss_Sandy_Head2::Create(m_pDevice, m_pContext))))
		return E_FAIL;




	/* For.Prototype_GameObject Boss_Sandy_Slame*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Sandy_Slame"),
		CSandy_Slame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject Boss_Player_Slame*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Player_Slame"),
		CPlayer_Slame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Boss_HP_Energy */
		if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Boss_Energy"),
			CBoss_HP_Energy::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/*if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_MENU, TEXT("Prototype_Component_Texture_MenuBar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Menu/MenuBar%d.dds"), 3))))
			return E_FAIL;*/



		//	/* For.Prototype_Component_VIBuffer_Point_Instance */
		//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
		//	CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, 1))))
		//	return E_FAIL;

		///* For.Prototype_Component_Shader_VtxPointInstance*/
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_Point_Instance.hlsl"), VTXPOINTINSTANCE_DECLARATION::Element, VTXPOINTINSTANCE_DECLARATION::iNumElement))))
		//	return E_FAIL;


		/* For.Prototype_GameObject_Effect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SmokeEffect"),
			CSmokeEffect::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Effect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SmokeEffec2t"),
			CSmokeEffect2::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Effect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Electric"),
			CElectric::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		

		/* For.Prototype_GameObject_Effect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BubblePangEffect"),
			CBubblePangEffect::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Effect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PangEffect"),
			CPangEffect::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Effect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UnderPangEffect"),
			CUnderPangEffect::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		///* For.Prototype_GameObject_Effect */
		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EndPangEffect"),
		//	CEndPangEffect_Point::Create(m_pDevice, m_pContext))))
		//	return E_FAIL;



		/* For.Prototype_GameObject_Effect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CGlowBlue"),
			CGlowBlue::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Effect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CGlowBlue2"),
			CGlowBlue2::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Effect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CSnadySmokeEffect"),
			CSnadySmokeEffect::Create(m_pDevice, m_pContext))))
			return E_FAIL;


		/* For.Prototype_GameObject_Effect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BlackBackGround"),
			CBlackBackGround::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		
	///* For.Prototype_GameObject_Monster */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
	//	CMonster::Create(m_pDevi`ce, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Effect */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
	//	CEffect::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Sky */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
	//	CSky::Create(m_pGraphic_Device))))
	//	return E_FAIL;


	///* For.Prototype_GameObject_UI */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI"),
	//	CUI_Default::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("게임플레이용 완료되었습니다."));


	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForMenuLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	

	Safe_AddRef(pGameInstance);
	
	///* For.Prototype_Component_Shader_VtxNorTex */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_MENU, TEXT("Prototype_Component_Shader_VtxNorTex"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElement))))
	//	return E_FAIL;

	///* For.Prototype_Component_Shader_VtxModel */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_MENU, TEXT("Prototype_Component_Shader_VtxModel"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElement))))
	//	return E_FAIL;

	///* For.Prototype_Component_Shader_VtxAnimModel */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_MENU, TEXT("Prototype_Component_Shader_VtxAnimModel"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Element, VTXANIMMODEL_DECLARATION::iNumElement))))
	//	return E_FAIL;

	lstrcpy(m_szLoading, TEXT("로고용텍스쳐를 로딩 중입니다"));
	/* For.Prototype_GameObject_Boss_MenuBar */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_MENU, TEXT("Prototype_Component_Texture_MenuBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Menu/MenuBar%d.dds"), 3))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_MenuBar"),
		CMenuBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	
	/* For.Prototype_Component_Model_스폰지밥*/
	_matrix PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(-5));
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Menu_SpongeBob"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Model/Patrick/", "Menu_Patrick.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fish_H */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Menu_SpongeBob"),
		CMenu_SpongeBob::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	
	///* For.Prototype_GameObject_Disable_Exit01*/
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Disable_Exit"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Menu/Endable_Exit%d.png"), 2))))
	//	return E_FAIL;


	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Disable_Exit"),
	//	CDisable_Exit::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Disable_Geme_Start01 */
	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Disable_Geme_Start"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/UI/Menu/Endable_Geme_Start%d.png"),2))))
	//	return E_FAIL;


	//if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Disable_Geme_Start"),
	//	CDisable_Geme_Start::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	 wsprintf(m_szLoading, TEXT("로고용 텍스쳐를 로딩 중입니다."));
	///* For.Prototype_Component_Texture_Logo */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Menu"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Menu_01.png"), 1))))
	//	return E_FAIL;

	//wsprintf(m_szLoading, TEXT("로고용 모델를 로딩 중입니다."));
	//

	//wsprintf(m_szLoading, TEXT("로고용 객체원형 들을 생성 중입니다."));

	///* For.Prototype_GameObject_BackGround*/
	/*if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;*/


	wsprintf(m_szLoading, TEXT("로딩이 완료되었습니다. \n"));
	

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	DeleteCriticalSection(&m_CriticalSection);

	CloseHandle(m_hThread);
}

