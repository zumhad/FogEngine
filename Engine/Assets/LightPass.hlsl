#include "Light.hlsl"

cbuffer cbLightPassBuffer : register(b0)
{
    DirectionalLight gDirLight;
    PointLight gPointLight[16];
    float4x4 gShadowTransform;
    float3 gCameraPos;
    int gPointCount;
    int gWidth;
    int gHeight;
    float gTexelSizeInv; float pad;
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

float3 ToneMapping(float3 color)
{
    return color / (color + 1.0f);
}

float3 GammaCorrection(float3 color)
{
    return pow(abs(color), 1.0f / 2.2f);
}

float3 GetShadowPosOffset(float nDotL, float3 normal)
{
    float nmlOffsetScale = saturate(1.0f - nDotL);
    return gTexelSizeInv * 100.0f * nmlOffsetScale * normal;
}



float SampleShadowMap(float2 base_uv, float u, float v, float depth) 
{
    float2 uv = base_uv + float2(u, v) * gTexelSizeInv;

    return gTextureShadow.SampleCmpLevelZero(gShadowSampler, uv, depth);
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

    float bias = 0.001f;

    currentDepth += bias;

    float texelSize = 1.0f / gTexelSizeInv;
    float2 uv = projCoords.xy * texelSize;

    float2 base_uv;
    base_uv.x = floor(uv.x + 0.5f);
    base_uv.y = floor(uv.y + 0.5f);

    float s = uv.x + 0.5f - base_uv.x;
    float t = uv.y + 0.5f - base_uv.y;

    base_uv -= float2(0.5f, 0.5f);
    base_uv *= gTexelSizeInv;

    float uw0 = (3.0f - 2.0f * s);
    float uw1 = (1.0f + 2.0f * s);

    float u0 = (2.0f - s) / uw0 - 1;
    float u1 = s / uw1 + 1.0f;

    float vw0 = (3.0f - 2.0f * t);
    float vw1 = (1.0f + 2.0f * t);

    float v0 = (2.0f - t) / vw0 - 1;
    float v1 = t / vw1 + 1.0f;

    float shadow = 0.0f;
    shadow += uw0 * vw0 * SampleShadowMap(base_uv, u0, v0, currentDepth);
    shadow += uw1 * vw0 * SampleShadowMap(base_uv, u1, v0, currentDepth);
    shadow += uw0 * vw1 * SampleShadowMap(base_uv, u0, v1, currentDepth);
    shadow += uw1 * vw1 * SampleShadowMap(base_uv, u1, v1, currentDepth);

    return shadow / 16.0f;
}

PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT output;

    float4 diffuse = gTextureColor.Sample(gSampler, input.uv);
    float4 normal = gTextureNormal.Sample(gSampler, input.uv);
    float4 select = gTextureSelect.Sample(gSampler, input.uv);
    float depth = gTextureDepth.Sample(gSampler, input.uv);
    bool lighting = normal.a;

    normal.rgb = (normal.rgb - 0.5f) * 2.0f;
    normal.rgb = normalize(normal.rgb);

    output.color = float4(diffuse.rgb, 1.0f);

    if (depth > 0.0f && lighting)
    {
        output.color.rgb = diffuse.rgb * 0.001f;

        if (gDirLight.power)
        {
            float nDotL = dot(normal.xyz, -gDirLight.direction);
            select.xyz += GetShadowPosOffset(nDotL, normal.xyz);
            float4 shadowPos = mul(gShadowTransform, float4(select.xyz, 1.0f));

            float shadow = Shadow(shadowPos, normal.xyz, gDirLight.direction);
            output.color.rgb += ApplyDirectionLight(gDirLight, normal.xyz, diffuse.rgb, select.xyz, gCameraPos) * (1.0f - shadow);
        }
        
        [unroll]
        for (int j = 0; j < gPointCount; j++)
        {
            output.color.rgb += ApplyPointLight(gPointLight[j], normal.xyz, diffuse.rgb, select.xyz, gCameraPos);
        }
    }

    //output.color.rgb = ToneMapping(output.color.rgb);
    output.color.rgb = GammaCorrection(output.color.rgb);
    output.color.a = dot(output.color.rgb, float3(0.299, 0.587, 0.114));

    return output;
}