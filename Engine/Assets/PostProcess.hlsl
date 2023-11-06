#define FXAA_PC 1
#define FXAA_HLSL_5 1
#define FXAA_QUALITY__PRESET 39
#define FXAA_GATHER4_ALPHA 1

#include "FXAA.hlsl"

struct VS_INPUT
{
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

    float x = input.uv.x * 2.0f - 1.0f;
    float y = 1.0f - input.uv.y * 2.0f;

    output.pos = float4(x, y, 1.0f, 1.0f);
    output.uv = input.uv.xy;

    return output;
}

cbuffer cbPostProcessBuffer : register(b0)
{
    float gInvWidth;
    float gInvHeight; float2 pad;
};

struct PS_OUTPUT
{
    float4 color : SV_TARGET0;
};

Texture2D<float4> gTextureColor : register(t0);
SamplerState gSampler : register(s0);

PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT output;

    FxaaTex tex;
    tex.smpl = gSampler;
    tex.tex = gTextureColor;
    output.color = FxaaPixelShader(input.uv, tex, float2(gInvWidth, gInvHeight), 1.0f, 0.063f, 0.0312f);

    output.color.a = 1.0f;

    return output;
}
