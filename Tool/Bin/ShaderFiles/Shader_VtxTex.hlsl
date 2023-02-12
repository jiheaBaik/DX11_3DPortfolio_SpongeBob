
//float2
//float3
//float4 vector
//
//float1x3
//float3x3
//float4x4	matrix

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D	g_DiffuseTexture;

sampler DefaultSampler = sampler_state 
{  
	FILTER = MIN_MAG_MIP_LINEAR;
	/*AddressU = clamp;
	AddressV = clamp;*/
};


struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};


VS_OUT s(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	vector		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}


/* w나누기 연산.  */
/* 뷰포트로 변환. */
/* 래스터라이즈를 수행 .*/

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;	
};


/* 픽셀의 색을 결정한다. */
// float4 PS_MAIN(PS_IN In) : SV_TARGET0
PS_OUT v(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0; 

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.gb = Out.vColor.r;

	// Out.vColor = vector(1.f, 1.f, 0.0f, 1.0f);

	return Out;
}



technique11			DefaultTechnique
{
	pass DefaultPass
	{
		VertexShader = compile vs_5_0 s();
		PixelShader = compile ps_5_0 v();
	}
}
