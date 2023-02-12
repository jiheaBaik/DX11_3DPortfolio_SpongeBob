#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"

#include "Camera_Free.h"
#include "SpongeBob_Foot_L.h"
#include "SpongeBob_Wand.h"
#include "Boss_Sandy_Head_Glass.h"
#include "SmokeEffect.h"
#include "SnadySmokeEffect.h"

#include "Flame.h"

#include "PangEffect.h"

#include "Player.h"


CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_GamePlay::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	int rc;
	char *err_msg = 0;

	rc = sqlite3_open("../../Reference/DataBase/SpongeBob_jhBaik_DB.db", &db);
	if (rc)
		sqlite3_close(db);


	if (rc != SQLITE_OK)
	{
		MSG_BOX("SQL errory");
		sqlite3_free(err_msg);
		sqlite3_close(db);

		return E_FAIL;
	}
	

	if (FAILED(Ready_LightDesc()))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Boss(TEXT("Layer_Boss"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;	

	

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	/*CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	pGameInstance->PlayBGM(L"Poseidome.ogg", 0.1f);
	Safe_Release(pGameInstance);*/
//	CGameInstance::Get_Instance()->StopSound(CSoundMgr::PLAYER);
	
		CGameInstance::Get_Instance()->PlayMySound(L"testIntro5.ogg", CSoundMgr::EFFECT, 0.5f);
	//	CGameInstance::Get_Instance()->PlayMySound(L"Intro6.ogg", CSoundMgr::EFFECT, 0.5f);

	//CGameInstance::Get_Instance()->PlayMySound(L"BG1_3.mp3", CSoundMgr::EFFECT, 0.5f);
	return S_OK;
}

void CLevel_GamePlay::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
		
}

void CLevel_GamePlay::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	SetWindowText(g_hWnd, TEXT("게임플레이레벨임"));
}

