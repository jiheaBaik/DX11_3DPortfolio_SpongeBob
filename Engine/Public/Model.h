#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_ANIM, TYPE_NONANIM, TYPE_END };
public:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& Prototype);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshContainers() const {
		return m_iNumMeshContainers;
	}

	class CHierarchyNode* Get_HierarchyNodes(const char* pHierarchyNodeName);

	_float4x4* Get_BoneMatrixPtr(const char* pHierarchyNodeName);
	_float4x4  Get_BoneOffsetMatrix(const char* pHierarchyNodeName);
	_float4x4  Get_PivotMatrix() {
		return m_PivotMatrix;
	}

public:
	void Set_CurrentAnimIndex(_uint iIndex) {
		m_iCurrentAnimIndex = iIndex;
	}


	void Set_IsFinish(_bool _isFinish) {
		m_bIsfinish = _isFinish;
	}
	_bool Get_IsFinish() {
		return m_bIsfinish;
	}

	void Set_IsOneMotionFinish(_bool _isFinish) {
		m_bIsOneMotionfinish = _isFinish;
	}
	_bool Get_IsOneMotionFinish() {
		return m_bIsOneMotionfinish;
	}

	_bool Get_IsNext() {
		return bNotNext;
	}

	_bool Get_NextFinish() { return m_bNextISFinish; }
public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eModelType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix);
	virtual HRESULT NativeConstruct(void* pArg);

public:
	void Update_Animation(_double TimeDelta);
	void Update_SpongeBobAnimation(_double TimeDelta, _uint _Currentindex, _bool _isOnly, _uint size);
	void Update_King_NeptuneAnimation(_double TimeDelta);
	void Update_Boss_Sandy_Animation(_double TimeDelta, _uint _Currentindex, _bool _isZero, _uint size);
	void Update_Boss_Sandy_Head_Animation(_double TimeDelta, _uint _Currentindex, _bool _isZero, _uint size);

	
	vector<class CAnimation*>				m_Animations;
	
	HRESULT Bind_SRV(class CShader* pShader, const char* pConstantName, _uint iMeshContainerIndex, aiTextureType eTextureType);
	HRESULT Render(_uint iMeshContainerIndex, class CShader* pShader, _uint iPassIndex = 0);

private:
	const aiScene*			m_pScene = nullptr;
	Assimp::Importer		m_Importer;
	TYPE					m_eModelType = TYPE_END;
	_bool bNotNext = false;
public:

	// GetMeshContainers
	vector<class CMeshContainer*> GetMeshContainers() { return m_MeshContainers; }
	// GetAnimations
	vector<class CAnimation*> GetMeshAnimations() { return m_Animations; }

private:
	_uint									m_iNumMeshContainers = 0;

	_bool m_bNextISFinish = false;
	vector<class CMeshContainer*>			m_MeshContainers;
	typedef vector<class CMeshContainer*>	MESHCONTAINERS;

private:
	_uint									m_iNumMaterials = 0;
	vector<MODELMATERIALDESC>				m_Materials;
	typedef vector<MODELMATERIALDESC>		MATERIALS;

private:
	vector<class CHierarchyNode*>			m_HierarchyNodes;
	typedef vector<class CHierarchyNode*>	HIERARCHYNODES;

private:

	//vector<class CAnimation*>				m_Animations;
	typedef vector<class CAnimation*>		ANIMATIONS;

	//map<const char*, const char*>			m_AnimationInfo;

private:
	_float4x4					m_PivotMatrix;
	_uint						m_iCurrentAnimIndex = 0;
	_uint									m_iNumAnimations;

	_uint						m_iPastAnimIndex = 0;
	_bool m_bIsfinish = false;
	_bool m_bIsOneMotionfinish = false;

private:
	HRESULT Create_MeshContainers(_fmatrix PivotMatrix);
	HRESULT Create_Materials(const char* pModelFilePath);
	HRESULT Create_HierarchyNodes(aiNode* pNode, class CHierarchyNode* pParent, _uint iDepth);
	HRESULT Create_Animation();

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eModelType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END