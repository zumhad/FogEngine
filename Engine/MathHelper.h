#pragma once

#define _USE_MATH_DEFINES

#include "Core.h"

#include "CustomArray.h"

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
    static float Ceil(float f);
    static float Round(float f);
    static float Repeat(float t, float length);
    static float DeltaAngle(float current, float target);
    static float SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime);
    static float SmoothDamp(float current, float target, float& currentVelocity, float smoothTime);
    static float ATan2(float a, float b);
    static float ASin(float a);
    static float ConvertToDegrees(float a);
    static float ConvertToRadians(float a);
    static float Float16ToFloat32(unsigned short float16);

    template <typename T>
    static int BinarySearch(Array<T>& arr, T t);

private:
    static float mEpsilon;
    static float mInfinity;
    static float mPI;
    static float mE;
};

template <typename T>
int Math::BinarySearch(Array<T>& arr, T t)
{
    int size = arr.Size();
    int left = 0;
    int right = size - 1;

    while (left <= right)
    {
        int m = left + (right - left) / 2;

        if (arr[m] == t)
            return m;

        if (arr[m] < t)
            left = m + 1;
        else
            right = m - 1;
    }

    return -1;
}