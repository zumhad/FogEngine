cbuffer ConstantBuffer : register(b0)
{
    float4x4 gWorldViewProj;
}

struct VS_INPUT
{
    float3 pos : POSITION;
};


struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
};

VS_OUTPUT VS(VS_INPUT input) : SV_POSITION
{
    VS_OUTPUT output;

    output.pos = mul(float4(input.pos, 1.0f), gWorldViewProj);
    return output;
}


float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(1,1,0,0);
}