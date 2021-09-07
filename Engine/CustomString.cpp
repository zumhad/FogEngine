#include "CustomString.h"

#include <string>


WCHAR* CString::Strcat(WCHAR* dest, const WCHAR* src)
{
	return wcscat(dest, src);
}

WCHAR* CString::Strcpy(WCHAR* dest, const WCHAR* src)
{
	return wcscpy(dest, src);
}

WCHAR* CString::Strcpy(WCHAR* dest, const WCHAR* src, uint16 start, uint16 end)
{
	dest[end - start + 1] = '\0';
	return wcsncpy(dest, src + start, end - start + 1);;
}

uint16 CString::Strlen(const WCHAR* str)
{
	uint16 res = 0;

	uint16 i = 0;
	while ((str[i] != L'\0'))
	{
		if ((str[i] == L'\n')) break;
		res++; i++;
	}

	return res;
}

bool CString::IsFind(const WCHAR* dest, const WCHAR* src)
{
	uint16 destLen = Strlen(dest);
	uint16 srcLen = Strlen(src);

	for (uint16 i = 0; i < destLen; i++)
	{
		for (uint16 j = 0; j < srcLen; j++)
		{
			if (src[j] != dest[i + j]) break;
			if (j == srcLen - 1) return true;
		}
	}
	return false;
}

bool CString::IsFindCh(const WCHAR* dest, const WCHAR ch)
{
	uint16 destLen = Strlen(dest);

	for (uint16 i = 0; i < destLen; i++)
	{
		if (dest[i] == ch) return true;
	}
	return false;
}

bool CString::Equal(const WCHAR* str1, const WCHAR* str2)
{
	uint16 str1Len = Strlen(str1);
	uint16 str2Len = Strlen(str2);

	if (str1Len != str2Len) return false;

	for (uint16 i = 0; i < str1Len; i++)
	{
		if (str1[i] != str2[i]) return false;
	}

	return true;
}

uint16 CString::FindCh(const WCHAR* str, WCHAR ch)
{
	uint16 index = 0;

	uint16 i = 0;
	while (str[i++] != ch) index++;

	return index;
}

uint16 CString::CountCh(const WCHAR* dest, const WCHAR ch)
{
	uint16 res = 0;
	uint16 destLen = Strlen(dest);

	for (uint16 i = 0; i < destLen; i++)
	{
		if (dest[i] == ch) res++;
	}
	return res;
}

bool CString::IsInt(const WCHAR* str)
{
	uint16 strLen = Strlen(str);

	for (uint16 i = 0; i < strLen; i++)
	{
		if (str[i] > L'9' || str[i] < L'0') return false;
	}
	return true;
}

int32 CString::Atoi(const WCHAR* str)
{
	return wcstol(str, 0, 10);
}

bool CString::Atob(const WCHAR* str)
{ 
	if (Equal(str, L"true")) return true;
	if (Equal(str, L"false")) return false;

	return str;
}

uint16 CString::FindStr(const WCHAR* dest, const WCHAR* str)
{
	uint16 destLen = Strlen(dest);
	uint16 strLen = Strlen(str);

	for (uint16 i = 0; i < destLen - strLen + 1; i++)
	{
		for (uint16 j = 0; j < strLen; j++)
		{
			if (dest[i + j] != str[j]) break;

			if (j == strLen - 1) return i;
		}
	}
	return 0;
}