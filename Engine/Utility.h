#pragma once

#include "Core.h"

#include "CustomString.h"

FOG_API void MessageBoxError(const String& file, int line, const String& msg);

#if defined(DEBUG) | defined(_DEBUG)
#define FOG_TRACE(x) \
		do \
		{ \
			HRESULT hr = (x); \
			if (FAILED(hr)) { MessageBoxError(__FILEW__, __LINE__, L#x); } \
		} while (false)
#else
#define FOG_TRACE(x) (x)
#endif

#if defined(DEBUG) | defined(_DEBUG)
#define FOG_ASSERT(x) \
		do \
		{ \
			if (!(x)) { MessageBoxError(__FILEW__, __LINE__, L#x); } \
		} while (false)
#else
#define FOG_ASSERT(x) (x)
#endif

#if defined(DEBUG) | defined(_DEBUG)
#define FOG_ERROR(x) \
		do \
		{ \
			MessageBoxError(__FILEW__, __LINE__, x); \
		} while (false)
#else
#define FOG_ERROR(x) (x)
#endif
