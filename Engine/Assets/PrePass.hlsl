cbuffer cbPrePassBuffer : register(b0)
{
	float4x4 gViewProj;
};

cbuffer cbPrePassBuffer1 : register(b1)
{
	float4x4 gWorld;
	float3x3 gWorldInvTranspose; float3 pad;
};

cbuffer cbColorBuffer : register(b2)
{
	float4 gColor;
};

cbuffer cbSelectBuffer : register(b3)
{
	uint gID; float3 pad1;
};

cbuffer cbNormalBuffer : register(b4)
{
	bool gLighting; float3 pad2;
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 posW : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	output.posW = mul(gWorld, float4(input.pos, 1.0f)).xyz;
	output.pos = mul(gViewProj, float4(output.posW, 1.0f));
	output.normal = mul(gWorldInvTranspose, input.normal);

	output.uv = input.uv;

	return output;
}

struct PS_OUTPUT
{
	float4 color : SV_TARGET0;
	float4 position : SV_TARGET1;
	float4 normal : SV_TARGET2;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PS_OUTPUT PS(VS_OUTPUT input)
{
	PS_OUTPUT output;

	output.color = gTexture.Sample(gSampler, input.uv);
	output.color.rgb = pow(abs(output.color.rgb), 2.2f) * gColor.rgb;
	output.position = float4(input.posW, gID);
	output.normal = float4(input.normal.rgb * 0.5f + 0.5, gLighting);

	return output;
}