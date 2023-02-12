#include "..\Public\Channel.h"

#include "..\Public\Model.h"
#include "HierarchyNode.h"

CChannel::CChannel()
{
}

HRESULT CChannel::NativeConstruct(aiNodeAnim * pChannel, CModel* pModel)
{
	strcpy_s(m_szName, pChannel->mNodeName.data);

	m_pHierarchyNode = pModel->Get_HierarchyNodes(m_szName);
	Safe_AddRef(m_pHierarchyNode);

	m_iNumMaxKeyFrames = max(pChannel->mNumScalingKeys, pChannel->mNumRotationKeys);
	m_iNumMaxKeyFrames = max(m_iNumMaxKeyFrames, pChannel->mNumPositionKeys);

	_float3		vScale;
	_float4		vRotation;
	_float4		vPosition;

	_double		Time = 0.0;

	for (_uint i = 0; i < m_iNumMaxKeyFrames; ++i)
	{
		KEYFRAME			KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

		if (i < pChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &pChannel->mScalingKeys[i].mValue, sizeof(_float3));
			Time = pChannel->mScalingKeys[i].mTime;
		}

		if (i < pChannel->mNumRotationKeys)
		{
			vRotation.x = pChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pChannel->mRotationKeys[i].mValue.w;
			Time = pChannel->mRotationKeys[i].mTime;
		}

		if (i < pChannel->mNumPositionKeys)
		{
			memcpy(&vPosition, &pChannel->mPositionKeys[i].mValue, sizeof(_float3));
			vPosition.w = 1.f;
			Time = pChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;
		KeyFrame.Time = Time;

		m_KeyFrames.push_back(KeyFrame);

	}

	return S_OK;
}

void CChannel::Update_TransformationMatrices(_double TimeAcc, _float size)
{
	_vector			vScale;
	_vector			vRotation;
	_vector			vPosition;
	
//	if (bIsfrist)
//	{
		if (TimeAcc < m_KeyFrames[1].Time)
			m_iCurrentKeyFrame = 0;

		//bIsfrist = false;
	//}
	
	/*if (size == 0)
		m_iCurrentKeyFrame = 0;*/
	//else
	//	int a = 0;
	if (TimeAcc >= m_KeyFrames.back().Time)
	{
		vScale = XMLoadFloat3(&m_KeyFrames.back().vScale);
		vRotation = XMLoadFloat4(&m_KeyFrames.back().vRotation);
		vPosition = XMLoadFloat4(&m_KeyFrames.back().vPosition);
		m_CurKeyFrames = m_KeyFrames;

		bIsLast = true;

	/*	_float		fRatio = (TimeAcc - m_KeyFrames[m_iCurrentKeyFrame].Time)
			/ (m_KeyFrames.back().Time - m_KeyFrames.front().Time);

		_vector		vSourScale, vDestScale;
		_vector		vSourRotation, vDestRotation;
		_vector		vSourPosition, vDestPosition;

		vSourScale = XMLoadFloat3(&m_KeyFrames.front().vScale);
		vDestScale = XMLoadFloat3(&m_KeyFrames.back().vScale);

		vSourRotation = XMLoadFloat4(&m_KeyFrames.front().vRotation);
		vDestRotation = XMLoadFloat4(&m_KeyFrames.back().vRotation);

		vSourPosition = XMLoadFloat4(&m_KeyFrames.front().vPosition);
		vDestPosition = XMLoadFloat4(&m_KeyFrames.back().vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		m_CurKeyFrames = m_KeyFrames;*/

	}
	else
	{
	//	m_bISFinish = false;

		while (TimeAcc > m_KeyFrames[m_iCurrentKeyFrame + 1].Time)
		{ 
			++m_iCurrentKeyFrame;
		}

		_float		fRatio = (TimeAcc - m_KeyFrames[m_iCurrentKeyFrame].Time)
			/ (m_KeyFrames[m_iCurrentKeyFrame + 1].Time - m_KeyFrames[m_iCurrentKeyFrame].Time);

		_vector		vSourScale, vDestScale;
		_vector		vSourRotation, vDestRotation;
		_vector		vSourPosition, vDestPosition;

		vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame].vScale);
		vDestScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1].vScale);

		vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame].vRotation);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame + 1].vRotation);

		vSourPosition = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame].vPosition);
		vDestPosition = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame + 1].vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);


	}



	_matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pHierarchyNode->SetUp_TransformationMatrix(TransformationMatrix);

}

void CChannel::Update_TransformationMatricesNext(_double TimeAcc, vector<KEYFRAME> _CurKeyframe, _float fRatio)
{
	
	/*if (fRatio >= 1)
	{
		fRatio = 0;
		m_bNextISFinish = true;
		return;
	}
	else
		m_bNextISFinish = false;
		

	fRatio += 0.01;*/

	_vector			vScale;
	_vector			vRotation;
	_vector			vPosition;


	_vector		vSourScale, vDestScale;
	_vector		vSourRotation, vDestRotation;
	_vector		vSourPosition, vDestPosition;

	//위 함수에서 m_iCurrentKeyFrame + 1 로 끝났기에 m_iCurrentKeyFrame + 1부터시작해야 됨
	//혹시 키프레임이 하나면 위에서 터졌겠지만 혹시 모르니.. 사이즈가 m_iCurrentKeyFrame + 1보다 어쩌구~~ 
	//나중에 터지면 이 예외처리 잊지말고 하기

	vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1].vScale);
	vDestScale = XMLoadFloat3(&_CurKeyframe[0].vScale);

	vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame + 1].vRotation);
	vDestRotation = XMLoadFloat4(&_CurKeyframe[0].vRotation);

	vSourPosition = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame + 1].vPosition);
	vDestPosition = XMLoadFloat4(&_CurKeyframe[0].vPosition);

	vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
	vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
	vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);


	_matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pHierarchyNode->SetUp_TransformationMatrix(TransformationMatrix);
}

vector<KEYFRAME> CChannel::Get_keyframe()
{
	return m_KeyFrames;
}

CChannel * CChannel::Create(aiNodeAnim * pChannel, CModel* pModel)
{
	CChannel*		pInstance = new CChannel();

	if (FAILED(pInstance->NativeConstruct(pChannel, pModel)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChannel::Free()
{
	Safe_Release(m_pHierarchyNode);

	m_KeyFrames.clear();
}
