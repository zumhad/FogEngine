cbuffer cbTextBuffer : register(b0)
{
	float gIngWidth;
	float gInvHeight; float2 pad;
};

struct VS_INPUT
{
	float2 pos : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	float x = 2.0f * (input.pos.x * gIngWidth) - 1.0f;
	float y = 1.0f - (input.pos.y * gInvHeight) * 2.0f;

	output.pos = float4(x, y, 0.0f, 1.0f);
	output.uv = input.uv;

	return output;
}

cbuffer cbTextBuffer : register(b1)
{
	float4 gColor;
};

struct PS_OUTPUT
{
	float4 color : SV_TARGET;
};

Texture2D<float> gTexture : register(t0);
SamplerState gSampler;

PS_OUTPUT PS(VS_OUTPUT input)
{
	PS_OUTPUT output;

	float c = gTexture.Sample(gSampler, input.uv);
	float4 color = gColor * c;

	if (c == 0.0f) clip(-1);

	output.color = color;

	return output;
}