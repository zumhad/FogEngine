#pragma once

#include "Core.h"

class Math
{
public:
    static float Infinity() { return mInfinity; }
    static float Epsilon() { return mEpsilon; }
    static float Sqrt(float f);
    static float Pow(float f, float p);
    static float Max(float a, float b) { return a > b ? a : b; }
    static float Clamp(float value, float min, float max)
    {
        if (value < min)
            value = min;
        else if (value > max)
            value = max;
        return value;
    }

private:
    static float mEpsilon;
    static float mInfinity;
    static float mPI;
    static float mE;
};

