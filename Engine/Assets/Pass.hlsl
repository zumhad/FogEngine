#include "Light.hlsl"

cbuffer cbPassBuffer : register(b0)
{
    int gWidth;
    int gHeight; float2 pad;
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
SamplerState gSampler : register(s0);

struct PS_OUTPUT
{
    float4 color : SV_TARGET0;
};

cbuffer cbLightBuffer : register(b1)
{
    DirectionalLight gDirLight[16];
    PointLight gPointLight[16];
    float3 gCameraPos;
    int gDirCount;
    int gPointCount; float3 pad1;
};

PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT output;

    float4 diffuse = gTextureColor.Sample(gSampler, input.uv.xy);
    float4 normal = gTextureNormal.Sample(gSampler, input.uv.xy);
    float4 select = gTextureSelect.Sample(gSampler, input.uv.xy);
    float depth = gTextureDepth.Sample(gSampler, input.uv.xy);
    normal = normalize(normal);

    output.color = float4(diffuse.rgb, 1.0f);

    if (depth > 0.0f)
    {
        output.color.rgb = float3(0.0f, 0.0, 0.0f);

        for (int i = 0; i < gDirCount; i++)
        {
            output.color.rgb += ApplyDirectionLight(gDirLight[i], normal.xyz, diffuse.rgb);
        }
        
        for (int j = 0; j < gPointCount; j++)
        {
            output.color.rgb += ApplyPointLight(gPointLight[j], normal.xyz, diffuse.rgb, select.xyz, gCameraPos);
        }
    }

    output.color.rgb = pow(abs(output.color.rgb), 1.0f / 2.2f);
    output.color.a = dot(output.color.rgb, float3(0.299, 0.587, 0.114));

    return output;
}