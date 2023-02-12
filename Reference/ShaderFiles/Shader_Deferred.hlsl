
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

matrix		g_ProjMatrixInv;
matrix		g_ViewMatrixInv;

float3		g_vCamPosition;

float3		g_vLightDir;
float3		g_vLightPos;
float		g_fRange;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f ,1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

texture2D	g_Texture;
texture2D	g_NormalTexture;
texture2D	g_DepthTexture;
texture2D	g_DiffuseTexture;
texture2D	g_ShadeTexture;
texture2D	g_SpecularTexture;

sampler DefaultSampler = sampler_state 
{
	filter = min_mag_mip_linear;
	//minfilter = linear;
	//magfilter = linear;
	//mipfilter = linear;
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


/* w������ ����.  */
/* ����Ʈ�� ��ȯ. */
/* �����Ͷ���� ���� .*/

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;	
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0; 

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	return Out;
}

struct PS_OUT_LIGHT
{
	float4 vShade : SV_TARGET0;
	float4 vSpecular : SV_TARGET1;
};


PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);

	float		fViewZ = vDepthDesc.x * 500.0f;

	Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(vector(g_vLightDir, 0.f)) * -1.f, vNormal)) + g_vLightAmbient * g_vMtrlAmbient);
	Out.vShade.a = 1.f;	

	vector		vWorldPos;

	/* 2���� ���������̽� ���� ��ġ�� ���Ѵ�. */
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.y;
	vWorldPos.w = 1.f;	

	/* �佺���̽� ��������ġ�� ���Ѥ�. ������ z���� �ٽ� ���ϰ� ��������� ������� ���ϳ�. */
	vWorldPos = vWorldPos * fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* ���彺���̽� ��������ġ�� ���Ѥ�. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector		vReflect = normalize(reflect(normalize(vector(g_vLightDir, 0.f)), normalize(vNormal)));
	vector		vLook = vWorldPos - vector(g_vCamPosition, 1.f);

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vLook * -1.f), vReflect)), 30.f);
	Out.vSpecular.a = 0;
	return Out;	
}


PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);

	float		fViewZ = vDepthDesc.x * 500.0f;

	vector		vWorldPos;

	/* 2���� ���������̽� ���� ��ġ�� ���Ѵ�. */
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.y;
	vWorldPos.w = 1.f;

	/* �佺���̽� ��������ġ�� ���Ѥ�. ������ z���� �ٽ� ���ϰ� ��������� ������� ���ϳ�. */
	vWorldPos = vWorldPos * fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* ���彺���̽� ��������ġ�� ���Ѥ�. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	 
	vector		vLightDir = vWorldPos - vector(g_vLightPos, 1.f);
	float		fDistance = length(vLightDir);

	
	float		fAtt = saturate((g_fRange - fDistance) / g_fRange);


	Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(vLightDir) * -1.f, vNormal)) + g_vLightAmbient * g_vMtrlAmbient) * fAtt;
	Out.vShade.a = 1.f;	

	vector		vReflect = normalize(reflect(normalize(vLightDir), normalize(vNormal)));
	vector		vLook = vWorldPos - vector(g_vCamPosition, 1.f);

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vLook * -1.f), vReflect)), 30.f) * fAtt;

	return Out;
}


PS_OUT PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	/* ��ǻ�� Ÿ�ٿ� �׷����ִ� ���� ���̵� Ÿ�ٿ� �׷����ִ� ������ ȥ���Ͽ� ����ۿ� �׸���. */
	vector		vDiffuseDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vShadeDesc = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vSpecularDesc = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	

	Out.vColor = vDiffuseDesc * vShadeDesc + vSpecularDesc;

	if (Out.vColor.a == 0.f)
		discard;

	return Out;
}

RasterizerState RS_Default
{
	FillMode = Solid;
	CullMode = Back;
	FrontCounterClockwise = false;
};

DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_Debug
{
	DepthEnable = false;
	DepthWriteMask = zero;
	
	/*DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;*/
};

BlendState	BS_Default
{
	BlendEnable[0] = false;
	//SrcBlend = src_alpha;
	//DestBlend = inv_src_alpha;
};

BlendState	BS_Add
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;
	SrcBlend = one;
	DestBlend = one;
	Blendop = add;
};



technique11			DefaultTechnique
{
	pass Debug
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Debug, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}

	pass Light_Directional
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Debug, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	pass Light_Point
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Debug, 0);
		SetBlendState(BS_Add, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_POINT();
	}

	pass Blend
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Debug, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}

}


