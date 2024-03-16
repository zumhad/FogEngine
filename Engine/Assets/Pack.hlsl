float PackFloat8ToFloat16(float a, float b)
{
    unsigned int ui16 = (((unsigned int)(a * 255.0f)) << 8) | ((unsigned int)(b * 255.0f));

    return -32.0f + ui16 / 1024.0f;
}

float2 UnpackFloat16ToFloat8(float a)
{
    unsigned int ui16 = (a + 32.0f) * 1024.0f;

    float f1 = (ui16 >> 8) / 255.0f;
    float f2 = (ui16 & 0x00ff) / 255.0f;

    return float2(f1, f2);
}