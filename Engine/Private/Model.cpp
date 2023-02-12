#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "HierarchyNode.h"
#include "Texture.h"
#include "Shader.h"
#include "Animation.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel & Prototype)
	: CComponent(Prototype)
	, m_MeshContainers(Prototype.m_MeshContainers)
	, m_Materials(Prototype.m_Materials)
	, m_iNumMeshContainers(Prototype.m_iNumMeshContainers)
	, m_iNumMaterials(Prototype.m_iNumMaterials)
	, m_PivotMatrix(Prototype.m_PivotMatrix)
	, m_iNumAnimations(Prototype.m_iNumAnimations)
	, m_iCurrentAnimIndex(Prototype.m_iCurrentAnimIndex)
	, m_pScene(Prototype.m_pScene)
	, m_eModelType(Prototype.m_eModelType)
	//, m_Animations(Prototype.m_Animations)
{
	for (auto& pMeshContainer : m_MeshContainers)
		Safe_AddRef(pMeshContainer);

	for (auto& MaterialDesc : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(MaterialDesc.pMaterialTexture[i]);
	}
}

CHierarchyNode * CModel::Get_HierarchyNodes(const char * pHierarchyNodeName)
{
	auto	iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&](CHierarchyNode* pNode)
	{
		return !strcmp(pHierarchyNodeName, pNode->Get_Name());
	});

	return *iter;
}

_float4x4 * CModel::Get_BoneMatrixPtr(const char * pHierarchyNodeName)
{
	CHierarchyNode*		pNode = Get_HierarchyNodes(pHierarchyNodeName);
	if (nullptr == pNode)
		return nullptr;

	return pNode->Get_CombinedTransformationMatrixPtr();
}

_float4x4 CModel::Get_BoneOffsetMatrix(const char * pHierarchyNodeName)
{
	_float4x4		OffsetMatrix;

	XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());

	CHierarchyNode*		pNode = Get_HierarchyNodes(pHierarchyNodeName);
	if (nullptr == pNode)
		return OffsetMatrix;

	XMStoreFloat4x4(&OffsetMatrix, pNode->Get_OffsetMatrix());

	return OffsetMatrix;
}

HRESULT CModel::NativeConstruct_Prototype(TYPE eModelType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix)
{
	char		szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	_uint		iFlag = 0;

	m_eModelType = eModelType;

	if (TYPE_NONANIM == eModelType)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace;

	m_pScene = m_Importer.ReadFile(szFullPath, iFlag);

	if (nullptr == m_pScene)
		return E_FAIL;

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	if (FAILED(Create_HierarchyNodes(m_pScene->mRootNode, nullptr, 0)))
		return E_FAIL;

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	if (FAILED(Create_MeshContainers(PivotMatrix)))
		return E_FAIL;

	if (FAILED(Create_Materials(pModelFilePath)))
		return E_FAIL;


	if (FAILED(Create_Animation()))
		return E_FAIL;


	return S_OK;
}

HRESULT CModel::NativeConstruct(void * pArg)
{
	if (FAILED(Create_HierarchyNodes(m_pScene->mRootNode, nullptr, 0)))
		return E_FAIL;

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	vector<CMeshContainer*>		MeshContainers;

	for (auto& pMeshContainer : m_MeshContainers)
	{
		MeshContainers.push_back((CMeshContainer*)pMeshContainer->Clone(this));

		Safe_Release(pMeshContainer);
	}

	m_MeshContainers.clear();

	m_MeshContainers = MeshContainers;

	if (FAILED(Create_Animation()))
		return E_FAIL;
	//m_iPastAnimIndex = m_iCurrentAnimIndex;

	return S_OK;
}

void CModel::Update_Animation(_double TimeDelta)
{
	static _uint m_iPastAnimIndex = m_iCurrentAnimIndex;
	if (m_iCurrentAnimIndex >= m_iNumAnimations)
		return;

	vector<KEYFRAME> _pastKeyframe;
	vector<KEYFRAME> _curentKeyframe;


	m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(TimeDelta, true, 1);
	/* 부모로부터 자식뼈에게 누적시켜 전달한다.(CombinedTransformationMatrix) */
	for (auto& pHierarchyNode : m_HierarchyNodes)
		pHierarchyNode->Update_CombinedTransformationMatrix();

	//Set_IsFinish(m_Animations[m_iCurrentAnimIndex]->Get_IsFinish());

}

