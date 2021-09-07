#pragma once

#include "Core.h"


class FOG_API CString
{
public:
	static WCHAR* Strcat(WCHAR* dest, const WCHAR* src);
	static WCHAR* Strcpy(WCHAR* dest, const WCHAR* src);
	static WCHAR* Strcpy(WCHAR* dest, const WCHAR* src, uint16 start, uint16 end);
	static uint16 Strlen(const WCHAR* str);
	static bool IsFind(const WCHAR* dest, const WCHAR* src);
	static bool IsFindCh(const WCHAR* dest, const WCHAR ch);
	static bool Equal(const WCHAR* str1, const WCHAR* str2);
	static uint16 FindCh(const WCHAR* str, const WCHAR ch);
	static int32 Atoi(const WCHAR* str);
	static bool Atob(const WCHAR* str);
	static uint16 CountCh(const WCHAR* dest, const WCHAR ch);
	static bool IsInt(const WCHAR* str);
	static uint16 FindStr(const WCHAR* dest, const WCHAR* str);
};