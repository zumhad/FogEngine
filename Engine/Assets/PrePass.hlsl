cbuffer cbDefaultBuffer : register(b0)
{
	float4x4 gWorldViewProj;
	float4x4 gWorld;
};

cbuffer cbSelectBuffer : register(b1)
{
	uint gID; float3 pad;
};

struct VS_INPUT
{
	float3 PosL : POSITION;
};

struct VS_OUTPUT
{
	float4 PosH : SV_POSITION;

#ifdef SELECT_MAP
	float3 PosW : POSITION;
#endif
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	output.PosH = mul(gWorldViewProj, float4(input.PosL, 1.0f));

#ifdef SELECT_MAP
	output.PosW = mul(gWorld, float4(input.PosL, 1.0f)).xyz;
#endif

	return output;
}

struct PS_OUTPUT
{
#ifdef SELECT_MAP
	float4 pos_id : SV_TARGET0;
#endif
};

PS_OUTPUT PS(VS_OUTPUT input)
{
	PS_OUTPUT output;

#ifdef SELECT_MAP
	output.pos_id = float4(input.PosW, gID);
#endif
	
	return output;
}