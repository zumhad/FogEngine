#include "Pack.hlsl"

cbuffer Buffer0 : register(b0)
{
	float4x4 gViewProj;
};

cbuffer Buffer1 : register(b1)
{
	float4x4 gWorld;
};

cbuffer Buffer2 : register(b2)
{
	float4 gColor;
	unsigned int gID;
	bool gLighting;
	float gMetallic;
	float gRoughness;
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

	output.normal = mul((float3x3)gWorld, input.normal);
	output.normal = normalize(output.normal);
	output.normal = output.normal * 0.5f + 0.5f;

	output.uv = input.uv;

	return output;
}

struct PS_OUTPUT
{
	float4 color : SV_TARGET0;
	float4 normalLighting : SV_TARGET1;
	uint4 positionMaterial : SV_TARGET2;
	uint id : SV_TARGET3;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PS_OUTPUT PS(VS_OUTPUT input)
{
	PS_OUTPUT output;

	output.color.rgb = gTexture.Sample(gSampler, input.uv).rgb;
	output.color.rgb = pow(abs(output.color.rgb), 2.2f) * gColor.rgb;
	output.color.a = 1.0f;

	output.normalLighting.rgb = input.normal;
	output.normalLighting.a = gLighting;

	output.positionMaterial.rgb = uint3(f32tof16(input.posW.x), f32tof16(input.posW.y), f32tof16(input.posW.z));
	output.positionMaterial.a = uint(uint(gMetallic * 255.0f) << 8 | (uint)(gRoughness * 255.0f));

	output.id = gID;

	return output;
}