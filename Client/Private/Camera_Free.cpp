#include "stdafx.h"
#include "..\Public\Camera_Free.h"
#include "GameInstance.h"
#include "Player.h"
#include "Patrick_Player.h"

#include <iostream>
#include "Boss_Sandy_Body.h"

#include "Scoreboard.h"
#include "Scoreboard_Broken.h"
#include "Rope_Top.h"
#include "Rope_Top_Broekn.h"

#include "King_Neptune.h"

#include "SmokeEffect.h"
#include "SmokeEffect2.h"
#include "Electric.h"
#include "BubblePangEffect.h"
#include "GlowBlue.h"
#include "GlowBlue2.h"
#include "SnadySmokeEffect.h"
#include "PangEffect.h"
#include "BlackBackGround.h"
#include "Squidward.h"
#include "Cut.h"





CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{

}

CCamera_Free::CCamera_Free(const CCamera_Free & Prototype)
	: CCamera(Prototype)
{
}

HRESULT CCamera_Free::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Free::Tick(_double TimeDelta)
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


//	m_CameraDesc.fFovy = XMConvertToRadians(60.0f);

	if (m_bIsGameplaye)//게임시작
	{

		if (!m_bIsDirector_End && m_iLevel != 0)
		{
			if (m_iDirectorCount == 0)
			{
				Boss1_DirectorCount_1(TimeDelta);
			}
			else if (m_iDirectorCount == 1)
				Boss1_DirectorCount_2(TimeDelta);
			else if (m_iDirectorCount == 2)
				Boss1_DirectorCount_3(TimeDelta);
			else if (m_iDirectorCount == 3)
				Boss1_DirectorCount_4(TimeDelta);
		}

		if (!m_bIsDirector_End && m_iLevel == 0)
		{

			


			CTransform* m_pTargetTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_FIELD, TEXT("Layer_Squidward"), TEXT("Com_Transform"));
			if (m_pTargetTransform != nullptr)
			{
				_vector m_vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
				static _bool End;
				if(!End)
					End = m_pTransformCom->Chase(m_vTargetPos + XMVectorSet(0, 3, 0, 1), TimeDelta *3.f, 5.f);

				CSquidward* ptarget = static_cast<CSquidward*>(pGameInstance->Get_GameObject(LEVEL_FIELD, TEXT("Layer_Squidward")));



				if (m_IsShake)
				{
					/*if (m_iShackCount > 10)
					{
						m_IsShake = false;
						m_fX = 0.08f;
						m_fY = 0.08f;
					}*/
					m_iShackCount++;
					if (m_iShackCount % 2 == 0)
					{
						m_fX *= -1;
						m_fY *= -1;
					}
					_vector vEye = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

					m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEye + XMVectorSet(m_fX, m_fY, 0, 1));
					if (m_fX < 0)
					{
						m_fX -= 0.002;
						m_fY -= 0.002;

					}

				}


				if (ptarget->Get_End())
				{
					m_iShackCount = 0;
					m_bIsDirector_End = true;
					 m_fX = 0.03f;
					 m_fY = 0.1f;

				}
			}
			


		}
		



		if (m_bIsDirector_End)//라운드 시작
		{
			CTransform* m_pPlayerTransform = nullptr;
			_vector m_vPlayerPos;

		

			if (pGameInstance->Get_Player_Info() == 0)//스폰지밥
			{
				m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
				m_vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
			}
			else//뚱이
			{
				m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer"), TEXT("Com_Transform"));
				m_vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
			}

			//플레이어 위치 정보 얻기
			_float3 fPlayerPos;
			XMStoreFloat3(&fPlayerPos, m_vPlayerPos);

			//플레이어 Look 정보 얻기
			_vector		vPlayerLook = m_pPlayerTransform->Get_State(CTransform::STATE_LOOK) * -1;
			vPlayerLook = XMVector3Normalize(vPlayerLook);
			_float3 fPalyerLook;
			XMStoreFloat3(&fPalyerLook, vPlayerLook);

			//카메라 위치 셋팅
			//_float fdistance = 10.f;
			//_vector		vEye = XMVectorSet(fPlayerPos.x + (fPalyerLook.x * fdistance), fPlayerPos.y + (fPalyerLook.y * fdistance) + 11, fPlayerPos.z + (fPalyerLook.z * fdistance), 1);
			
			_float fdistance = 10.f;
			_float fYdistance = 11.f;

		

			CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_Player")));
			CPatrick_Player* pPtricPlayer = static_cast<CPatrick_Player*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer")));

			
			if (pPtricPlayer != nullptr)
			{
				if (pPlayer->Get_IsFlaying() || pPtricPlayer->Get_IsFlaying())
					fYdistance = 5;
			}
			
			if (m_iLevel == 0)
			{
				if (pPlayer->Get_IsAttack() || pPlayer->Get_IsHit_Field())
				{
					//m_CameraDesc.fFovy = XMConvertToRadians(50.0f);
					fdistance = 5;
					fYdistance = 5;
					if(fLookYdistance > 3)
						fLookYdistance -= 0.06f;

				}
				else
				{
					fdistance = 12;
					fYdistance = 6;
					if(fLookYdistance < 4.f)
						fLookYdistance += 0.01f;

				}
			
			}

			//if (GetAsyncKeyState('B') & 0x0001)
		//	{
				
				

		//	}
			_vector		vEye = XMVectorSet(fPlayerPos.x + (fPalyerLook.x * fdistance), fPlayerPos.y + (fPalyerLook.y * fdistance) + fYdistance, fPlayerPos.z + (fPalyerLook.z * fdistance), 1);


			if (m_IsShake)
			{
				if (m_iShackCount > 10)
				{
					m_IsShake = false;
					m_fX = 0.05f;
					m_fY = 0.1f;
				}
					m_iShackCount++;
				if (m_iShackCount % 4 == 0)
				{
					m_fX *= -1;
					m_fY *= -1;
				}
				
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEye + XMVectorSet(m_fX, m_fY, 0,1));
				if (m_fX < 0)
				{
					m_fX -= 0.002;
					m_fY -= 0.002;

				}

			}
			else
			{
	
				m_iShackCount = 0;
				vEye = XMVectorLerp(m_pTransformCom->Get_State(CTransform::STATE_POSITION), vEye , TimeDelta * fLookYdistance);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEye);
			}


			m_pTransformCom->LookAt(m_vPlayerPos + XMVectorSet(0, fLookYdistance, 0, 1));
		}

	}

	__super::Tick(TimeDelta);
	Safe_Release(pGameInstance);




}