void CModel::Update_SpongeBobAnimation(_double TimeDelta, _uint _Currentindex, _bool _isOnly, _uint size)
{


	if (m_iCurrentAnimIndex >= m_iNumAnimations)
		return;
	//만약 하나의 애니메이션으로 반복하는 녀석이면
	if (_isOnly)
	{
		//하나만 수행 하도록 한다.
		m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(TimeDelta, _isOnly, size);
		if (m_Animations[m_iCurrentAnimIndex]->Get_IsFinish())
			m_bIsfinish = true;
		else
			m_bIsfinish = false;
	}
	else
	{
		//애니메이션이 끝났다면
		if (m_Animations[m_iCurrentAnimIndex]->Get_IsFinish())
		{

			//다음 애니메이션을 가져와 선형보간을 한다
			m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices_Next(TimeDelta, _isOnly, m_Animations[_Currentindex]->Get_m_Channels());
			bNotNext = true;
		}
		else
		{

			//애니메이션이 끝나지 않았다면 선생님 코드를 수행한다 (하나의 애니메이션의 키프레임끼리 선형보간)
			m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(TimeDelta, _isOnly, size);
			bNotNext = false;
		}
		//만약 다음애니메이션끼리 선형보간이 끝났다면
		if (m_Animations[m_iCurrentAnimIndex]->Get_NextFinish())
		{
			//현재의 다음애니메이션의 초기화를 수행한다.
			m_Animations[_Currentindex]->Set_IsFinish(false);
			m_Animations[_Currentindex]->Set_Zero(0.0);
			//그리고 다음애니메이션의 선형보간이 끝났다는 bool값을 설정해  객체가 알도록 한다(다음 애니메이션을 넘길 수 있는 타이밍을 전하기 위함)
			m_bNextISFinish = true;
			//현재의 애니메이션의 설정도 초기화해준다.
			m_Animations[m_iCurrentAnimIndex]->Set_Finish(false);
			//기존과 현재를 바꾼다.
			m_iPastAnimIndex = m_iCurrentAnimIndex;
		}
		else
		{
			m_bNextISFinish = false;

		}
	}



	/* 부모로부터 자식뼈에게 누적시켜 전달한다.(CombinedTransformationMatrix) */
	for (auto& pHierarchyNode : m_HierarchyNodes)
		pHierarchyNode->Update_CombinedTransformationMatrix();

	Set_IsFinish(m_Animations[m_iCurrentAnimIndex]->Get_IsFinish());

}

