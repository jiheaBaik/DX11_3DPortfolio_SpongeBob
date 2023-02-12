
#include "Shader_Defines.hpp"
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D	g_DiffuseTexture;
float		g_vCamPosition;
vector g_Color;
matrix		g_SocketMatrix;
float g_Dvalue;
//sampler DefaultSampler = sampler_state 
//{  
//	FILTER = MIN_MAG_MIP_LINEAR;
//	/*AddressU = clamp;
//	AddressV = clamp;*/
//};


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


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	vector		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}

VS_OUT VS_MAIN_SOCKET(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	//float3			vLook = g_vCamPosition - In.vPosition.xyz;
	//float3			vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook));// *In.vPSize.x * 0.5f;
	//float3			vUp = normalize(cross(vLook, vRight));// *In[0].vPSize.y * 0.5f;

	
	matrix			matWS, matWSV, matWSVP;

	matWS = mul(g_WorldMatrix, g_SocketMatrix);
	matWSV = mul(matWS, g_ViewMatrix);
	matWSVP = mul(matWSV, g_ProjMatrix);

	vector		vPosition = mul(float4(In.vPosition, 1.f), matWSVP);

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
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0; 

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	/*if (Out.vColor.x == 0.f && Out.vColor.y == 0.f && Out.vColor.z == 0.f)
		discard;*/

	if (Out.vColor.a < 0.1f)
		discard;

	return Out;
}

PS_OUT PS_Blue(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	//Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	//Out.vColor.gb = Out.vColor.r;

	Out.vColor = vector(0.f, 0.f, 1.f, 1.0f);

	return Out;
}

PS_OUT PS_Color(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	//Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	//Out.vColor.gb = Out.vColor.r;

	Out.vColor = g_Color;

	return Out;
}

PS_OUT PS_ALPHA(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	/*if (Out.vColor.x == 0.f && Out.vColor.y == 0.f && Out.vColor.z == 0.f)
	discard;*/
	/*if (Out.vColor.a < 0.1f)
		discard;*/

	if (Out.vColor.a < 0.1f)
		discard;
	Out.vColor.a = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV).a;


	/*if (Out.vColor.x ==0 )
		discard;
	if (Out.vColor.y == 0)
		discard;
	if (Out.vColor.z == 0)
		discard;*/
	return Out;
}
PS_OUT PS_ALPHA80(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	/*if (Out.vColor.x == 0.f && Out.vColor.y == 0.f && Out.vColor.z == 0.f)
	discard;*/
	if (Out.vColor.a < 0.1f)
		discard;
	Out.vColor.a = 0.8;


	/*if (Out.vColor.x ==0 )
	discard;
	if (Out.vColor.y == 0)
	discard;
	if (Out.vColor.z == 0)
	discard;*/
	return Out;
}


PS_OUT PS_ALPHA60(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	/*if (Out.vColor.x == 0.f && Out.vColor.y == 0.f && Out.vColor.z == 0.f)
	discard;*/
	if (Out.vColor.a < 0.1f)
		discard;
	Out.vColor.a =0.6;


	/*if (Out.vColor.x ==0 )
	discard;
	if (Out.vColor.y == 0)
	discard;
	if (Out.vColor.z == 0)
	discard;*/
	return Out;
}

PS_OUT PS_pade(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	/*if (Out.vColor.x == 0.f && Out.vColor.y == 0.f && Out.vColor.z == 0.f)
	discard;*/
	if (Out.vColor.a < 0.1f)
		discard;
	Out.vColor.a = g_Dvalue;


	/*if (Out.vColor.x ==0 )
	discard;
	if (Out.vColor.y == 0)
	discard;
	if (Out.vColor.z == 0)
	discard;*/
	return Out;
}


PS_OUT PS_ALPHA40(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	/*if (Out.vColor.x == 0.f && Out.vColor.y == 0.f && Out.vColor.z == 0.f)
	discard;*/
	if (Out.vColor.a < 0.1f)
		discard;
	Out.vColor.a = 0.4;

	/*if (Out.vColor.x ==0 )
	discard;
	if (Out.vColor.y == 0)
	discard;
	if (Out.vColor.z == 0)
	discard;*/
	return Out;
}
PS_OUT a(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	//Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	//Out.vColor.gb = Out.vColor.r;

	Out.vColor = vector(1.f, 1.f, 0.f, 1.0f);

	return Out;
}

PS_OUT b(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	//Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	//Out.vColor.gb = Out.vColor.r;

	Out.vColor = vector(1.f, 1.f, 1.f, 1.0f);

	return Out;
}




PS_OUT Red(PS_IN In)
{
	//PS_OUT			Out = (PS_OUT)0;

	//Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.a < 0.1f)
		discard;
	else
		Out.vColor = vector(1.f, 0.f, 0.f, 1.0f);
	///*if(Out.vColor.a > 0.1f)
	//	Out.vColor = vector(1.f, 0.f, 0.f, 1.0f);*/
	

	//Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	//Out.vColor.gb = Out.vColor.r;

	//Out.vColor = vector(1.f, 0.f, 0.f, 1.0f);

	return Out;
}

PS_OUT Sky(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.a < 0.1f)
		discard;
	else
		Out.vColor = vector(0.f, 1.f, 0.917f, 1.f);
	/*if(Out.vColor.a > 0.1f)
	Out.vColor = vector(1.f, 0.f, 0.f, 1.0f);*/


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

	pass DefaultPass2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 a();
	}

	pass DefaultPass3
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 b();
	}

	pass DefaultPass4
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 Red();
	}

	pass Wire
	{
		SetRasterizerState(RS_WireFrame);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 b();
	}
	
	pass Soket
	{
		SetRasterizerState(RS_None);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_SOCKET();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Alpha
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHA();
	}

	pass Sky
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 Sky();
	}


	pass Alpha80
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHA80();
	}

	pass Alpha60
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHA60();
	}

	pass Alpha40
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHA40();
	}
	pass Blue
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Blue();
	}

	pass Color
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Color();
	}

	pass pade
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_pade();
	}
}