void CCamera_Free::LateTick(_double TimeDelta)
{
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

CCamera_Free * CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Free*		pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Free");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCamera_Free::Clone(void * pArg)
{
	CCamera_Free*		pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Created : CCamera_Free");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();
}

void CCamera_Free::Boss1_DirectorCount_1(_double TimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	static _bool IsSetting = false;
	static _vector		vLook;
	static _vector		vEye;
	if (!IsSetting)// 처음 셋팅
	{
		
		CCut* pCut = static_cast<CCut*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cut")));
		

		Direcotor_cameradesc = pGameInstance->Get_DirecotorCamera();
		vLook = XMVectorSet(Direcotor_cameradesc.m_vFirstDirecotorCamera[2].fPos.x, Direcotor_cameradesc.m_vFirstDirecotorCamera[2].fPos.y, Direcotor_cameradesc.m_vFirstDirecotorCamera[2].fPos.z, 1);
		vEye = XMVectorSet(Direcotor_cameradesc.m_vFirstDirecotorCamera[0].fPos.x, Direcotor_cameradesc.m_vFirstDirecotorCamera[0].fPos.y, Direcotor_cameradesc.m_vFirstDirecotorCamera[0].fPos.z, 1);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEye);
		m_pTransformCom->LookAt(vLook);

		IsSetting = true;
		m_bIsDirector1 = true;
	}

	if (m_bIsDirector1)//씬1
	{
		static _uint _cutCount = 0;
		_cutCount++;
		if (_cutCount > 70)
		{
			CCut* pCut = static_cast<CCut*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cut")));
			pCut->Set_Render(true);
			pCut->Set_TexutureNum(0);
		}



		_bool IsEnd = m_pTransformCom->Chase(vLook, TimeDelta * 0.5f, 4.f);
		_vector m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 fPos;
		XMStoreFloat3(&fPos, m_vPos);
		_float3 fLookPos;
		XMStoreFloat3(&fLookPos, vLook);

		//cout << "m_vPos : " << fPos.y << endl;
		if (IsEnd)//바꿔야함타이밍
		{
			CCut* pCut = static_cast<CCut*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cut")));
			pCut->Set_Render(true);
			pCut->Set_TexutureNum(1);

			m_bIsDirector1 = false;
			m_bIsDirector2 = true;
			vLook = XMVectorSet(Direcotor_cameradesc.m_vFirstDirecotorCamera[3].fPos.x, Direcotor_cameradesc.m_vFirstDirecotorCamera[3].fPos.y, Direcotor_cameradesc.m_vFirstDirecotorCamera[3].fPos.z, 1);
			m_pTransformCom->LookAt(vLook);
		}
	}

	if (!m_bIsDirector1 && m_bIsDirector2)//씬2
	{
		_vector m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 fPos;
		XMStoreFloat3(&fPos, m_vPos);

		if (!m_bIsDirector2_Close_Up_Start)
			m_bIsDirector2_Close_Up1 = m_pTransformCom->Chase(vLook, TimeDelta * 0.2f, 3.f);


		if (m_bIsDirector2_Close_Up1)//클로즈업 1
		{
			

			m_bIsDirector2_Close_Up_Start = true;
			m_iClise_Up++;
			m_pTransformCom->Chase(vLook, TimeDelta * 0.2f, 2.5f);
			if (m_iClise_Up == 60)
			{
				m_bIsDirector2_Close_Up2 = m_pTransformCom->Chase(vLook, TimeDelta * 0.2f, 2.5f);
				m_iClise_Up = 0;
				m_bIsDirector2_Close_Up1 = false;
			}
		}
		if (m_bIsDirector2_Close_Up2)//클로즈업 2
		{
			m_iClise_Up++;
			m_pTransformCom->Chase(vLook, TimeDelta * 0.2f, 2.f);
			if (m_iClise_Up == 60)
			{
				m_iClise_Up = 0;

				m_bIsDirector2_Close_Up3 = m_pTransformCom->Chase(vLook, TimeDelta * 0.2f, 2.f);
				m_bIsDirector2_Close_Up2 = false;
			}
		}
		if (m_bIsDirector2_Close_Up3)//클로즈업 3
		{
			

			_bool IsEnd = m_pTransformCom->Chase(vLook, TimeDelta * 0.2f, 1.5f);
			if (IsEnd)
			{
			/*	CCut* pCut = static_cast<CCut*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cut")));
				pCut->Set_Render(true);
				pCut->Set_TexutureNum(2);*/
				m_iClise_Up++;
				if (m_iClise_Up == 60)
				{
					CCut* pCut = static_cast<CCut*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cut")));
					pCut->Set_Render(true);
					pCut->Set_TexutureNum(2);

					m_iClise_Up = 0;

					m_bIsDirector2 = false;
					m_bIsDirector3 = true;
					vLook = XMVectorSet(Direcotor_cameradesc.m_vFirstDirecotorCamera[4].fPos.x, Direcotor_cameradesc.m_vFirstDirecotorCamera[4].fPos.y, Direcotor_cameradesc.m_vFirstDirecotorCamera[4].fPos.z, 1);
					m_pTransformCom->LookAt(vLook);
				}
			}

		}
	}


	if (!m_bIsDirector2 && m_bIsDirector3)//씬3
	{
		static int j = 0;
		if (j < 150)
			j++;
		else
		{
			CCut* pCut = static_cast<CCut*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cut")));
			pCut->Set_Render(false);

			CSnadySmokeEffect* pCSnadySmokeEffect = static_cast<CSnadySmokeEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SnadySmokeEffect")));
			pCSnadySmokeEffect->IsRender(true);
			pCSnadySmokeEffect = static_cast<CSnadySmokeEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SnadySmokeEffect"), 1));
			pCSnadySmokeEffect->IsRender(true);
			pCSnadySmokeEffect = static_cast<CSnadySmokeEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SnadySmokeEffect"), 2));
			pCSnadySmokeEffect->IsRender(true);

			m_bIsDirector_End = true;
			m_bIsDirector3 = false;
		}
	}
	Safe_Release(pGameInstance);

}



