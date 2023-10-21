cbuffer cbShadowPassBuffer : register(b0)
{
	float4x4 gWorldViewProj;
};

struct VS_INPUT
{
	float3 pos : POSITION;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = mul(gWorldViewProj, float4(input.pos, 1.0f));

	return output;
}

struct PS_OUTPUT
{
	float depth : SV_DEPTH;
};

PS_OUTPUT PS(VS_OUTPUT input)
{
	PS_OUTPUT output;

	output.depth = input.pos.z / input.pos.w;

	return output;
}