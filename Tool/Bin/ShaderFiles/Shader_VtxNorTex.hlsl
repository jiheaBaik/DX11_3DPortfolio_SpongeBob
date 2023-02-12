

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float3		g_vLightDir = float3(1.f, -1.f, 1.f);
float3		g_vLightPos = (float3)0.f;
float		g_fRange = 0.f;
float4		g_vLightDiffuse = float4(1.f, 0.0f, 0.0f, 1.0f);
float4		g_vLightAmbient = float4(1.f, 1.f, 1.f, 1.0f);
float4		g_vLightSpecular = float4(1.f, 1.f, 0.0f, 1.0f);

float3		g_vCamPosition;


texture2D	g_DiffuseTexture;
float4		g_vMtrlAmbient = float4(0.4f, 0.4f, 0.4f, 1.0f);
float4		g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.0f);

sampler DefaultSampler = sampler_state 
{  
	FILTER = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
};


struct VS_IN
{
	float3 vPosition : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	vector		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

	return Out;
}


/* w나누기 연산.  */
/* 뷰포트로 변환. */
/* 래스터라이즈를 수행 .*/

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
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

	vector	vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV * 20.f);	


	float	fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal.xyz)), 0.f);

	float4	vAmbientColor = g_vLightAmbient * g_vMtrlAmbient;

	float4	vReflect = vector(reflect(normalize(g_vLightDir), normalize(In.vNormal.xyz)), 0.f);
	float4	vLook = In.vWorldPos - vector(g_vCamPosition, 1.f);

	float	fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 20.f);

	float4	vSpecularColor = g_vLightSpecular * g_vMtrlSpecular;

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + vAmbientColor) 
		+ vSpecularColor * fSpecular;

	return Out;
}

PS_OUT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector	vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV * 20.f);
	
	float3	vLightDir = (In.vWorldPos - vector(g_vLightPos, 1.f)).xyz;	

	float	fDistance = length(vLightDir);

	float	fAtt = saturate((g_fRange - fDistance) / g_fRange);

	float	fShade = max(dot(normalize(vLightDir) * -1.f, normalize(In.vNormal.xyz)), 0.f) * fAtt;

	float4	vAmbientColor = g_vLightAmbient * g_vMtrlAmbient;

	float4	vReflect = vector(reflect(normalize(vLightDir), normalize(In.vNormal.xyz)), 0.f);
	float4	vLook = In.vWorldPos - vector(g_vCamPosition, 1.f);

	float	fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 20.f) * fAtt;

	float4	vSpecularColor = g_vLightSpecular * g_vMtrlSpecular;

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + vAmbientColor)
		+ vSpecularColor * fSpecular;

	return Out;
}




technique11			DefaultTechnique
{
	pass DefaultPass
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Test
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN_POINT();
	}
}
