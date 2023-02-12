#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class CMeshContainer final : public CVIBuffer
{
public:
	CMeshContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMeshContainer(const CMeshContainer& Prototype);
	virtual ~CMeshContainer() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT NativeConstruct_Prototype(CModel::TYPE eModelType, aiMesh* pAIMesh, class CModel* pModel, _fmatrix PivotMatrix);
	virtual HRESULT NativeConstruct(void* pArg);

public:
	HRESULT SetUp_Vertices(aiMesh* pAIMesh, _fmatrix PivotMatrix);
	HRESULT SetUp_AnimVertices(aiMesh* pAIMesh, class CModel* pModel);
	HRESULT SetUp_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix);

private:
	char								m_szName[MAX_PATH] = "";
	_uint								m_iMaterialIndex = 0;
	_uint								m_iNumBones = 0;
	vector<class CHierarchyNode*>		m_HierarchyNodes;


public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, aiMesh* pAIMesh, class CModel* pModel, _fmatrix PivotMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END