HRESULT CLevel_GamePlay::Render()
{
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_LightDesc()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Light_Remove();
	LIGHTDESC			LightDesc;
//분
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float3(54,50,30);
	//LightDesc.fRange = 5000;
	//LightDesc.vDiffuse = _float4(1, 1, 1, 1);
	//LightDesc.vAmbient = _float4(1,0,0.92,0.5);
	//LightDesc.vSpecular = _float4(1, 0.98, 0.917,1);

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;

	


	////주
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float3(30.3f, 18.f, 40.8f);
	//LightDesc.fRange = 30;
	//LightDesc.vDiffuse = _float4(1, 1, 1, 1);
	//LightDesc.vAmbient = _float4(1, 0.49, 0, 0.5);
	//LightDesc.vSpecular = _float4(1,0.49,0,0.5);

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;




	//

	////노
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float3(40.2f, 18.f, 60);
	//LightDesc.fRange = 30;
	//LightDesc.vDiffuse = _float4(1, 1, 1, 1);
	//LightDesc.vAmbient = _float4(1, 0.75, 0, 0.5);
	//LightDesc.vSpecular = _float4(1,0.75,0,0.5);

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;

	


	////파
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float3(39.4, -1, 60);
	//LightDesc.fRange = 10;
	//LightDesc.vDiffuse = _float4(1, 1, 1, 1);
	//LightDesc.vAmbient = _float4(0,0,1,0.5);
	//LightDesc.vSpecular = _float4(0,0,1,1);


	////분
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float3(42,18,60);
	//LightDesc.fRange = 10;
	//LightDesc.vDiffuse = _float4(1, 1, 1, 1);
	//LightDesc.vAmbient = _float4(1, 0, 0.92, 0.5);
	//LightDesc.vSpecular = _float4(1, 0.98, 0.917, 1);

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;



	/*파
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float3(47, 18, 46);
	LightDesc.fRange = 700;
	LightDesc.vDiffuse = _float4(1, 1, 1, 0);
	LightDesc.vAmbient = _float4(0, 0, 1, 1);
	LightDesc.vSpecular = _float4(0, 0, 1, 1);*/



	//{{{{{{{{{{{{{
	//BSSE
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float3(47, 18, 46);
	LightDesc.fRange = 45;
	LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
	LightDesc.vAmbient = _float4(1, 0.87, 0.73, 1);
	LightDesc.vSpecular = _float4(0.3, 0.3,0.3,1);
	//LightDesc.vSpecular = _float4(0, 0, 0, 1);
	LightDesc.isCulling = false;
	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;
	//보
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float3(56, 0, 77);
	LightDesc.fRange = 15;
	LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
	LightDesc.vAmbient = _float4(0.725, 0, 1, 1);
	LightDesc.vSpecular = _float4(0.725, 0, 1, 1);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	////보
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float3(17.2, 0, 54.3);
	//LightDesc.fRange = 15;
	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
	//LightDesc.vAmbient = _float4(1, 0.627, 0.015, 1);
	//LightDesc.vSpecular = _float4(1, 0.627, 0.015, 1);

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;




	////파
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float3(75, 0, 35.29);
	//LightDesc.fRange = 15;
	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
	//LightDesc.vAmbient = _float4(1, 0.627, 0.015, 1);
	//LightDesc.vSpecular = _float4(1, 0.627, 0.015, 1);

	//파
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float3(75, 0, 35.29);
	LightDesc.fRange = 15;
	LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
	LightDesc.vAmbient = _float4(0.725, 0, 1, 1);
	LightDesc.vSpecular = _float4(0.725, 0, 1, 1);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;
	//***********************************************

	////보
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float3(56, 0, 77);
	//LightDesc.fRange = 15;
	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
	//LightDesc.vAmbient = _float4(0, 0, 1, 1);
	//LightDesc.vSpecular = _float4(0, 0, 1, 1);

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;

	//////보
	////ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	////LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	////LightDesc.vPosition = _float3(17.2, 0, 54.3);
	////LightDesc.fRange = 15;
	////LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
	////LightDesc.vAmbient = _float4(1, 0.627, 0.015, 1);
	////LightDesc.vSpecular = _float4(1, 0.627, 0.015, 1);

	////if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	////	return E_FAIL;




	////파
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float3(75, 0, 35.29);
	//LightDesc.fRange = 15;
	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
	//LightDesc.vAmbient = _float4(1, 0.627, 0.015, 1);
	//LightDesc.vSpecular = _float4(1, 0.627, 0.015, 1);

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;

	////파
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float3(33.97, 0, 15);
	//LightDesc.fRange = 15;
	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
	//LightDesc.vAmbient = _float4(0, 0, 1, 1);
	//LightDesc.vSpecular = _float4(0, 0, 1, 1);
	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;





	//}}}}}}}}}}}}}}}}}}}




	///////////////////////////////
	////보
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float3(59.7, 0, 16.1);
	//LightDesc.fRange = 15;
	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
	//LightDesc.vAmbient = _float4(1, 0.447, 0, 1);
	//LightDesc.vSpecular = _float4(1, 0.447, 0, 1);

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;

	////보
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float3(15.65, 0, 30.76);
	//LightDesc.fRange = 15;
	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
	//LightDesc.vAmbient = _float4(1, 0.447, 0, 1);
	//LightDesc.vSpecular = _float4(1, 0.447, 0, 1);

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;




	////파
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float3(29.1, 0, 75.8);
	//LightDesc.fRange = 15;
	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
	//LightDesc.vAmbient = _float4(1, 0.447, 0, 1);
	//LightDesc.vSpecular = _float4(1, 0.447, 0, 1);

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;

	////파
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eLightType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float3(79.4, 0, 60);
	//LightDesc.fRange = 15;
	//LightDesc.vDiffuse = _float4(1, 1, 1, 0.5);
	//LightDesc.vAmbient = _float4(1, 0.447, 0, 1);
	//LightDesc.vSpecular = _float4(1, 0.447, 0, 1);
	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/*if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_King_Neptune"), TEXT("Prototype_GameObject_King_Neptune"))))
		return E_FAIL;*/

	CPlayer* pplayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_Player")));
	pplayer->Set_m_Info(1);
	pplayer->Set_Navigation();
	pplayer->Set_Isrender(true);

	CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	m_pPlayerTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(45.5f, 0.f, 36.5f, 1));

	//_uint level = 1;
	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Player"), &level)))
	//	return E_FAIL;
	if(FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer"), TEXT("Prototype_GameObject_Patrick_Player"))))
		return E_FAIL;


	CTransform* m_pPatrickPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer"), TEXT("Com_Transform"));
	_vector vlook =  m_pPatrickPlayerTransform->Get_State(CTransform::STATE_LOOK);
//	m_pPlayerTransform->Set_State(CTransform::STATE_LOOK, vlook);
	
	m_pPlayerTransform->LookAt(XMVectorSet(47.f, 0.f, 44.7f, 1));
	/*CSpongeBob_Foot_L::FOOTDESC		FootDesc;
	ZeroMemory(&FootDesc, sizeof(CSpongeBob_Foot_L::FOOTDESC));

	FootDesc.eLevel = LEVEL_STATIC;
	FootDesc.pLayerTag = pLayerTag;
	FootDesc.iIndex = 0;
	FootDesc.pModelComTag = TEXT("Com_VIBuffer");
	FootDesc.pBoneName = "jt_bubbleFoot_L";

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SpongeBob_Foot_L"), TEXT("Prototype_GameObject_SpongeBob_Foot_L"), &FootDesc)))
		return E_FAIL;

	CSpongeBob_Wand::WANDDESC		WandDesc;
	ZeroMemory(&WandDesc, sizeof(CSpongeBob_Wand::WANDDESC));

	WandDesc.eLevel = LEVEL_STATIC;
	WandDesc.pLayerTag = pLayerTag;
	WandDesc.iIndex = 0;
	WandDesc.pModelComTag = TEXT("Com_VIBuffer");
	WandDesc.pBoneName = "IndexFinger1_R";

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_STATIC, TEXT("Layer_SpongeBob_Wand"), TEXT("Prototype_GameObject_SpongeBob_Wand"), &WandDesc)))
		return E_FAIL;*/


	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_King_Neptune"), TEXT("Prototype_GameObject_King_Neptune"))))
		return E_FAIL;

	

	
	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_Boss(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body"), TEXT("Prototype_GameObject_Boss_Sandy_Body"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head"), TEXT("Prototype_GameObject_Boss_Sandy_Head"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head2"), TEXT("Prototype_GameObject_Boss_Sandy_Head2"))))
		return E_FAIL;



	CFlame::FLAMEDESC		mFlameDesc;
	ZeroMemory(&mFlameDesc, sizeof(CFlame::FLAMEDESC));

	mFlameDesc.eLevel = LEVEL_GAMEPLAY;
	mFlameDesc.pLayerTag = L"Layer_Sandy_Body";
	mFlameDesc.iIndex = 0;
	mFlameDesc.pModelComTag = TEXT("Com_VIBuffer");
	mFlameDesc.pBoneName = "joint80";

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SandyFlame"), TEXT("Prototype_GameObject_Flame"), &mFlameDesc)))
		return E_FAIL;




	CBoss_Sandy_Head_Glass::GLASSDESC		mGlassDesc;
	ZeroMemory(&mGlassDesc, sizeof(CBoss_Sandy_Head_Glass::GLASSDESC));

	mGlassDesc.eLevel = LEVEL_GAMEPLAY;
	mGlassDesc.pLayerTag = L"Layer_Sandy_Head";
	mGlassDesc.iIndex = 0;
	mGlassDesc.pModelComTag = TEXT("Com_VIBuffer");
	mGlassDesc.pBoneName = "jt_helmet";

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head_Glass"), TEXT("Prototype_GameObject_Boss_Sandy_Head_Glass"), &mGlassDesc)))
		return E_FAIL;




	ZeroMemory(&mGlassDesc, sizeof(CBoss_Sandy_Head_Glass::GLASSDESC));

	mGlassDesc.eLevel = LEVEL_GAMEPLAY;
	mGlassDesc.pLayerTag = L"Layer_Sandy_Head2";
	mGlassDesc.iIndex = 0;
	mGlassDesc.pModelComTag = TEXT("Com_VIBuffer");
	mGlassDesc.pBoneName = "jt_helmet";
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head_Glass2"), TEXT("Prototype_GameObject_Boss_Sandy_Head_Glass2"), &mGlassDesc)))
		return E_FAIL;




	CSnadySmokeEffect::SMOKEDESC		mSmokeDesc;
	ZeroMemory(&mSmokeDesc, sizeof(CSnadySmokeEffect::SMOKEDESC));

	
	mSmokeDesc.scale = 1;
	mSmokeDesc.addPosition = 4;
	mSmokeDesc.interpolation = 3;
	mSmokeDesc.shaderAlpha = 0;
	mSmokeDesc.distance =5;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SnadySmokeEffect"), TEXT("Prototype_GameObject_CSnadySmokeEffect"), &mSmokeDesc)))
		return E_FAIL;
	ZeroMemory(&mSmokeDesc, sizeof(CSnadySmokeEffect::SMOKEDESC));

	mSmokeDesc.scale = 2;
	mSmokeDesc.addPosition = 5;
	mSmokeDesc.interpolation = 2;
	mSmokeDesc.shaderAlpha = 0;
	mSmokeDesc.distance = 6;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SnadySmokeEffect"), TEXT("Prototype_GameObject_CSnadySmokeEffect"), &mSmokeDesc)))
		return E_FAIL;
	ZeroMemory(&mSmokeDesc, sizeof(CSnadySmokeEffect::SMOKEDESC));
	mSmokeDesc.scale = 3;
	mSmokeDesc.addPosition = 6;
	mSmokeDesc.interpolation = 1;
	mSmokeDesc.shaderAlpha = 0;
	mSmokeDesc.distance = 7;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SnadySmokeEffect"), TEXT("Prototype_GameObject_CSnadySmokeEffect"), &mSmokeDesc)))
		return E_FAIL;



	_vector pos = XMVectorSet(20.5f, 3.f, 47.f, 1);


	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SmokeEffect"), TEXT("Prototype_GameObject_SmokeEffect"),&pos)))
		return E_FAIL;
	pos = XMVectorSet(20.7f, 3.f, 56.2f, 1);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SmokeEffect"), TEXT("Prototype_GameObject_SmokeEffect"),&pos)))
		return E_FAIL;
	
	pos = XMVectorSet(27.f, 3, 46.f, 1);
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrowBlue"), TEXT("Prototype_GameObject_CGlowBlue"), &pos)))
		return E_FAIL;
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ElectircEffect"), TEXT("Prototype_GameObject_Electric"), &pos)))
		return E_FAIL;
	

	pos = XMVectorSet(28.5f, 7, 50.5f, 1);
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrowBlue"), TEXT("Prototype_GameObject_CGlowBlue"), &pos)))
		return E_FAIL;
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ElectircEffect"), TEXT("Prototype_GameObject_Electric"), &pos)))
		return E_FAIL;
	

	



	pos = XMVectorSet(0,1,1, 1);//머리용
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ElectircEffect"), TEXT("Prototype_GameObject_Electric"), &pos)))
		return E_FAIL;
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrowBlue2"), TEXT("Prototype_GameObject_CGlowBlue2"), &pos)))
		return E_FAIL;
	pos = XMVectorSet(1, 0, 1, 1);//몸통용
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ElectircEffect"), TEXT("Prototype_GameObject_Electric"), &pos)))
		return E_FAIL;
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrowBlue2"), TEXT("Prototype_GameObject_CGlowBlue2"), &pos)))
		return E_FAIL;

	pos = XMVectorSet(28.1f, 4, 56.691f, 1);
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrowBlue"), TEXT("Prototype_GameObject_CGlowBlue"), &pos)))
		return E_FAIL;
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ElectircEffect"), TEXT("Prototype_GameObject_Electric"), &pos)))
		return E_FAIL;


	//pos = XMVectorSet(28.5f, 9, 50.f, 1);
	_int look = 1;
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_BubblePangEffect"), TEXT("Prototype_GameObject_BubblePangEffect"), &look)))
		return E_FAIL;
	look = -1;
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_BubblePangEffect"), TEXT("Prototype_GameObject_BubblePangEffect"), &look)))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SmokeEffect2"), TEXT("Prototype_GameObject_SmokeEffec2t"))))
		return E_FAIL;*/



	


	

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Slame"), TEXT("Prototype_GameObject_Sandy_Slame"))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player_Slame"), TEXT("Prototype_GameObject_Player_Slame"))))
		return E_FAIL;


	//_float3 Info = _float3(50, 13, 50);
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Point"), TEXT("Prototype_GameObject_Effect_Point"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect_UnderPang"), TEXT("Prototype_GameObject_UnderPangEffect"))))
		return E_FAIL;
	

	//CPangEffect::PANGDESC		mPangDesc;
	//ZeroMemory(&mPangDesc, sizeof(CPangEffect::PANGDESC));


	// mPangDesc.scale = 13;
	// mPangDesc.addPosition = 4;
	// mPangDesc.interpolation = 0;
	// mPangDesc.shaderAlpha = 8;
	// mPangDesc.distance = 5;


	/*if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Pang"), TEXT("Prototype_GameObject_PangEffect"), &mPangDesc)))
		return E_FAIL;
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect_EndPang"), TEXT("Prototype_GameObject_EndPangEffect"))))
		return E_FAIL;*/
	
	/*if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy"), TEXT("Prototype_GameObject_Boss_Sandy"))))
		return E_FAIL;*/
	
	/*if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SmokeEffect"), TEXT("Prototype_GameObject_SandySmokeEffect"))))
		return E_FAIL;
*/
	_bool iFront = true;
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_BlackBackGround"), TEXT("Prototype_GameObject_BlackBackGround"),&iFront)))
		return E_FAIL;
	iFront = false;
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_BlackBackGround"), TEXT("Prototype_GameObject_BlackBackGround"), &iFront)))
		return E_FAIL;
	 
	pGameInstance->PlayBGM(L"Sound_Effects_Factory_Large_Crowd_Applause.mp3", 0.08f);


	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/*if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;*/

	/*if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Ring_Top"))))
		return E_FAIL;*/



	int rc;
	sqlite3_stmt *res3;

	string sqlTemp = "select * from Level_Boss1_Map";
	const char* sql = sqlTemp.c_str();


	rc = sqlite3_prepare_v2(db, sql, -1, &res3, 0);

	while (sqlite3_step(res3) == SQLITE_ROW)
	{
//		objInfo.toolLevel = (LEVEL)sqlite3_column_int(res3, 0);
		//objInfo.clientLevel = (OBJ_CLIENTLEVELE)sqlite3_column_int(res3, 1);

		char			temp[MAX_PATH] = "";
		strcpy_s(temp, (char*)sqlite3_column_text(res3, 2));

	/*	if (strcmp(temp, "Cube") == 0)
		{
			objInfo.pLayerTag = L"Layer_Cube";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Cube";

		}
		else */if (strcmp(temp, "SpongeBob") == 0)
		{
			objInfo.pLayerTag = L"Layer_SpongeBob";
			objInfo.pPrototypeTag = L"Prototype_GameObject_SpongeBob";
		}
		else if (strcmp(temp, "Ground") == 0)
		{
			objInfo.pLayerTag = L"Layer_Ground";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Ground";
		}
		else if (strcmp(temp, "Ground2") == 0)
		{
			objInfo.pLayerTag = L"Layer_Ground2";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Ground2";
		}
		else if (strcmp(temp, "Ring_Top") == 0)
		{
			objInfo.pLayerTag = L"Layer_Ring_Top";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Ring_Top";
		}
		else if (strcmp(temp, "Rope_Top") == 0)
		{
			objInfo.pLayerTag = L"Layer_Rope_Top";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Rope_Top";
		}
		else if (strcmp(temp, "Ring_Pillar") == 0)
		{
			objInfo.pLayerTag = L"Layer_Ring_Pillar";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Ring_Pillar";
		}
		else if (strcmp(temp, "Light_Tube") == 0)
		{
			objInfo.pLayerTag = L"Layer_Light_Tube";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Light_Tube";
		}
		else if (strcmp(temp, "Scoreboard") == 0)
		{
			objInfo.pLayerTag = L"Layer_Scoreboard";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Scoreboard";
		}
		else if (strcmp(temp, "Big_Pillar") == 0)
		{
			objInfo.pLayerTag = L"Layer_Big_Pillar";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Big_Pillar";
		}
		else if (strcmp(temp, "Big_Pillar_Attachment") == 0)
		{
			objInfo.pLayerTag = L"Big_Pillar_Attachment";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Big_Pillar_Attachment";
		}
		else if (strcmp(temp, "Big_Cloth") == 0)
		{
			objInfo.pLayerTag = L"Big_Cloth";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Big_Cloth";
		}
		else if (strcmp(temp, "Pos_Light") == 0)
		{
			objInfo.pLayerTag = L"Pos_Light";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Pos_Light";
		}
		else if (strcmp(temp, "Stairs_02") == 0)
		{
			objInfo.pLayerTag = L"Stairs_02";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Stairs_02";
		}
		else if (strcmp(temp, "Stairs_04") == 0)
		{
			objInfo.pLayerTag = L"Stairs_04";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Stairs_04";
		}
		else if (strcmp(temp, "SM_PD_Banner_01") == 0)
		{
			objInfo.pLayerTag = L"SM_PD_Banner_01";
			objInfo.pPrototypeTag = L"Prototype_GameObject_SM_PD_Banner_01";
		}
		else if (strcmp(temp, "Poster") == 0)
		{
			objInfo.pLayerTag = L"Poster";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Poster";
		}
		else if (strcmp(temp, "Poster2") == 0)
		{
			objInfo.pLayerTag = L"Poster2";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Poster2";
		}

		else if (strcmp(temp, "Fish_A") == 0)
		{
			objInfo.pLayerTag = L"Fish_A";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Fish_A";
		}
		else if (strcmp(temp, "Fish_B") == 0)
		{
			objInfo.pLayerTag = L"Fish_B";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Fish_B";
		}
		else if (strcmp(temp, "Fish_C") == 0)
		{
			objInfo.pLayerTag = L"Fish_C";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Fish_C";
		}
		else if (strcmp(temp, "Fish_D") == 0)
		{
			objInfo.pLayerTag = L"Fish_D";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Fish_D";
		}
		else if (strcmp(temp, "Fish_E") == 0)
		{
			objInfo.pLayerTag = L"Fish_E";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Fish_E";
		}
		else if (strcmp(temp, "Fish_G") == 0)
		{
			objInfo.pLayerTag = L"Fish_G";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Fish_G";
		}
		else if (strcmp(temp, "Fish_H") == 0)
		{
			objInfo.pLayerTag = L"Fish_H";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Fish_H";
		}
		else if (strcmp(temp, "Fish_I") == 0)
		{
			objInfo.pLayerTag = L"Fish_I";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Fish_I";
		}
	/*	else if (strcmp(temp, "Fish_J") == 0)
		{
			objInfo.pLayerTag = L"Fish_J";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Fish_J";
		}*/
		else if (strcmp(temp, "Fish_K") == 0)
		{
			objInfo.pLayerTag = L"Fish_K";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Fish_K";
		}
		else if (strcmp(temp, "Fish_L") == 0)
		{
			objInfo.pLayerTag = L"Fish_L";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Fish_L";
		}
		else if (strcmp(temp, "Scoreboard_Ropes") == 0)
		{
			objInfo.pLayerTag = L"Scoreboard_Ropes";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Scoreboard_Ropes";
		}
		else if (strcmp(temp, "Light_Cone_Pink") == 0)
		{
			objInfo.pLayerTag = L"Layer_Light_Cone_Pink";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Light_Cone_Pink";
		}
		else if (strcmp(temp, "Light_Cone_Blue") == 0)
		{
			objInfo.pLayerTag = L"Layer_Light_Cone_Blue";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Light_Cone_Blue";
		}
		else if (strcmp(temp, "Light_Cone_Green") == 0)
		{
			objInfo.pLayerTag = L"Layer_Light_Cone_Green";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Light_Cone_Green";
		}
		else if (strcmp(temp, "Bubble") == 0)
		{
			objInfo.pLayerTag = L"Layer_Bubble";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Bubble";
		}
		else if (strcmp(temp, "Mesh_scoreboard_broken") == 0)
		{
			objInfo.pLayerTag = L"Layer_Mesh_scoreboard_broken";
			objInfo.pPrototypeTag = L"Prototype_GameObject_Mesh_scoreboard_broken";
		}
		else
		{
			continue;
		}
		objInfo.iIndex = sqlite3_column_int(res3, 5);
		objInfo.iNumber = sqlite3_column_int(res3, 6);
		objInfo.fPos.x = (_float)sqlite3_column_double(res3, 7);
		objInfo.fPos.y = (_float)sqlite3_column_double(res3, 8);
		objInfo.fPos.z = (_float)sqlite3_column_double(res3, 9);
		objInfo.fScale.x = (_float)sqlite3_column_double(res3, 10);
		objInfo.fScale.y = (_float)sqlite3_column_double(res3, 11);
		objInfo.fScale.z = (_float)sqlite3_column_double(res3, 12);
		objInfo.fAxist.x = (_float)sqlite3_column_int(res3, 21);
		objInfo.fAxist.y = (_float)sqlite3_column_int(res3, 22);
		objInfo.fAxist.z = (_float)sqlite3_column_int(res3, 23);
		objInfo.fRotation = (_float)sqlite3_column_double(res3, 24);

		char * temp1 = (char*)sqlite3_column_text(res3, 4);
		MultiByteToWideChar(CP_ACP, 0, temp1, strlen(temp1) + 1, pTextureInfo, 256);
		objInfo.pTextureInfo = pTextureInfo;
	
		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, (_tchar*)objInfo.pLayerTag, objInfo.pPrototypeTag, &objInfo)))
			return E_FAIL;


	}
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SkyBox_Poeisdom"), TEXT("Prototype_GameObject_SkyBox_Poeisdom"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Rope_Top_Broken"), TEXT("Prototype_GameObject_Rope_Top_Broekn"))))
		return E_FAIL;


	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ForkLift"))))
	//	return E_FAIL;

	/*if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;*/

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);	

	/*for (_uint i = 0; i < 30; ++i)
	{
		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Effect"))))
			return E_FAIL;
	}
*/


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	int rc;
	sqlite3_stmt *res3;

	string sqlTemp = "select * from Level_Boss1_UI";
	const char* sql = sqlTemp.c_str();


	rc = sqlite3_prepare_v2(db, sql, -1, &res3, 0);

	while (sqlite3_step(res3) == SQLITE_ROW)
	{
//		obj_uiInfo.toolLevel = (LEVEL)sqlite3_column_int(res3, 0);
	//	obj_uiInfo.clientLevel = (OBJ_CLIENTLEVELE)sqlite3_column_int(res3, 1);

		char			temp[MAX_PATH] = "";
		strcpy_s(temp, (char*)sqlite3_column_text(res3, 1));

		obj_uiInfo.fPos.x = (_float)sqlite3_column_double(res3, 4);
		obj_uiInfo.fPos.y = (_float)sqlite3_column_double(res3, 5);
		obj_uiInfo.pTextureInfo = sqlite3_column_int(res3, 6);
		pGameInstance->Inital_Hp();
		if (strcmp(temp, "Underpants") == 0)
		{
 			obj_uiInfo.pLayerTag = L"Layer_Underpants";
			obj_uiInfo.pPrototypeTag = L"Prototype_GameObject_Underpants";

		}
		else if (strcmp(temp, "Boss_HPBar") == 0)
		{
			obj_uiInfo.pLayerTag = L"Laye_Boss_HPBar";
			obj_uiInfo.pPrototypeTag = L"Prototype_GameObject_Boss_HPBar";

		}
		else if (strcmp(temp, "Eenergy") == 0)
		{
			static _uint count = 0;

			if (count < 3 && count >= 0)
				obj_uiInfo.pTextureInfo = 0;
			else if (count >= 3 && count < 6)
				obj_uiInfo.pTextureInfo = 1;
			else
				obj_uiInfo.pTextureInfo = 2;

			obj_uiInfo.pObj = L"Eenergy";
			obj_uiInfo.pLayerTag = L"Laye_Eenergy";
			obj_uiInfo.pPrototypeTag = L"Prototype_GameObject_Boss_Energy";

			count++;
		}
		else 
		{
			continue;
		}
		
	
		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, (_tchar*)obj_uiInfo.pLayerTag, obj_uiInfo.pPrototypeTag, &obj_uiInfo)))
			return E_FAIL;

		
	}

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Fish"), TEXT("Prototype_GameObject_Fish"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cut"), TEXT("Prototype_GameObject_Cut"))))
		return E_FAIL;
	
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	//35.f, 0.f, 45.f, 1
	CameraDesc.vEye = _float3(35.f, 13.f, 25.f);
	CameraDesc.vAt = _float3(35.f, 0.f, 45.f);
	CameraDesc.TransformDesc.SpeedPerSec = 5.f;
	CameraDesc.TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = _float(g_iWinSizeX) / g_iWinSizeY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 500.f;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
		return E_FAIL;

	//Buble정보 얻기
	CCamera_Free* pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, pLayerTag));
	pCamera->Set_IsGamePlay(true);
	pCamera->Set_m_bIsDirector_End(false);
	pCamera->Set_Level(1);
	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_GamePlay::Free()
{
	//CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	//Safe_AddRef(pGameInstance);

	//pGameInstance->StopAll();
	//Safe_Release(pGameInstance);

	/*CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Light_Remove();

	Safe_Release(pGameInstance);*/


	__super::Free();
	sqlite3_close(db);
}
