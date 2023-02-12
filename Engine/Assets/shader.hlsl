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
    int gPointCount;
    int2 gMousePos; float pad;
};

cbuffer cbPerObject : register(b1)
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
    Material gMaterial;
    int gID; float3 pad1;
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

struct PickingObject
{
    float3 position;
    int id;
    float depth;
};

AppendStructuredBuffer<PickingObject> gOutput  : register(u1);

[earlydepthstencil] // ??
float4 PS(VS_OUTPUT input) : SV_Target
{
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

    if (gMousePos.x == (int)input.PosH.x && gMousePos.y == (int)input.PosH.y)
    {
        PickingObject pick;
        pick.position = worldPos;
        pick.id = gID;
        pick.depth = input.PosH.z / input.PosH.w;

        gOutput.Append(pick);
    }

    return float4(color, 1.0f);
}
