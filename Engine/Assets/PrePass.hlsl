cbuffer cbPrePassBuffer : register(b0)
{
	float4x4 gWorldViewProj;
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
};

cbuffer cbColorBuffer : register(b1)
{
	float4 gColor;
};

cbuffer cbSelectBuffer : register(b2)
{
	uint gID; float3 pad;
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 posW : POSITION;
	float3 normal : NORMAL;
};


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.pos = mul(gWorldViewProj, float4(input.pos, 1.0f));
	output.posW = mul(gWorld, float4(input.pos, 1.0f)).xyz;
	output.normal = mul(float4(input.normal, 1.0f), gWorldInvTranspose).xyz;

	return output;
}

struct PS_OUTPUT
{
	float4 color : SV_TARGET0;
	float4 position : SV_TARGET1;
	float4 normal : SV_TARGET2;
};

PS_OUTPUT PS(VS_OUTPUT input)
{
	PS_OUTPUT output;

	output.color = gColor;
	output.position = float4(input.posW, gID);
	output.normal = float4(input.normal, 0.0f);

	return output;
}