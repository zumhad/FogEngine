#pragma once

#include "Core.h"

#include "CustomString.h"

FOG_API void MessageBoxError(const String& file, int line, const String& msg);

#if defined(DEBUG) | defined(_DEBUG)
#define FOG_TRACE(x) \
	{ \
		HRESULT hr = (x); \
		if(FAILED(hr)) { MessageBoxError(__FILEW__, __LINE__, L#x); } \
	}
#else
#define FOG_TRACE(x) (x)
#endif

#if defined(DEBUG) | defined(_DEBUG)
#define FOG_ASSERT(x) \
	{ \
		if(!(x)) { MessageBoxError(__FILEW__, __LINE__, L#x); } \
	}
#else
#define FOG_ASSERT(x) (x)
#endif

#if defined(DEBUG) | defined(_DEBUG)
#define FOG_ERROR(x) \
	{ \
		MessageBoxError(__FILEW__, __LINE__, x); \
	}
#else
#define FOG_ERROR(x) (x)
#endif
