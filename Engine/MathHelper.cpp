#include "MathHelper.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <float.h>

float Math::mEpsilon = (float)FLT_EPSILON;
float Math::mInfinity = (float)FLT_MAX;
float Math::mPI = (float)M_PI;
float Math::mE = (float)M_E;




float Math::Sqrt(float f) 
{ 
	return sqrt(f); 
}

float Math::Pow(float f, float p) 
{ 
	return pow(f, p); 
}