void CCamera_Free::Boss1_DirectorCount_2(_double TimeDelta)
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	static _bool IsSetting = false;
	static _vector		vLook;
	static _vector		vEye;
	_vector m_vScoreBoardPos = XMVectorSet(0, 0, 0, 1);
	static _bool first = false;
	static _vector m_vRound2_ParickPos = XMVectorSet(0, 0, 0, 1);

	if (!first)
	{
		CGameInstance::Get_Instance()->StopSound(CSoundMgr::EFFECT);
		CGameInstance::Get_Instance()->PlayMySound(L"ScoreBoxBroken2.ogg", CSoundMgr::EFFECT, 0.5f);
		first = true;
	}
	if (!IsSetting)// 처음 셋팅
	{
		CTransform* m_pScoreboardTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Scoreboard"), TEXT("Com_Transform"));
		m_vScoreBoardPos = m_pScoreboardTransform->Get_State(CTransform::STATE_POSITION);
		//Direcotor_cameradesc = pGameInstance->Get_DirecotorCamera();
		//vLook = XMVectorSet(Direcotor_cameradesc.m_vFirstDirecotorCamera[2].fPos.x, Direcotor_cameradesc.m_vFirstDirecotorCamera[2].fPos.y, Direcotor_cameradesc.m_vFirstDirecotorCamera[2].fPos.z, 1);
		vEye = XMVectorSet(Direcotor_cameradesc.m_vFirstDirecotorCamera[7].fPos.x, Direcotor_cameradesc.m_vFirstDirecotorCamera[7].fPos.y, Direcotor_cameradesc.m_vFirstDirecotorCamera[7].fPos.z, 1);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEye);
		m_pTransformCom->LookAt(XMVectorSet(44, 24, 45, 1));


		CScoreboard* pScoreboard = static_cast<CScoreboard*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Scoreboard")));
		pScoreboard->Move(TimeDelta);



		m_iClise_Up++;

		if (m_iClise_Up == 150)
		{
			IsSetting = true;
			m_bIsDirector1 = true;
			m_iClise_Up = 0;
			//vEye = XMVectorSet(Direcotor_cameradesc.m_vFirstDirecotorCamera[8].fPos.x, Direcotor_cameradesc.m_vFirstDirecotorCamera[8].fPos.y, Direcotor_cameradesc.m_vFirstDirecotorCamera[8].fPos.z, 1);
			 vEye = XMVectorSet(37.3, 6,32, 1);

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEye);
			m_pTransformCom->LookAt(/*m_vScoreBoardPos*/XMVectorSet(28, 1, 52.7, 1));

			pScoreboard->Set_Dead();

			CSmokeEffect* pSmoke = static_cast<CSmokeEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SmokeEffect")));
			pSmoke->IsRender();
			CSmokeEffect* pSmoke2 = static_cast<CSmokeEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SmokeEffect"),1));
			pSmoke2->IsRender();

			//CSmokeEffect2* pSmoke2 = static_cast<CSmokeEffect2*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SmokeEffect2")));
			//pSmoke2->IsRender();
			

			CRope_Top* pRope_Top = static_cast<CRope_Top*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Rope_Top"), 6));
			pRope_Top->Set_Dead();
			CRope_Top_Broekn* pRope_Top_Brken = static_cast<CRope_Top_Broekn*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Rope_Top_Broken")));
			pRope_Top_Brken->Set_IsBroken(true);

		}

	}


	if (m_bIsDirector1)//씬1
	{
		m_iClise_Up++;
		CScoreboard_Broken* pScoreboard_Broekn = static_cast<CScoreboard_Broken*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Mesh_scoreboard_broken")));
		pScoreboard_Broekn->Set_IsBroken();

		if (m_iClise_Up <= 70)
		{
			pScoreboard_Broekn->Move(TimeDelta);
		}
		if (m_iClise_Up > 71)
		{
			CElectric* pElectric = static_cast<CElectric*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ElectircEffect")));
			pElectric->IsRender(true);
			CElectric* pElectric2 = static_cast<CElectric*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ElectircEffect"),1));
			pElectric2->IsRender(true);
			CElectric* pElectric3 = static_cast<CElectric*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ElectircEffect"), 4));
			pElectric3->IsRender(true);

			CGlowBlue* pGlowBlue = static_cast<CGlowBlue*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrowBlue")));
			pGlowBlue->IsRender(true);
			CGlowBlue* pGlowBlue2 = static_cast<CGlowBlue*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrowBlue"), 1));
			pGlowBlue2->IsRender(true);
			CGlowBlue* pGlowBlue3 = static_cast<CGlowBlue*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrowBlue"), 2));
			pGlowBlue3->IsRender(true);

		}

		CTransform* m_pScoreboardTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Mesh_scoreboard_broken"), TEXT("Com_Transform"));
		m_vScoreBoardPos = m_pScoreboardTransform->Get_State(CTransform::STATE_POSITION);

		_bool IsEnd = m_pTransformCom->Chase(m_vScoreBoardPos, TimeDelta *0.7f, 15);
		if (IsEnd)
		{


			m_bIsDirector1 = false;
			m_bIsDirector2 = true;
			m_iClise_Up = 0;


			//플레이어 위치 정보 얻기
			CTransform* m_pPatrickTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer"), TEXT("Com_Transform"));

			m_pPatrickTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(35.5f, 0.f, 32.7f, 1));
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(35.9, 2, 37.f, 1));
			m_pTransformCom->LookAt(XMVectorSet(35.4, 1, 32.1, 1));
		}
	}


	if (!m_bIsDirector1 && m_bIsDirector2)//씬2
	{

		CCut* pCut = static_cast<CCut*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cut")));
		pCut->Set_Render(true);
		pCut->Set_TexutureNum(3);

		CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
		m_pPlayerTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(59.39, 0.f, 36, 1));

		_bool IsEnd = m_pTransformCom->Chase(XMVectorSet(35.4, 1, 32.1, 1), TimeDelta * 0.3f, 3);

		if (IsEnd)
		{
			m_bIsDirector_End = true;
			m_bIsDirector2 = false;

			CCut* pCut = static_cast<CCut*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cut")));
			pCut->Set_Render(false);
		}

	}


	Safe_Release(pGameInstance);
}



