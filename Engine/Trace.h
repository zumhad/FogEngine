#pragma once

#include <windows.h>

HRESULT WINAPI DXTrace(const WCHAR* strFile, DWORD dwLine, HRESULT hr, const WCHAR* strMsg, bool bPopMsgBox);


#if defined(DEBUG) | defined(_DEBUG) // TRACE
#define FOG_TRACE(x) \
	{ \
		HRESULT hr = (x); \
		if(FAILED(hr)) { DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true); } \
	}
#else
#define FOG_TRACE(x) (x)
#endif 

#if defined(DEBUG) | defined(_DEBUG) // ASSERT
#define FOG_ASSERT(x) \
	{ \
		if(!(x)) DXTrace(__FILEW__, (DWORD)__LINE__, 0, L#x, true); \
	}
#else
#define FOG_ASSERT(x) (x)
#endif 

#if defined(DEBUG) | defined(_DEBUG)
#define FOG_MSG(str)              DXTrace( __FILEW__, (DWORD)__LINE__, 0, str, false )
#define FOG_ERROR(str)           DXTrace( __FILEW__, (DWORD)__LINE__, 0, str, true )
#else
#define DXTRACE_MSG(str)
#define DXTRACE_ERR(str)
#endif 