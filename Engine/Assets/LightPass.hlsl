#include "Light.hlsl"
#include "Shadow.hlsl"

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

    float4 diffuse = gTextureColor.Sample(gSampler, input.uv);
    //float4 normalLighting = gTextureNormalLighting.Sample(gSampler, input.uv);
    float4 normalLighting = gTextureNormalLighting.Load(int3(input.uv * int2(1280.0f, 720.0f), 0));
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
            float shadow = Shadow(range, position);
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

    output.color.rgb = ToneMapping(output.color.rgb);
    output.color.rgb = GammaCorrection(output.color.rgb);
    output.color.a = dot(output.color.rgb, float3(0.299, 0.587, 0.114));

    return output;
}