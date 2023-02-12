#pragma once

#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Cell final : public CVIBuffer
{
private:
	CVIBuffer_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Cell(const CVIBuffer_Cell& Prototype);
	virtual ~CVIBuffer_Cell() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _float3* pPoints);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render() override;

public:
	static CVIBuffer_Cell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END