void CCamera_Free::Boss1_DirectorCount_3(_double TimeDelta)
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	static _bool IsSetting = false;
	static _vector		vLook;
	static _vector		vEye;


	if (!IsSetting)// 처음 셋팅
	{
		CGameInstance::Get_Instance()->StopSound(CSoundMgr::EFFECT);
		CGameInstance::Get_Instance()->PlayMySound(L"Spongebobturen.ogg", CSoundMgr::EFFECT, 0.5f);
		
		IsSetting = true;
		m_bIsDirector1 = true;
		m_iClise_Up = 0;

		//플레이어 위치 정보 얻기
		CTransform* m_pPatrickTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_PatrickPlayer"), TEXT("Com_Transform"));
		m_pPatrickTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(35.5f, 0.f, 32.7f, 1));
		m_pPatrickTransform->Rotation(XMLoadFloat3(&_float3(0.f, 1.f, 0.f)), 90);
			
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(35.9, 2, 37.f, 1));
		m_pTransformCom->LookAt(XMVectorSet(35.4, 1, 32.1, 1));

		
		CCut* pCut = static_cast<CCut*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cut")));
		pCut->Set_Render(true);
		pCut->Set_TexutureNum(7);

	}
	_bool IsEnd = m_pTransformCom->Chase(XMVectorSet(35.4, 1, 32.1, 1), TimeDelta * 0.2f, 3);

	if (IsEnd)
	{
		m_bIsDirector_End = true;
		CCut* pCut = static_cast<CCut*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Cut")));
		pCut->Set_Render(false);
	}


	Safe_Release(pGameInstance);
}

