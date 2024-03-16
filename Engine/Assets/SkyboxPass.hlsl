cbuffer Buffer0 : register(b0)
{
	float4x4 gWorldViewProj;
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
	float2 uv : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = mul(gWorldViewProj, float4(input.pos, 1.0f));
	output.pos.z = 0.0f;
	output.uv = input.uv;

	return output;
}

struct PS_OUTPUT
{
	float4 color : SV_TARGET0;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PS_OUTPUT PS(VS_OUTPUT input)
{
	PS_OUTPUT output;

	output.color.rgb = gTexture.Sample(gSampler, input.uv).rgb;
	output.color.a = dot(output.color.rgb, float3(0.299, 0.587, 0.114));

	return output;
}