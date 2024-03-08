cbuffer Buffer0 : register(b0)
{
    float4x4 gViewProj;
};

cbuffer Buffer1 : register(b1)
{
    float4x4 gWorld;
};

struct VS_INPUT0
{
    float3 pos : POSITION;
};

struct VS_OUTPUT0
{
    float4 pos : SV_POSITION;
};

VS_OUTPUT0 VS0(VS_INPUT0 input)
{
    VS_OUTPUT0 output;

    output.pos = mul(gWorld, float4(input.pos, 1.0f));
    output.pos = mul(gViewProj, output.pos);

    return output;
}

struct PS_OUTPUT0
{
    int2 offset : SV_TARGET0;
};

PS_OUTPUT0 PS0(VS_OUTPUT0 input)
{
    PS_OUTPUT0 output;

    output.offset = int2(0, 0);

    return output;
}

/////////////////////////////////////////////////////////////////////////////////////////

struct VS_INPUT1
{
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT1
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_OUTPUT1 VS1(VS_INPUT1 input)
{
    VS_OUTPUT1 output;

    float x = input.uv.x * 2.0f - 1.0f;
    float y = 1.0f - input.uv.y * 2.0f;

    output.pos = float4(x, y, 1.0f, 1.0f);
    output.uv = input.uv.xy;

    return output;
}

struct PS_OUTPUT1
{
    float4 color : SV_TARGET0;
};

Texture2D<int2> gTexture : register(t0);

cbuffer Buffer3 : register(b0)
{
    float4 gColor;
    int gOutlineWidth;
    int gWidth;
    int gHeight; float pad1;
};

PS_OUTPUT1 PS1(VS_OUTPUT1 input)
{
    PS_OUTPUT1 output;
    output.color = float4(0.0f, 0.0f, 0.0f, 1.0f);

    input.uv *= float2(gWidth, gHeight);

    int2 offset = gTexture.Load(int3(input.uv, 0));

    if ((offset.x != 0 || offset.y != 0) && dot(offset, offset) <= (gOutlineWidth * gOutlineWidth))
    {
        output.color = gColor;
        output.color.a = dot(output.color.rgb, float3(0.299, 0.587, 0.114));
        return output;
    }
    else discard;

    return output;
}