#include "MathHelper.h"

#include "Timer.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <float.h>

float Math::mEpsilon = (float)FLT_EPSILON;
float Math::mInfinity = (float)FLT_MAX;
float Math::mPI = (float)M_PI;
float Math::mE = (float)M_E;


float Math::Abs(float f)
{
    return std::abs(f);
}

float Math::Sqrt(float f) 
{ 
	return std::sqrt(f); 
}

float Math::Pow(float f, float p) 
{ 
	return std::pow(f, p); 
}

float Math::Infinity() 
{ 
    return mInfinity; 
}

float Math::Epsilon() 
{
    return mEpsilon; 
}

float Math::Min(float a, float b)
{
    return std::min(a, b);
}

float Math::Max(float a, float b) 
{
    return std::max(a, b);
}

int Math::Sign(float f) 
{
    if (f > 0.0f) return 1;
    if (f < 0.0f) return -1;
    return 0;
}

float Math::Clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float Math::Floor(float f)
{
    return std::floor(f);
}

float Math::Repeat(float t, float length)
{
    return t - Floor(t / length) * length;
}

float Math::DeltaAngle(float current, float target)
{
    float num = Repeat(target - current, 360.0f);
    if (num > 180.0f)
    {
        num -= 360.0f;
    }
    return num;
}

float Math::ATan2(float a, float b)
{
    return std::atan2(a, b);
}

float Math::ASin(float a)
{
    return std::asin(a);
}

float Math::ConvertToDegrees(float a)
{
    return a * 180.0f / mPI;
}

float Math::ConvertToRadians(float a)
{
    return a * mPI / 180.0f;
}

float Math::SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime)
{
    target = current + DeltaAngle(current, target);
    return SmoothDamp(current, target, currentVelocity, smoothTime);
}

float Math::SmoothDamp(float current, float target, float& currentVelocity, float smoothTime)
{
    float maxSpeed = mInfinity;
    float deltaTime = Time::DeltaTime();

    smoothTime = Max(0.0001f, smoothTime);
    float num = 2.0f / smoothTime;
    float num2 = num * deltaTime;
    float num3 = 1.0f / (1.0f + num2 + 0.48f * num2 * num2 + 0.235f * num2 * num2 * num2);
    float num4 = current - target;
    float num5 = target;
    float num6 = maxSpeed * smoothTime;
    num4 = Clamp(num4, -num6, num6);
    target = current - num4;
    float num7 = (currentVelocity + num * num4) * deltaTime;
    currentVelocity = (currentVelocity - num * num7) * num3;
    float num8 = target + (num4 + num7) * num3;
    if (num5 - current > 0.0f == num8 > num5)
    {
        num8 = num5;
        currentVelocity = (num8 - num5) / deltaTime;
    }
    return num8;
}