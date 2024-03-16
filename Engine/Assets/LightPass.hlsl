#include "Light.hlsl"
#include "Shadow.hlsl"
#include "Pack.hlsl"

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
Texture2D<uint4> gTexturePositionMaterial : register(t2);
Texture2D<float4> gTextureNormalLighting : register(t3);

SamplerState gSampler : register(s0);

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

PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT output;

    float4 color = gTextureColor.Sample(gSampler, input.uv);
    float depth = gTextureDepth.Sample(gSampler, input.uv);
    uint4 positionMaterial = gTexturePositionMaterial.Load(int3(input.uv.x * gWidth, input.uv.y * gHeight, 0));
    float4 normalLighting = gTextureNormalLighting.Sample(gSampler, input.uv);

    float3 position = f16tof32(positionMaterial.rgb);
    bool lighting = normalLighting.a;
    float metallic = (positionMaterial.a >> 8) / 255.0f;
    float roughness = (positionMaterial.a & 0xff) / 255.0f;
    float3 normal = (normalLighting.rgb - 0.5f) * 2.0f;
    normal = normalize(normal);

    output.color.rgb = color.rgb;

    if (depth && lighting)
    {
        output.color.rgb *= 0.001f; // ambient

        if (gDirLight.power)
        {
            float shadow = Shadow(position, normal);
            output.color.rgb += ApplyDirectionLight(gDirLight, normal, color.rgb, position, gCameraPosition, metallic, roughness) * (1.0f - shadow);
        }
        
        [unroll]
        for (int j = 0; j < gPointCount; j++)
        {
            if (gPointLight[j].power)
            {
                output.color.rgb += ApplyPointLight(gPointLight[j], normal, color.rgb, position, gCameraPosition, metallic, roughness);
            }
        }
    }

    if (lighting) output.color.rgb = ToneMapping(output.color.rgb);

    output.color.rgb = GammaCorrection(output.color.rgb);
    output.color.a = dot(output.color.rgb, float3(0.299, 0.587, 0.114));

    return output;
}