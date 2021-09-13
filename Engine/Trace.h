#pragma once

#include <string>
#include <windows.h>
#include <comdef.h>
//#include "dxerr.h"


#if defined(DEBUG) | defined(_DEBUG) // TRACE
#ifndef FOG_TRACE
#define FOG_TRACE(x)                                            \
	{                                                           \
		HRESULT hr = (x);                                       \
		if(FAILED(hr))                                          \
		{                                                       \
			DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true); \
			OutputDebugString(L"Error");                        \
		}                                                       \
	}
#endif

#else
#ifndef FOG_TRACE
#define FOG_TRACE(x) (x)
#endif
#endif 

#if defined(DEBUG) | defined(_DEBUG) // ASSERT
#ifndef FOG_ASSERT
#define FOG_ASSERT(x)                                                \
	{                                                                \
		if(!(x))                                                       \
		{                                                            \
			DXTrace(__FILEW__, (DWORD)__LINE__, E_FAIL, L#x, true);  \
		}                                                            \
	}
#endif

#else
#ifndef FOG_ASSERT
#define FOG_ASSERT(x) (x)
#endif
#endif 