void CModel::Update_Boss_Sandy_Head_Animation(_double TimeDelta, _uint _Currentindex, _bool _isOnly, _uint size)
{
	
	if (m_iCurrentAnimIndex >= m_iNumAnimations)
		return;
	//만약 하나의 애니메이션으로 반복하는 녀석이면
	if (_isOnly)
	{
		//하나만 수행 하도록 한다.
		m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(TimeDelta, _isOnly, size);
	}
	else
	{
		//애니메이션이 끝났다면
		if (m_Animations[m_iCurrentAnimIndex]->Get_IsFinish())
		{

			//다음 애니메이션을 가져와 선형보간을 한다
			m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices_Next(TimeDelta, _isOnly, m_Animations[_Currentindex]->Get_m_Channels());
			bNotNext = true;
		}
		else
		{

			//애니메이션이 끝나지 않았다면 선생님 코드를 수행한다 (하나의 애니메이션의 키프레임끼리 선형보간)
			m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(TimeDelta, _isOnly, size);
			bNotNext = false;
		}
		//만약 다음애니메이션끼리 선형보간이 끝났다면
		if (m_Animations[m_iCurrentAnimIndex]->Get_NextFinish())
		{
			//현재의 다음애니메이션의 초기화를 수행한다.
			m_Animations[_Currentindex]->Set_IsFinish(false);
			m_Animations[_Currentindex]->Set_Zero(0.0);
			//그리고 다음애니메이션의 선형보간이 끝났다는 bool값을 설정해  객체가 알도록 한다(다음 애니메이션을 넘길 수 있는 타이밍을 전하기 위함)
			m_bNextISFinish = true;
			//현재의 애니메이션의 설정도 초기화해준다.
			m_Animations[m_iCurrentAnimIndex]->Set_Finish(false);
			//기존과 현재를 바꾼다.
			m_iPastAnimIndex = m_iCurrentAnimIndex;
		}
		else
		{
			m_bNextISFinish = false;

		}
	}

	

	/* 부모로부터 자식뼈에게 누적시켜 전달한다.(CombinedTransformationMatrix) */
	for (auto& pHierarchyNode : m_HierarchyNodes)
		pHierarchyNode->Update_CombinedTransformationMatrix();

	Set_IsFinish(m_Animations[m_iCurrentAnimIndex]->Get_IsFinish());
	
}
void CModel::Update_Boss_Sandy_Animation(_double TimeDelta, _uint _Currentindex, _bool _isOnly, _uint size)
{

	if (m_iCurrentAnimIndex >= m_iNumAnimations)
		return;
	//만약 하나의 애니메이션으로 반복하는 녀석이면
	if (_isOnly)
	{
		//하나만 수행 하도록 한다.
		m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(TimeDelta, _isOnly, size);
	}
	else
	{
		//애니메이션이 끝났다면
		if (m_Animations[m_iCurrentAnimIndex]->Get_IsFinish())
		{

			//다음 애니메이션을 가져와 선형보간을 한다
			m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices_Next(TimeDelta, _isOnly, m_Animations[_Currentindex]->Get_m_Channels());
			bNotNext = true;
		}
		else
		{

			//애니메이션이 끝나지 않았다면 선생님 코드를 수행한다 (하나의 애니메이션의 키프레임끼리 선형보간)
			m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(TimeDelta, _isOnly, size);
			bNotNext = false;
		}
		//만약 다음애니메이션끼리 선형보간이 끝났다면
		if (m_Animations[m_iCurrentAnimIndex]->Get_NextFinish())
		{
			//현재의 다음애니메이션의 초기화를 수행한다.
			m_Animations[_Currentindex]->Set_IsFinish(false);
			m_Animations[_Currentindex]->Set_Zero(0.0);
			//그리고 다음애니메이션의 선형보간이 끝났다는 bool값을 설정해  객체가 알도록 한다(다음 애니메이션을 넘길 수 있는 타이밍을 전하기 위함)
			m_bNextISFinish = true;
			//현재의 애니메이션의 설정도 초기화해준다.
			m_Animations[m_iCurrentAnimIndex]->Set_Finish(false);
			//기존과 현재를 바꾼다.
			m_iPastAnimIndex = m_iCurrentAnimIndex;
		}
		else
		{
			m_bNextISFinish = false;

		}
	}



	/* 부모로부터 자식뼈에게 누적시켜 전달한다.(CombinedTransformationMatrix) */
	for (auto& pHierarchyNode : m_HierarchyNodes)
		pHierarchyNode->Update_CombinedTransformationMatrix();

	Set_IsFinish(m_Animations[m_iCurrentAnimIndex]->Get_IsFinish());

}
void CModel::Update_King_NeptuneAnimation(_double TimeDelta)
{
	if (m_iCurrentAnimIndex >= m_iNumAnimations)
		return;

	vector<KEYFRAME> _pastKeyframe;
	vector<KEYFRAME> _curentKeyframe;

	m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(TimeDelta, true,1);
	/* 부모로부터 자식뼈에게 누적시켜 전달한다.(CombinedTransformationMatrix) */
	for (auto& pHierarchyNode : m_HierarchyNodes)
		pHierarchyNode->Update_CombinedTransformationMatrix();


}