void CCamera_Free::Boss1_DirectorCount_4(_double TimeDelta)
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	static _bool IsSetting = false;
	static _vector		vLook;
	static _vector		vEye;
	//_vector m_vScoreBoardPos = XMVectorSet(0, 0, 0, 1);

	//static _vector m_vRound2_ParickPos = XMVectorSet(0, 0, 0, 1);

	if (!IsSetting)// 처음 셋팅
	{
		CSnadySmokeEffect* pCSnadySmokeEffect = static_cast<CSnadySmokeEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SnadySmokeEffect")));
		pCSnadySmokeEffect->IsRender(false);
		pCSnadySmokeEffect = static_cast<CSnadySmokeEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SnadySmokeEffect"), 1));
		pCSnadySmokeEffect->IsRender(false);
		pCSnadySmokeEffect = static_cast<CSnadySmokeEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_SnadySmokeEffect"), 2));
		pCSnadySmokeEffect->IsRender(false);

		CBubblePangEffect* pBubblePangEffect = static_cast<CBubblePangEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_BubblePangEffect")));
		CBubblePangEffect* pBubblePangEffect2 = static_cast<CBubblePangEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_BubblePangEffect"), 1));
		CBoss_Sandy_Body* pSandtBody = static_cast<CBoss_Sandy_Body*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body")));


		pBubblePangEffect->IsRender(false);
		pBubblePangEffect2->IsRender(false);

		pSandtBody->Set_IsNotRed();
		CGlowBlue2* pGlowBlue = static_cast<CGlowBlue2*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrowBlue2")));
		pGlowBlue->IsRender(false);
		CGlowBlue2* pGlowBlue2 = static_cast<CGlowBlue2*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrowBlue2"), 1));
		pGlowBlue2->IsRender(false);
		CElectric* pElect = static_cast<CElectric*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ElectircEffect"), 2));
		pElect->IsRender(false);
		CElectric* pElect2 = static_cast<CElectric*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_ElectircEffect"), 3));
		pElect2->IsRender(false);



		CBlackBackGround* pBlack = static_cast<CBlackBackGround*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_BlackBackGround")));
		pBlack->Set_IsRender(true);
		CBlackBackGround* pBlack2 = static_cast<CBlackBackGround*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_BlackBackGround"),1));
		pBlack2->Set_IsRender(true);


		
		IsSetting = true;
		m_iClise_Up = 0;
		m_bIsDirector1 = true;


		CKing_Neptune* pKing_Neptune = static_cast<CKing_Neptune*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_King_Neptune")));
		
		if(pKing_Neptune != nullptr)
			pKing_Neptune->Set_Dead();

		_vector m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);



		CTransform* m_pSandyTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Body"), TEXT("Com_Transform"));
		_vector m_vSandtPos = m_pSandyTransform->Get_State(CTransform::STATE_POSITION);



		m_pSandyTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(47.56, 0.f, 52, 1));

		CTransform* m_pSandy_HeadTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Sandy_Head"), TEXT("Com_Transform"));
		m_pSandy_HeadTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(47.56, 0.f, 52, 1));
		
		


		vLook = m_pSandy_HeadTransform->Get_State(CTransform::STATE_POSITION);
		vEye = XMVectorSet(Direcotor_cameradesc.m_vFirstDirecotorCamera[8].fPos.x, Direcotor_cameradesc.m_vFirstDirecotorCamera[8].fPos.y, Direcotor_cameradesc.m_vFirstDirecotorCamera[8].fPos.z, 1);



		m_pSandyTransform->LookAt(XMVectorSet( Direcotor_cameradesc.m_vFirstDirecotorCamera[8].fPos.x,0, Direcotor_cameradesc.m_vFirstDirecotorCamera[8].fPos.z, 1));
		m_pSandy_HeadTransform->LookAt(XMVectorSet(Direcotor_cameradesc.m_vFirstDirecotorCamera[8].fPos.x, 0, Direcotor_cameradesc.m_vFirstDirecotorCamera[8].fPos.z, 1));

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEye);
		m_pTransformCom->LookAt(vLook + XMVectorSet(0,5,0,1));

		CGameInstance::Get_Instance()->StopSound(CSoundMgr::EFFECT);
		CGameInstance::Get_Instance()->PlayMySound(L"End.ogg", CSoundMgr::EFFECT, 0.5f);

	}
	if (m_bIsDirector1)
	{
		m_iClise_Up++;

		if (m_iClise_Up >= 380 && m_iClise_Up <= 425)
		{
			//카메라 쉐이크
			//_vector		vEye = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			//	if (m_iShackCount > 10)
			//	{
			//		m_IsShake = false;
			//		m_fX = 0.1f;
			//		m_fY = 0.2f;
			//	}
			//	m_iShackCount++;
			//	if (m_iShackCount % 10 == 0)
			//	{
			//		m_fX *= -1;
			//		m_fY *= -1;
			//	}

			//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEye + XMVectorSet(m_fX, m_fY, 0, 1));
			//	if (m_fX < 0)
			//	{
			//		m_fX -= 0.002;
			//		m_fY -= 0.002;

			//	}
			//


		}

		if (m_iClise_Up >= 450)
		{
			m_bIsDirector1 = false;
			m_bIsDirector2 = true;
			m_iClise_Up = 0;
			m_iDirectorCount = 4;
		}


		

	}

	

	Safe_Release(pGameInstance);
}
