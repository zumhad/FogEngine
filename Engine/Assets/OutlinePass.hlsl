

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

struct PS_OUTPUT
{
    float4 color : SV_TARGET0;
};

Texture2D<int2> gTextureOffset : register(t0);

cbuffer cbOutlineBuffer : register(b0)
{
    float4 gColor;
    int gOutlineWidth;
    int gWidth;
    int gHeight; float pad;
};

PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT output;
    output.color = float4(0.0f, 0.0f, 0.0f, 1.0f);

    input.uv *= float2(gWidth, gHeight);

    int2 offset = gTextureOffset.Load(int3(input.uv, 0));

    if ((offset.x != 0 || offset.y != 0) && dot(offset, offset) <= (gOutlineWidth * gOutlineWidth))
    {
        output.color = gColor;
        output.color.a = dot(output.color.rgb, float3(0.299, 0.587, 0.114));
        return output;
    }
    else discard;

    return output;
}