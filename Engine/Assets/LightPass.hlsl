#include "Light.hlsl"

cbuffer cbLightPassBuffer : register(b0)
{
    DirectionalLight gDirLight;
    PointLight gPointLight[MAX_POINT_LIGHT];
    float3 gCameraPosition;
    int gWidth;
    int gHeight;
    int gPointCount; float2 pad;
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
Texture2D<float4> gTexturePositionID : register(t2);
Texture2D<float4> gTextureNormalLighting : register(t3);
Texture2D<unsigned int> gTextureRangeMaterial : register(t4);
Texture2DArray<float> gTextureShadow : register(t5);

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

float SampleShadowMap(float2 base_uv, float u, float v, float depth, int index, float2 shadowMapSizeInv)
{
    float2 uv = base_uv + float2(u, v) * shadowMapSizeInv;

    return gTextureShadow.SampleCmpLevelZero(gShadowSampler, float3(uv, (float)index), depth);
}

float SampleShadowMapOptimizedPCF(float3 shadowPos, in float3 shadowPosDX, float3 shadowPosDY, int index) 
{
    float2 shadowMapSize;
    float numSlices;
    gTextureShadow.GetDimensions(shadowMapSize.x, shadowMapSize.y, numSlices);

    float lightDepth = shadowPos.z;

    lightDepth += gDirLight.bias;

    float2 uv = shadowPos.xy * shadowMapSize;

    float2 shadowMapSizeInv = 1.0 / shadowMapSize;

    float2 base_uv;
    base_uv.x = floor(uv.x + 0.5);
    base_uv.y = floor(uv.y + 0.5);

    float s = (uv.x + 0.5 - base_uv.x);
    float t = (uv.y + 0.5 - base_uv.y);

    base_uv -= float2(0.5, 0.5);
    base_uv *= shadowMapSizeInv;

    float sum = 0;

    float uw0 = (3 - 2 * s);
    float uw1 = (1 + 2 * s);

    float u0 = (2 - s) / uw0 - 1;
    float u1 = s / uw1 + 1;

    float vw0 = (3 - 2 * t);
    float vw1 = (1 + 2 * t);

    float v0 = (2 - t) / vw0 - 1;
    float v1 = t / vw1 + 1;

    sum += uw0 * vw0 * SampleShadowMap(base_uv, u0, v0, lightDepth, index, shadowMapSizeInv);
    sum += uw1 * vw0 * SampleShadowMap(base_uv, u1, v0, lightDepth, index, shadowMapSizeInv);
    sum += uw0 * vw1 * SampleShadowMap(base_uv, u0, v1, lightDepth, index, shadowMapSizeInv);
    sum += uw1 * vw1 * SampleShadowMap(base_uv, u1, v1, lightDepth, index, shadowMapSizeInv);

    return sum * 1.0f / 16;
}

float SampleShadowCascade(float3 position, float3 shadowPosDX, float3 shadowPosDY, int index)
{
    position += gDirLight.offset[index].xyz;
    position *= gDirLight.scale[index].xyz;

    shadowPosDX *= gDirLight.scale[index].xyz;
    shadowPosDY *= gDirLight.scale[index].xyz;

    return SampleShadowMapOptimizedPCF(position, shadowPosDX, shadowPosDY, index);
}

PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT output;

    float4 diffuse = gTextureColor.Sample(gSampler, input.uv);
    float4 normalLighting = gTextureNormalLighting.Sample(gSampler, input.uv);
    float4 positionID = gTexturePositionID.Sample(gSampler, input.uv);
    unsigned int rangeMaterial = gTextureRangeMaterial.Load(int3(input.uv * int2(gWidth, gHeight), 0));
    float depth = gTextureDepth.Sample(gSampler, input.uv);

    float3 position = positionID.rgb;
    bool lighting = normalLighting.a;
    float roughness = (rangeMaterial & 0x000000ff) / 255.0f;
    float metallic = ((rangeMaterial & 0x0000ff00) >> 8) / 255.0f;
    float range = f16tof32(rangeMaterial >> 16);
    float3 normal = (normalLighting.rgb - 0.5f) * 2.0f;
    normal = normalize(normal);

    output.color = diffuse;

    if (depth && lighting)
    {
        output.color.rgb = diffuse.rgb * 0.001f;

        if (gDirLight.power)
        {
            int index = -1;

            [unroll]
            for (int i = MAX_CASCADES - 1; i >= 0; i--)
            {
                if (range < gDirLight.split[i].split) index = i;
            }

            float shadow = 0.0f;

            if (index != -1)
            {
                float3 shadowPos = mul(gDirLight.viewProj, float4(position, 1.0f)).xyz;
                float3 shadowPosDX = ddx_fine(shadowPos);
                float3 shadowPosDY = ddy_fine(shadowPos);
                shadow = SampleShadowCascade(shadowPos, shadowPosDX, shadowPosDY, index);

                const float BlendThreshold = 0.1f;

                float nextSplit = gDirLight.split[index].split;
                float splitSize = index == 0 ? nextSplit : nextSplit - gDirLight.split[index-1].split;
                float fadeFactor = (nextSplit - range) / splitSize;

                [branch]
                if (fadeFactor <= BlendThreshold && index != MAX_CASCADES - 1)
                {
                    float3 nextPosition = mul(gDirLight.viewProj, float4(position, 1.0f)).xyz;

                    float3 nextSplitVisibility = SampleShadowCascade(nextPosition, shadowPosDX, shadowPosDY, index + 1);
                    float lerpAmt = smoothstep(0.0f, BlendThreshold, fadeFactor);
                    shadow = lerp(nextSplitVisibility, shadow, lerpAmt).x;
                }
            }

            output.color.rgb += ApplyDirectionLight(gDirLight, normal, diffuse.rgb, position, gCameraPosition, metallic, roughness) * (1.0f - shadow);
        }
        
        [unroll]
        for (int j = 0; j < gPointCount; j++)
        {
            if (gPointLight[j].power)
            {
                output.color.rgb += ApplyPointLight(gPointLight[j], normal, diffuse.rgb, position, gCameraPosition, metallic, roughness);
            }
        }
    }

    //output.color.rgb = ToneMapping(output.color.rgb);
    output.color.rgb = GammaCorrection(output.color.rgb);
    output.color.a = dot(output.color.rgb, float3(0.299, 0.587, 0.114));

    return output;
}