cbuffer ConstantBuffer : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gWorld;
}

cbuffer ObjectBuffer : register(b1)
{
    float4 gColor;
};


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

    float4 position = float4(input.pos, 1.0f);
    float4 world = mul(gWorld, position);

    output.pos = mul(gWorldViewProj, world);

    //output.pos = mul(float4(input.pos, 1.0f), gWorldViewProj);
    return output;
}


float4 PS(VS_OUTPUT input) : SV_Target
{
    return gColor;
}