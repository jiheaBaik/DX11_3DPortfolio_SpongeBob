#include "Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector		g_vColor = vector(0.f, 1.f, 0.f, 1.f);

struct VS_IN
{
	float3 vPosition : POSITION;
	float4 vColor : COLOR0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float4 vColor : COLOR0;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	vector		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

	/*mul(벡터, 행렬)이라면, 벡터를 행(Row) 기준 벡터로 읽을게.
	(벡터를 1x4로 본다는 뜻이므로, 행렬을 열(Column) 기준 행렬로 계산한다는 말.)
	mul(행렬, 벡터)라면, 벡터를 열(Column) 기준 벡터로 읽을게.
	(벡터를 4x1로 본다는 뜻이므로, 행렬을 행(Row) 기준 행렬로 계산한다는 말.)
	행렬 곱셈은 AxB CxD 일때, B와 C가 같아야 곱셈이 되니까.
	(1x4 4x4는 됨. 4x1 4x4는 안되므로, 위치를 바꾸어서 4x4 4x1로 한 것이다.)*/
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vColor = In.vColor;

	return Out;
}


/* w나누기 연산.  */
/* 뷰포트로 변환. */
/* 래스터라이즈를 수행 .*/

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float4 vColor: COLOR0;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = In.vColor * g_vColor;

	return Out;
}



technique11			DefaultTechnique
{
	pass DefaultPass
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}
