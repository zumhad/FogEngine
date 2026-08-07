cbuffer Buffer0 : register(b0)
{
	float4x4 gViewProj;
};

cbuffer Buffer1 : register(b1)
{
	float4x4 gWorld;
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

	output.pos = mul(gWorld, float4(input.pos, 1.0f));
	output.pos = mul(gViewProj, output.pos);

	return output;
}