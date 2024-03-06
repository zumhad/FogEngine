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
	float4 positionID : SV_TARGET2;
	unsigned int rangeMaterial : SV_TARGET3;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PS_OUTPUT PS(VS_OUTPUT input)
{
	PS_OUTPUT output;

	output.color = gTexture.Sample(gSampler, input.uv);
	output.color.rgb = pow(abs(output.color.rgb), 2.2f) * gColor.rgb;

	output.normalLighting = float4(input.normal, gLighting);
	output.positionID = float4(input.posW, gID);

	unsigned int rangeMaterial = f32tof16(input.pos.w);
	rangeMaterial = (rangeMaterial << 8) | (unsigned int)(gMetallic * 255.0f);
	rangeMaterial = (rangeMaterial << 8) | (unsigned int)(gRoughness * 255.0f);

	output.rangeMaterial = rangeMaterial;

	return output;
}