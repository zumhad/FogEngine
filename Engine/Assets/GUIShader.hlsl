struct VS_INPUT
{
    float3 pos : POSITION;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
};

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
};


VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    output.pos = mul(gWorldViewProj, float4(input.pos, 1.0f));

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}
