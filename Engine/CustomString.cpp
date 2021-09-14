#include "CustomString.h"


WCHAR* CString::Strcat(WCHAR* dest, const WCHAR* src)
{
	return wcscat(dest, src);
}

WCHAR* CString::Strcpy(WCHAR* dest, const WCHAR* src)
{
	return wcscpy(dest, src);
}

WCHAR* CString::Strcpy(WCHAR* dest, const WCHAR* src, short start, short end)
{
	dest[end - start + 1] = '\0';
	return wcsncpy(dest, src + start, end - start + 1);;
}

short CString::Strlen(const WCHAR* str)
{
	short res = 0;

	short i = 0;
	while ((str[i] != L'\0'))
	{
		if ((str[i] == L'\n')) break;
		res++; i++;
	}

	return res;
}

bool CString::IsFind(const WCHAR* dest, const WCHAR* src)
{
	short destLen = Strlen(dest);
	short srcLen = Strlen(src);

	for (short i = 0; i < destLen; i++)
	{
		for (short j = 0; j < srcLen; j++)
		{
			if (src[j] != dest[i + j]) break;
			if (j == srcLen - 1) return true;
		}
	}
	return false;
}

bool CString::IsFindCh(const WCHAR* dest, const WCHAR ch)
{
	short destLen = Strlen(dest);

	for (short i = 0; i < destLen; i++)
	{
		if (dest[i] == ch) return true;
	}
	return false;
}

bool CString::Equal(const WCHAR* str1, const WCHAR* str2)
{
	short str1Len = Strlen(str1);
	short str2Len = Strlen(str2);

	if (str1Len != str2Len) return false;

	for (short i = 0; i < str1Len; i++)
	{
		if (str1[i] != str2[i]) return false;
	}

	return true;
}

short CString::FindCh(const WCHAR* str, WCHAR ch)
{
	short index = 0;

	short i = 0;
	while (str[i++] != ch) index++;

	return index;
}

short CString::CountCh(const WCHAR* dest, const WCHAR ch)
{
	short res = 0;
	short destLen = Strlen(dest);

	for (short i = 0; i < destLen; i++)
	{
		if (dest[i] == ch) res++;
	}
	return res;
}

bool CString::IsInt(const WCHAR* str)
{
	short strLen = Strlen(str);

	for (short i = 0; i < strLen; i++)
	{
		if (str[i] > L'9' || str[i] < L'0') return false;
	}
	return true;
}

int CString::Atoi(const WCHAR* str)
{
	return wcstol(str, 0, 10);
}

bool CString::Atob(const WCHAR* str)
{ 
	if (Equal(str, L"true")) return true;
	if (Equal(str, L"false")) return false;

	return str;
}

short CString::FindStr(const WCHAR* dest, const WCHAR* str)
{
	short destLen = Strlen(dest);
	short strLen = Strlen(str);

	for (short i = 0; i < destLen - strLen + 1; i++)
	{
		for (short j = 0; j < strLen; j++)
		{
			if (dest[i + j] != str[j]) break;

			if (j == strLen - 1) return i;
		}
	}
	return 0;
}