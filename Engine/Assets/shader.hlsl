cbuffer ConstantBuffer : register(b0)
{
    float4x4 gWorld;
    float4 gMaterial;
    float4x4 gWorldViewProj;
}

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
};

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float3 dir : DIRECTION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    output.pos = mul(float4(input.pos, 1.0f), gWorldViewProj);
    output.dir = mul(float4(input.pos, 1.0f), gWorld);
    /*output.pos = mul(float4(input.pos, 1.0f), World);
    output.dir = output.pos;
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Projection);
    output.normal = input.normal.xyz;*/

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSL(VS_OUTPUT input) : SV_Target
{
    /*float3 dir = normalize((float3)vLightDir - input.dir);
    float4 finalColor = saturate(dot(dir, input.normal) * vLightColor);
    finalColor.a = 1;

    return finalColor * Color;*/
    return 0;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return gMaterial;
}