HRESULT CModel::Bind_SRV(CShader * pShader, const char * pConstantName, _uint iMeshContainerIndex, aiTextureType eTextureType)
{
	_uint	iMaterialIndex = m_MeshContainers[iMeshContainerIndex]->Get_MaterialIndex();

	return m_Materials[iMaterialIndex].pMaterialTexture[eTextureType]->SetUp_SRV(pShader, pConstantName, 0);
}

HRESULT CModel::Render(_uint iMeshContainerIndex, CShader* pShader, _uint iPassIndex)
{
	if (nullptr != m_MeshContainers[iMeshContainerIndex])
	{
		if (TYPE_ANIM == m_eModelType)
		{

			_float4x4		BoneMatrices[256];

			m_MeshContainers[iMeshContainerIndex]->SetUp_BoneMatrices(BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));

			if (FAILED(pShader->Set_RawValue("g_Bones", BoneMatrices, sizeof(_float4x4) * 256)))
				int a = 10;
		}

		pShader->Begin(iPassIndex);

		m_MeshContainers[iMeshContainerIndex]->Render();
	}

	return S_OK;
}

HRESULT CModel::Create_MeshContainers(_fmatrix PivotMatrix)
{
	m_iNumMeshContainers = m_pScene->mNumMeshes;

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pContext, m_eModelType, m_pScene->mMeshes[i], this, PivotMatrix);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_MeshContainers.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Create_Materials(const char* pModelFilePath)
{
	m_iNumMaterials = m_pScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial*		pAIMaterial = m_pScene->mMaterials[i];

		MODELMATERIALDESC			ModelMaterial;
		ZeroMemory(&ModelMaterial, sizeof(MODELMATERIALDESC));


		for (_uint j = 1; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString		strTexturePath;
			char			szFullPath[MAX_PATH] = "";

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strTexturePath)))
				continue;

			char			szFileName[MAX_PATH] = "";
			char			szExt[MAX_PATH] = "";

			_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			_tchar			szRealFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szRealFullPath, MAX_PATH);

			ModelMaterial.pMaterialTexture[j] = CTexture::Create(m_pDevice, m_pContext, szRealFullPath, 1);
			if (nullptr == ModelMaterial.pMaterialTexture[j])
				return E_FAIL;
		}

		m_Materials.push_back(ModelMaterial);
	}

	return S_OK;
}

HRESULT CModel::Create_HierarchyNodes(aiNode * pNode, CHierarchyNode * pParent, _uint iDepth)
{
	if (nullptr == pNode)
		return S_OK;

	_float4x4		TransformationMatrix;
	memcpy(&TransformationMatrix, &pNode->mTransformation, sizeof(_float4x4));

	CHierarchyNode*			pHierarchyNode = CHierarchyNode::Create(pNode->mName.data, XMLoadFloat4x4(&TransformationMatrix), pParent, iDepth);
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_HierarchyNodes.push_back(pHierarchyNode);

	for (_uint i = 0; i < pNode->mNumChildren; ++i)
	{
		Create_HierarchyNodes(pNode->mChildren[i], pHierarchyNode, iDepth + 1);
	}

	return S_OK;
}

HRESULT CModel::Create_Animation()
{
	m_iNumAnimations = m_pScene->mNumAnimations;

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		aiAnimation*		pAIAnim = m_pScene->mAnimations[i];

		CAnimation*		pAnimation = CAnimation::Create(pAIAnim, this);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eModelType, const char * pModelFilePath, const char * pModelFileName, _fmatrix PivotMatrix)
{
	CModel*		pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eModelType, pModelFilePath, pModelFileName, PivotMatrix)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*		pInstance = new CModel(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	m_Animations.clear();

	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_Release(pHierarchyNode);
	m_HierarchyNodes.clear();

	for (auto& MaterialDesc : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(MaterialDesc.pMaterialTexture[i]);

		/*if (false == m_isCloned)
			Safe_Delete(pMaterialDesc);*/
	}

	for (auto& pMeshContainer : m_MeshContainers)
		Safe_Release(pMeshContainer);

	m_MeshContainers.clear();

	m_Importer.FreeScene();
}
