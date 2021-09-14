#pragma once

#include "Core.h"


class FOG_API CString
{
public:
	static WCHAR* Strcat(WCHAR* dest, const WCHAR* src);
	static WCHAR* Strcpy(WCHAR* dest, const WCHAR* src);
	static WCHAR* Strcpy(WCHAR* dest, const WCHAR* src, short start, short end);
	static short Strlen(const WCHAR* str);
	static bool IsFind(const WCHAR* dest, const WCHAR* src);
	static bool IsFindCh(const WCHAR* dest, const WCHAR ch);
	static bool Equal(const WCHAR* str1, const WCHAR* str2);
	static short FindCh(const WCHAR* str, const WCHAR ch);
	static int Atoi(const WCHAR* str);
	static bool Atob(const WCHAR* str);
	static short CountCh(const WCHAR* dest, const WCHAR ch);
	static bool IsInt(const WCHAR* str);
	static short FindStr(const WCHAR* dest, const WCHAR* str);
};