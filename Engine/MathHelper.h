#pragma once

#include "Core.h"

class FOG_API Math
{
public:
    static int Sign(float f);
    static float Abs(float f);
    static float Sqrt(float f);
    static float Pow(float f, float p);
    static float Infinity();
    static float Epsilon();
    static float Min(float a, float b);
    static float Max(float a, float b);
    static float Clamp(float value, float min, float max);
    static float Floor(float f);
    static float Repeat(float t, float length);
    static float DeltaAngle(float current, float target);
    static float SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime);
    static float SmoothDamp(float current, float target, float& currentVelocity, float smoothTime);
    static float ATan2(float a, float b);
    static float ASin(float a);
    static float ConvertToDegrees(float a);
    static float ConvertToRadians(float a);

private:
    static float mEpsilon;
    static float mInfinity;
    static float mPI;
    static float mE;
};

