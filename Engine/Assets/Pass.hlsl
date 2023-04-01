cbuffer cbTextureBuffer : register(b0)
{
    int gWidth;
    int gHeight; float2 pad;
};

struct VS_INPUT
{
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD;
};

struct PS_OUTPUT
{
    float4 Color : SV_TARGET0;
};



VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    float x = input.UV.x * 2.0f - 1.0f;
    float y = 1.0f - input.UV.y * 2.0f;

    output.PosH = float4(x, y, 1.0f, 1.0f);
    output.UV = float2(input.UV.x * gWidth, input.UV.y * gHeight);

    return output;
}

Texture2D<float> gTextureDepth : register(t0);
Texture2D<float4> gTextureSelect : register(t1);

PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT output;
    output.Color = float4(0.0f, 0.0f, 0.0f, 1.0f);

#ifdef DEPTH_MAP_DRAW
    float depth = gTextureDepth.Load(int3(input.UV.x, input.UV.y, 0));
    output.Color = float4(depth, depth, depth, 1.0f);
#endif 
    
#ifdef SELECT_MAP_DRAW
    float4 select = gTextureSelect.Load(int3(input.UV.x, input.UV.y, 0));
    output.Color = select;
#endif

    return output;
}
