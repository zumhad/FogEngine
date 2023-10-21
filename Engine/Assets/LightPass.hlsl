#include "Light.hlsl"

cbuffer cbLightPassBuffer : register(b0)
{
    DirectionalLight gDirLight[16];
    PointLight gPointLight[16];
    float3 gCameraPos;
    int gDirCount;
    int gPointCount;
    int gWidth;
    int gHeight;
    float gTexelSize;
    float4x4 gShadowTransform;
};

struct VS_INPUT
{
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    float x = input.uv.x * 2.0f - 1.0f;
    float y = 1.0f - input.uv.y * 2.0f;

    output.pos = float4(x, y, 1.0f, 1.0f);
    output.uv = input.uv.xy;

    return output;
}

Texture2D<float4> gTextureColor : register(t0);
Texture2D<float> gTextureDepth : register(t1);
Texture2D<float4> gTextureSelect : register(t2);
Texture2D<float4> gTextureNormal : register(t3);
Texture2D<float> gTextureShadow : register(t4);

SamplerState gSampler : register(s0);
SamplerComparisonState gShadowSampler : register(s1);

struct PS_OUTPUT
{
    float4 color : SV_TARGET0;
};

float3 Uncharted2ToneMapping(float3 color)
{
    color *= 16.0f;
    color = color / (1 + color);
    color = pow(abs(color), 1.0f / 2.2f);
    return color;
}

float3 GammaCorrection(float3 color)
{
    return pow(abs(color), 1.0f / 2.2f);
}

float Shadow(float4 fragPosLightSpace, float3 normal, float3 dir)
{
    float3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    float currentDepth = projCoords.z;

    if (currentDepth <= 0.0f || currentDepth >= 1.0f)
        return 0.0f;

    projCoords = projCoords * 0.5f + 0.5f;
    projCoords.y = 1.0f - projCoords.y;

    if (projCoords.x > 1.0f || projCoords.x < 0.0f)
        return 0.0f;

    if (projCoords.y > 1.0f || projCoords.y < 0.0f)
        return 0.0f;

    float closestDepth = gTextureShadow.Sample(gSampler, projCoords.xy);
    float bias = max(0.003f * saturate(1.0f - dot(normal, -dir)), 0.003f);
    float shadow = 0.0f;

    [unroll]
    for (int x = -1; x <= 1; ++x)
    {
        [unroll]
        for (int y = -1; y <= 1; ++y)
        {
            float depth = gTextureShadow.SampleCmpLevelZero(gShadowSampler, projCoords.xy + float2(x, y) * gTexelSize, currentDepth + bias);
            shadow += depth;
        }
    }
    shadow /= 9.0f;

    return shadow;
}

PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT output;

    float4 diffuse = gTextureColor.Sample(gSampler, input.uv);
    float4 normal = gTextureNormal.Sample(gSampler, input.uv);
    float4 select = gTextureSelect.Sample(gSampler, input.uv);
    float depth = gTextureDepth.Sample(gSampler, input.uv);
    normal = normalize(normal);

    output.color = float4(diffuse.rgb, 1.0f);

    float shadow = Shadow(mul(gShadowTransform, float4(select.xyz, 1.0f)), normal.xyz, float3(0, -1, -1));

    if (depth > 0.0f)
    {
        output.color.rgb = diffuse.rgb * 0.001f;

        [unroll]
        for (int i = 0; i < gDirCount; i++)
        {
            output.color.rgb += ApplyDirectionLight(gDirLight[i], normal.xyz, diffuse.rgb, select.xyz, gCameraPos) * (1.0f - shadow);
        }
        
        [unroll]
        for (int j = 0; j < gPointCount; j++)
        {
            output.color.rgb += ApplyPointLight(gPointLight[j], normal.xyz, diffuse.rgb, select.xyz, gCameraPos);
        }
    }

    output.color.rgb = GammaCorrection(output.color.rgb);
    output.color.a = dot(output.color.rgb, float3(0.299, 0.587, 0.114));

    return output;
}