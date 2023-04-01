#include "light.hlsli"


//Texture2D Texture : register(t0);
//SamplerState Sampler : register(s0);


struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // w = SpecPower
    float4 Reflect;
};


struct DirectionalLight
{
    float4 Color;
    float3 Direction; float pad;
};

struct PointLight
{
    float4 Color;
    float3 Position;
    float Range;
    float Power; float3 pad;
};

cbuffer cbPerFrame : register(b0)
{
    PointLight gPointLight[16];
    DirectionalLight gDirLight[16];
    float3 gCameraPosW;
    int gDirCount;
    int gPointCount; float3 pad;

};

cbuffer cbPerObject : register(b1)
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
    Material gMaterial;
    bool gDrawOutline; float3 pad1;
};


struct VS_INPUT
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
};


struct VS_OUTPUT
{
    float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    output.PosW = mul(float4(input.PosL, 1.0f), gWorld).xyz;
    output.NormalW = mul(float4(input.NormalL, 1.0f), gWorldInvTranspose).xyz;
    output.PosH = mul(gWorldViewProj, float4(input.PosL, 1.0f));

    return output;
}


void FSchlick(inout float3 specular, inout float3 diffuse, float3 lightDir, float3 halfVec)
{
    float fresnel = pow(1.0 - saturate(dot(lightDir, halfVec)), 5.0);
    specular = lerp(specular, 1, fresnel);
    diffuse = lerp(diffuse, 0, fresnel);
}

float3 ApplyLight(Material material, float3 normal, float3 viewDir, float3 lightDir, float3 lightColor)
{
    float3 halfVec = normalize(lightDir - viewDir);
    float nDotH = saturate(dot(halfVec, normal));

    FSchlick(material.Diffuse.xyz, material.Specular.xyz, lightDir, halfVec);

    float specularMask = 1.0f;
    float specularFactor = specularMask * saturate(pow(nDotH, material.Specular.w)) * (material.Specular.w - 2.0f) / 8.0f;

    float nDotL = saturate(dot(normal, lightDir));

    return nDotL * lightColor * (material.Diffuse.xyz + specularFactor * material.Specular.xyz);
}

float3 ApplyDirectionLight(Material material, DirectionalLight light, float3 normal, float3 viewDir)
{
    return ApplyLight(material, normal, viewDir, -light.Direction, light.Color.xyz);
}

float3 ApplyPointLight(Material material, PointLight light, float3 normal, float3 viewDir, float3 worldPos)
{
    float lightRangeSq = light.Range * light.Range;
    float3 lightDir = light.Position - worldPos;
    float lightDist = length(lightDir);
    lightDir /= lightDist;

    float distToLightNorm = 1.0 - saturate(lightDist / light.Range);
    float distanceFalloff = distToLightNorm * distToLightNorm * light.Power;

    return distanceFalloff * ApplyLight(material, normal, viewDir, lightDir, light.Color.xyz);

}


Texture2D<float> Texture : register(t0);
SamplerState Sampler : register(s0);

struct PS_OUTPUT
{
    float4 color : SV_Target0;
    //float depth : SV_Depth;
};

[earlydepthstencil] // ??
PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT output;
    //output.depth = input.PosH.z / input.PosH.w;

    float3 normal = normalize(input.NormalW);
    float3 viewDir = normalize(input.PosW - gCameraPosW);
    float3 worldPos = input.PosW;

    float3 color = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < gDirCount; i++)
    {
        color += ApplyDirectionLight(gMaterial, gDirLight[i], normal, viewDir);
    }

    for (int j = 0; j < gPointCount; j++)
    {
        color += ApplyPointLight(gMaterial, gPointLight[j], normal, viewDir, worldPos);
    }

    //if (gMousePos.x == (int)input.PosH.x && gMousePos.y == (int)input.PosH.y)
    //{
    //    PickingObject pick;
    //    pick.position = input.PosW;
    //    pick.id = gID;
    //    pick.depth = input.PosH.z / input.PosH.w;
    //
    //   gOutput.Append(pick);
    //}

    if (gDrawOutline)
    {
        float w = 5.0f;

        float left = input.PosH.x - w;
        float right = input.PosH.x + w;
        float top = (input.PosH.y - 60) - w;
        float bottom = (input.PosH.y - 60) + w;

        float2 uv = float2(input.PosH.x / 800.0f, (input.PosH.y - 60) / 600.0f);
        float2 uv0 = float2(left / 800.0f, top / 600.0f);
        float2 uv1 = float2(right / 800.0f, bottom / 600.0f);
        float2 uv2 = float2(right / 800.0f, top / 600.0f);
        float2 uv3 = float2(left / 800.0f, bottom / 600.0f);

        float2 uv4 = float2(left / 800.0f, (input.PosH.y - 60) / 600.0f);
        float2 uv5 = float2((input.PosH.x) / 800.0f, top / 600.0f);
        float2 uv6 = float2(right / 800.0f, (input.PosH.y - 60) / 600.0f);
        float2 uv7 = float2((input.PosH.x) / 800.0f, bottom / 600.0f);

        float d = Texture.Sample(Sampler, uv);
        float d0 = Texture.Sample(Sampler, uv0);
        float d1 = Texture.Sample(Sampler, uv1);
        float d2 = Texture.Sample(Sampler, uv2);
        float d3 = Texture.Sample(Sampler, uv3);

        float d4 = Texture.Sample(Sampler, uv4);
        float d5 = Texture.Sample(Sampler, uv5);
        float d6 = Texture.Sample(Sampler, uv6);
        float d7 = Texture.Sample(Sampler, uv7);

        //color = float3(d, d, d);

        if (d0 == 1.0f || d1 == 1.0f || d2 == 1.0f || d3 == 1.0f)
            color = float3(1, 1, 0);
    }

    output.color = float4(color, 1);
    return output;
}
