#include "..\Public\MeshContainer.h"
#include "Model.h"
#include "HierarchyNode.h"

CMeshContainer::CMeshContainer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CMeshContainer::CMeshContainer(const CMeshContainer & Prototype)
	: CVIBuffer(Prototype)
	, m_iMaterialIndex(Prototype.m_iMaterialIndex)
	, m_HierarchyNodes(Prototype.m_HierarchyNodes)
	, m_iNumBones(Prototype.m_iNumBones)
{
	strcpy_s(m_szName, Prototype.m_szName);

	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_AddRef(pHierarchyNode);
}

HRESULT CMeshContainer::NativeConstruct_Prototype(CModel::TYPE eModelType, aiMesh * pAIMesh, CModel* pModel, _fmatrix PivotMatrix)
{
	strcpy_s(m_szName, pAIMesh->mName.data);

	m_iMaterialIndex = pAIMesh->mMaterialIndex;

	if (CModel::TYPE_ANIM == eModelType)
	{
		if (FAILED(SetUp_AnimVertices(pAIMesh, pModel)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(SetUp_Vertices(pAIMesh, PivotMatrix)))
			return E_FAIL;
	}

	m_iNumPrimitive = pAIMesh->mNumFaces;
	m_iNumIndicesPerPrimitive = 3;
	m_iBytePerPrimitive = sizeof(FACEINDICES32);
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iBytePerPrimitive * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);
	FACEINDICES32*			pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);
	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		((FACEINDICES32*)m_pIndices)[i]._1 = pIndices[i]._1 = pAIMesh->mFaces[i].mIndices[0];
		((FACEINDICES32*)m_pIndices)[i]._2 = pIndices[i]._2 = pAIMesh->mFaces[i].mIndices[1];
		((FACEINDICES32*)m_pIndices)[i]._3 = pIndices[i]._3 = pAIMesh->mFaces[i].mIndices[2];

		/*((FACEINDICES32*)m_pIndices)[iNumFaces]._1 = pIndices[i]._1 = pAIMesh->mFaces[i].mIndices[0];
		((FACEINDICES32*)m_pIndices)[iNumFaces]._2 = pIndices[i]._2 = pAIMesh->mFaces[i].mIndices[1];
		((FACEINDICES32*)m_pIndices)[iNumFaces]._3 = pIndices[i]._3 = pAIMesh->mFaces[i].mIndices[2];*/

	}

	ZeroMemory(&m_BufferSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_BufferSubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CMeshContainer::NativeConstruct(void * pArg)
{
	CModel*		pModel = (CModel*)pArg;



	vector<CHierarchyNode*>		HierarchyNodes;

	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		CHierarchyNode*		pNewNode = pModel->Get_HierarchyNodes(pHierarchyNode->Get_Name());

		_float4x4		OffsetMatrix;

		XMStoreFloat4x4(&OffsetMatrix, pHierarchyNode->Get_OffsetMatrix());

		pNewNode->SetUp_OffsetMatrix(OffsetMatrix);

		HierarchyNodes.push_back(pNewNode);
		Safe_AddRef(pNewNode);// 추가
		Safe_Release(pHierarchyNode);
	}

	m_HierarchyNodes.clear();

	m_HierarchyNodes = HierarchyNodes;

	return S_OK;
}

HRESULT CMeshContainer::SetUp_Vertices(aiMesh * pAIMesh, _fmatrix PivotMatrix)
{
	m_iStride = sizeof(VTXMODEL);
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	VTXMODEL*			pVertices = new VTXMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		_uint iIndex = i;

		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));
		m_pVerticesPos[iIndex] = pVertices[i].vPosition;

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));

		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	ZeroMemory(&m_BufferSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_BufferSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::SetUp_AnimVertices(aiMesh * pAIMesh, CModel* pModel)
{
	m_iStride = sizeof(VTXANIMMODEL);
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXANIMMODEL*			pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	/* 현재 메시에 영향을 주는 뼈들의 갯수. */
	m_iNumBones = pAIMesh->mNumBones;



	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone*		pAIBone = pAIMesh->mBones[i];

		_float4x4	OffsetMatrix;
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		CHierarchyNode*	pNode = pModel->Get_HierarchyNodes(pAIBone->mName.data);

		pNode->SetUp_OffsetMatrix(OffsetMatrix);

		m_HierarchyNodes.push_back(pNode);

		Safe_AddRef(pNode);

		/* i번째 뼈는 몇개의 정점에 영향을 주는지?! */
		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.x = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.y = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.z = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.w)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.w = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		CHierarchyNode*		pNode = pModel->Get_HierarchyNodes(m_szName);

		if (nullptr != pNode)
		{
			m_HierarchyNodes.push_back(pNode);
			Safe_AddRef(pNode);// 추가
			m_iNumBones = 1;
		}

	}

	ZeroMemory(&m_BufferSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_BufferSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::SetUp_BoneMatrices(_float4x4 * pBoneMatrices, _fmatrix PivotMatrix)
{
	_uint			iBoneIndex = 0;

	if (m_HierarchyNodes.empty())
	{
		XMStoreFloat4x4(&pBoneMatrices[0], XMMatrixIdentity());
		return S_OK;
	}

	for (auto& pHierarchyNodes : m_HierarchyNodes)
	{
		XMStoreFloat4x4(&pBoneMatrices[iBoneIndex++], XMMatrixTranspose(pHierarchyNodes->Get_OffsetMatrix() * pHierarchyNodes->Get_CombinedTransformationMatrix() * PivotMatrix));
	}

	return S_OK;
}

CMeshContainer * CMeshContainer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CModel::TYPE eModelType, aiMesh * pAIMesh, class CModel* pModel, _fmatrix PivotMatrix)
{
	CMeshContainer*		pInstance = new CMeshContainer(pDevice, pContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eModelType, pAIMesh, pModel, PivotMatrix)))
	{
		MSG_BOX("Failed to Created : CMeshContainer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CMeshContainer::Clone(void * pArg)
{
	CMeshContainer*		pInstance = new CMeshContainer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMeshContainer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeshContainer::Free()
{
	__super::Free();

	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		if(pHierarchyNode == nullptr)
		{
			int a = 0;
		}

		if (pHierarchyNode == NULL)
		{
			int a = 0;
		}
		Safe_Release(pHierarchyNode);

	}

	m_HierarchyNodes.clear();
}
