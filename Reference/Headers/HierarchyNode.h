#pragma once

#include "Base.h"

BEGIN(Engine)

class CHierarchyNode final : public CBase
{
//private:
//	CHierarchyNode();
//	virtual ~CHierarchyNode() = default;
private:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;
public:
	_float Get_Depth() const {
		return m_iDepth;
	}

	const char* Get_Name() const {
		return m_szName;
	}

	_matrix Get_CombinedTransformationMatrix() {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}

	_float4x4* Get_CombinedTransformationMatrixPtr() {
		return &m_CombinedTransformationMatrix;
	}

	_matrix Get_OffsetMatrix() {
		return XMLoadFloat4x4(&m_OffsetMatrix);
	}

	void SetUp_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

	void SetUp_OffsetMatrix(_float4x4 OffsetMatrix) {
		m_OffsetMatrix = OffsetMatrix;
	}

public:
	HRESULT NativeConstruct(const char* pName, _fmatrix TransformationMatrix, CHierarchyNode* pParent, _uint iDepth);
	void Update_CombinedTransformationMatrix();
private:
	char				m_szName[MAX_PATH] = "";

	CHierarchyNode*		m_pParent = nullptr;

	_float4x4			m_OffsetMatrix;

	/* 부모뼈기준의 상태행렬. */
	_float4x4			m_TransformationMatrix;

	/*  m_TransformationMatrix * 부모`s m_CombinedTransformationMatrix */
	_float4x4			m_CombinedTransformationMatrix;

	_float				m_iDepth = 0;

public:
	static CHierarchyNode* Create(const char* pName, _fmatrix TransformationMatrix, CHierarchyNode* pParent, _uint iDepth);
	virtual void Free() override;
};

END