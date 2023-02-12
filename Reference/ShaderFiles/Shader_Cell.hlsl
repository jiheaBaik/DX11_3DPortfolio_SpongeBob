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

	/*mul(����, ���)�̶��, ���͸� ��(Row) ���� ���ͷ� ������.
	(���͸� 1x4�� ���ٴ� ���̹Ƿ�, ����� ��(Column) ���� ��ķ� ����Ѵٴ� ��.)
	mul(���, ����)���, ���͸� ��(Column) ���� ���ͷ� ������.
	(���͸� 4x1�� ���ٴ� ���̹Ƿ�, ����� ��(Row) ���� ��ķ� ����Ѵٴ� ��.)
	��� ������ AxB CxD �϶�, B�� C�� ���ƾ� ������ �Ǵϱ�.
	(1x4 4x4�� ��. 4x1 4x4�� �ȵǹǷ�, ��ġ�� �ٲپ 4x4 4x1�� �� ���̴�.)*/
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vColor = In.vColor;

	return Out;
}


/* w������ ����.  */
/* ����Ʈ�� ��ȯ. */
/* �����Ͷ���� ���� .*/

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
