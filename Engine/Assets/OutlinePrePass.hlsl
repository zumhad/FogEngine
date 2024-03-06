
RWTexture2D<int2> gTexture : register(u0);

cbuffer cbOutlineBuffer : register(b0)
{
    int gStepSize;
    int gWidth;
    int gHeight; float pad;
};

groupshared int2 gData[1024];

[numthreads(32, 32, 1)]
void CS(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex)
{
    gData[GI] = gTexture[DTid.xy];
    GroupMemoryBarrierWithGroupSync();

    if (DTid.x >= (uint)gWidth || DTid.y >= (uint)gHeight)
        return;

    float bestDist = 32767.0f;
    int2 bestOffset = int2(32767, 32767);

    [unroll]
    for (int i = -1; i <= 1; i++)
    {
        [unroll]
        for (int j = -1; j <= 1; j++)
        {
            int2 offset;

            int2 step = int2(i, j) * gStepSize;
            int2 uv1 = DTid.xy + step;
            int2 uv2 = GTid.xy + step;

            bool test = (uv1.x >= 0 && uv1.y >= 0 && uv1.x < gWidth && uv1.y < gHeight);

            if (uv2.x >= 0 && uv2.y >= 0 && uv2.x < 32 && uv2.y < 32)
            {
                offset = (test ? gData[uv2.y * 32 + uv2.x] : int2(32767, 32767));
            }
            else
            {
                offset = (test ? gTexture[uv1] : int2(32767, 32767));
            }

            int2 coord = offset - step;
            float dist = dot(coord, coord);

            if (dist < bestDist && offset.x != 32767)
            {
                bestDist = dist;
                bestOffset = coord;
            }
        }
    }

    if (bestOffset.x != 32767)
        gTexture[DTid.xy] = bestOffset;
}