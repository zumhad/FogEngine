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

float3 Uncharted2ToneMapping(float3 color)
{
    color *= 16;
    color = color / (1 + color);
    color = pow(abs(color), 1.0 / 2.2);
    return color;
}

struct PS_OUTPUT
{
    float4 color : SV_TARGET0;
};

Texture2D<float4> gTextureColor : register(t0);
SamplerState gSampler : register(s0);

PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT output;

    //FxaaTex tex;
    //tex.smpl = gSampler;
    //tex.tex = gTextureColor;
    //FxaaFloat2 fxaaQualityRcpFrame = float2(0.00125f, 0.00167f);
    //FxaaFloat fxaaQualitySubpix = 1.0f;
    //FxaaFloat fxaaQualityEdgeThreshold = 0.063f;
    //FxaaFloat fxaaQualityEdgeThresholdMin = 0.0312f;
    //output.color = FxaaPixelShader(input.uv, tex, fxaaQualityRcpFrame, fxaaQualitySubpix, fxaaQualityEdgeThreshold, fxaaQualityEdgeThresholdMin);

    output.color = gTextureColor.Sample(gSampler, input.uv.xy);
    output.color.a = 1.0f;

    return output;
}