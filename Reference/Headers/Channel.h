#pragma once

#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
public:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT NativeConstruct(aiNodeAnim* pChannel, class CModel* pModel);
	void Update_TransformationMatrices(_double TimeAcc ,_float size);
	void Update_TransformationMatricesNext(_double TimeAcc,  vector<KEYFRAME> _CurKeyframe, _float fRatio);

	
	vector<KEYFRAME> Get_keyframe();// { return m_KeyFrames; }
	_bool Get_Finish() { return m_bISFinish; }
	void Set_Finish(_bool _isFinish) { m_bISFinish = _isFinish; }
//	void Set_vFrame(vector<KEYFRAME> _frame) { m_PastKeyFrames = _frame; }

	_bool Get_NextFinish() { return m_bNextISFinish; }
	_bool Get_Last() { return bIsLast; }
	void Set_Last(_bool _Islast) { bIsLast = _Islast; }

private:
	char			m_szName[MAX_PATH] = "";

	_uint					m_iPastKeyFrame = 0;
	_uint					m_iCurrentKeyFrame = 0;

	_uint					m_iNumMaxKeyFrames = 0;
	vector<KEYFRAME>		m_KeyFrames;
	vector<KEYFRAME>		m_CurKeyFrames;

	class CHierarchyNode*	m_pHierarchyNode = nullptr;
	_bool m_isTurn = false;


	_bool m_bISFinish = false;
	_bool m_bNextISFinish = false;

	 _float fRatio = 0;
	  _bool bIsLast = false;

public:
	static CChannel* Create(aiNodeAnim* pChannel, class CModel* pModel);
	virtual void Free() override;
};

END