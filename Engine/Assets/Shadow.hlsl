cbuffer cbLightPassBuffer : register(b0)
{
    DirectionalLight gDirLight;
    PointLight gPointLight[MAX_POINT_LIGHT];
    float3 gCameraPosition;
    int gWidth;
    int gHeight;
    int gPointCount; float2 pad;
};

Texture2DArray<float> gTextureShadow : register(t5);
SamplerComparisonState gShadowSampler : register(s1);

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

    //return gTextureShadow.SampleCmpLevelZero(gShadowSampler, float3(shadowPos.xy, (float)index), lightDepth);

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

float Shadow(float range, float3 position)
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

        float nextSplit = gDirLight.split[index].split;
        float splitSize = index == 0 ? nextSplit : nextSplit - gDirLight.split[index - 1].split;
        float fadeFactor = (nextSplit - range) / splitSize;

        [branch]
        if (fadeFactor <= gDirLight.blend && index != MAX_CASCADES - 1)
        {
            float3 nextPosition = mul(gDirLight.viewProj, float4(position, 1.0f)).xyz;

            float3 nextSplitVisibility = SampleShadowCascade(nextPosition, shadowPosDX, shadowPosDY, index + 1);
            float lerpAmt = smoothstep(0.0f, gDirLight.blend, fadeFactor);
            shadow = lerp(nextSplitVisibility, shadow, lerpAmt).x;
        }
    }

    return shadow;
}