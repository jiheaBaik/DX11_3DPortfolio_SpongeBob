#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
public:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT NativeConstruct(aiAnimation* pAIAnim, class CModel* pModel);
	void Update_TransformationMatrices(_double TimeDelta, _bool _isChange, _uint size);
	void Update_TransformationMatrices_Next(_double TimeDelta, _bool _isChange, vector<class CChannel*> curAnim);

	
	_bool Get_IsFinish();// {return m_isFinished; }
	void Set_IsFinish(_bool isFinished) { m_isFinished = isFinished;  }


	_bool Get_NextFinish() { return m_bNextFinish; }
private:
	char						m_szName[MAX_PATH] = "";
	_double						m_Duration = 0.0;
	_double						m_TickPerSecond = 0.0;

	_bool						m_isFinished = false;

	_uint						m_iNumChannels = 0;
	vector<class CChannel*>		m_Channels;
	vector<class CAnimation*>				m_Animations;
	_double						m_TimeAcc = 0.0;
	_bool m_bISFinish = false;

	_uint					m_iCurrentKeyFrame = 0;
	vector<KEYFRAME>		m_KeyFrames;
	vector<KEYFRAME>		m_CurKeyFrames;
	_bool m_bNextFinish = false;

	  _float fRatio = 0;
public:
	//vector<KEYFRAME> Get_keyframe();
	_bool Get_Finish() { return m_bISFinish; }
	void Set_Finish(_bool _isFinish) { m_bNextFinish = _isFinish; }
	void Set_Zero(_uint _value) { m_TimeAcc = _value; }
	vector<class CChannel*>	 Get_m_Channels() { return m_Channels; }

public:
	static CAnimation* Create(aiAnimation* pAIAnim, class CModel* pModel);
	virtual void Free() override;
};

END