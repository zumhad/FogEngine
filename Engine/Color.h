#pragma once

#include "Core.h"

#include <DirectXMath.h>

class FOG_API Color
{
public:
	Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
	Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b), a(1.0f) {}
	Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
	Color(const float* arr) : r(arr[0]), g(arr[1]), b(arr[2]), a(arr[3]) {}
	Color(DirectX::FXMVECTOR V);
	Color(const DirectX::XMFLOAT4& f);

	operator DirectX::XMFLOAT4() const;
	operator DirectX::XMVECTOR() const;
	operator const float* () const;

	bool operator == (const Color& c) const;
	bool operator != (const Color& c) const;

	Color& operator= (DirectX::FXMVECTOR v);
	Color& operator= (const DirectX::XMFLOAT4& f);
	Color& operator+= (const Color& c);
	Color& operator-= (const Color& c);
	Color& operator*= (const Color& c);
	Color& operator*= (float f);
	Color& operator/= (const Color& c);

	Color operator+ () const;
	Color operator- () const;

public:
	float r, g, b, a;
};

FOG_API Color operator+ (const Color& c1, const Color& c2);
FOG_API Color operator- (const Color& c1, const Color& c2);
FOG_API Color operator* (const Color& c1, const Color& c2);
FOG_API Color operator* (const Color& c, float f);
FOG_API Color operator/ (const Color& c1, const Color& c2);
FOG_API Color operator* (float f, const Color& c);

