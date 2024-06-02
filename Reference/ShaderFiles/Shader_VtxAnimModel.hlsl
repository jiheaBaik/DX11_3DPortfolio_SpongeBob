#include "Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

struct tagBoneMatrices
{
    matrix BoneMatrices[256];
};

tagBoneMatrices g_Bones;

texture2D g_DiffuseTexture;
texture2D g_DiffuseTexture2;

float g_Dvalue;


struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float3 vTangent : TANGENT;
    uint4 vBlendIndex : BLENDINDEX;
    float4 vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    float fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

    matrix BoneMatrix = g_Bones.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_Bones.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_Bones.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_Bones.BoneMatrices[In.vBlendIndex.w] * fWeightW;

    vector vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);

    vPosition = mul(vPosition, g_WorldMatrix);
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);

    vector vNormal = mul(float4(In.vNormal, 0.f), BoneMatrix);

    Out.vPosition = vPosition;
    Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
    Out.vTexUV = In.vTexUV;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;


    return Out;
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};


PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 0.0f, 0.0f);

    if (Out.vDiffuse.a < 0.1f)
        discard;

    return Out;
}


PS_OUT PS_MAINTest(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    PS_OUT Out2 = (PS_OUT) 0;
    PS_OUT Temp = (PS_OUT) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 0.0f, 0.0f);

    if (Out.vDiffuse.a < 0.1f)
        discard;

    Out2.vDiffuse = g_DiffuseTexture2.Sample(DefaultSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 0.0f, 0.0f);


    Temp.vDiffuse = Out.vDiffuse * Out2.vDiffuse;


    if (Temp.vDiffuse.x < g_Dvalue)
        discard;
    return Out;
}


PS_OUT PS_Alpha(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 0.0f, 0.0f);

    if (Out.vDiffuse.a < 0.1f)
        discard;

    Out.vDiffuse.a = 0.3f;
    return Out;
}

PS_OUT PS_Alpha2(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 0.0f, 0.0f);

    Out.vDiffuse.a = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV).a;
    return Out;
}

PS_OUT PS_Red(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 0.0f, 0.0f);

    if (Out.vDiffuse.a < 0.1f)
        discard;

    Out.vDiffuse.x = 1;
    Out.vDiffuse.y = 0;
    Out.vDiffuse.z = 0;

    return Out;
}

PS_OUT PS_White(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 500.0f, In.vProjPos.z / In.vProjPos.w, 0.0f, 0.0f);

    if (Out.vDiffuse.a < 0.1f)
        discard;

    Out.vDiffuse.x = 1;
    Out.vDiffuse.y = 1;
    Out.vDiffuse.z = 1;

    return Out;
}


technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(RS_None);
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
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Alpha();
    }

    pass PS_Red
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Red();
    }

    pass PS_White
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_White();
    }

    pass DefaultPass5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Alpha2();
    }

    pass DefaultPassTest
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAINTest();
    }
}
