#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::NativeConstruct(aiAnimation* pAIAnim, CModel* pModel)
{
	strcpy_s(m_szName, pAIAnim->mName.data);

	m_Duration = pAIAnim->mDuration;
	m_TickPerSecond = pAIAnim->mTicksPerSecond;

	m_iNumChannels = pAIAnim->mNumChannels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::Create(pAIAnim->mChannels[i], pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

void CAnimation::Update_TransformationMatrices(_double TimeDelta, _bool _IsOnly, _uint size)
{
	m_TimeAcc += m_TickPerSecond * TimeDelta;
	m_bNextFinish = false;
	if (m_TimeAcc >= m_Duration/* + 0.2*/)
	{

		m_isFinished = true;
		if (_IsOnly)
		{
			m_TimeAcc = 0.0;
		}
		else
			return;

	}
	else
		m_isFinished = false;


	for (_uint i = 0; i < m_Channels.size(); ++i)
	{


		m_Channels[i]->Update_TransformationMatrices(m_TimeAcc, size);

	}


}


void CAnimation::Update_TransformationMatrices_Next(_double TimeDelta, _bool _IsChane, vector<class CChannel*>  curAnim)
{
	fRatio = 0.2f;
	m_isFinished = false;

	m_TimeAcc = 0.0;

	for (_uint i = 0; i < m_Channels.size(); ++i)
	{
		m_Channels[i]->Update_TransformationMatricesNext(m_TimeAcc, curAnim[i]->Get_keyframe(), fRatio);
	}

	m_bNextFinish = true;
}


_bool CAnimation::Get_IsFinish()
{
	return m_isFinished;
}

CAnimation* CAnimation::Create(aiAnimation* pAIAnim, CModel* pModel)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->NativeConstruct(pAIAnim, pModel)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();

}
