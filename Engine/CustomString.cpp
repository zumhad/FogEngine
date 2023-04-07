#include "CustomString.h"

#include <Windows.h>
#include <cwchar>
#include <cstdlib>


using namespace DirectX;

String::~String()
{ 
	SAFE_DELETE_ARR(mStr);
	mSize = 0;
}

String::String()
{
	mStr = new wchar_t[1];
	mStr[0] = L'\0';
}

String::String(const CHAR* str)
{
	if (str == 0)
	{
		mStr = new wchar_t[1];
		mStr[0] = L'\0';
	}
	else
	{
		mSize = Strlen(str);
		mStr = new wchar_t[mSize + 1];
		mbstowcs(mStr, str, mSize);
		mStr[mSize] = L'\0';
	}
}

String::String(const WCHAR* str)
{
	if (str == 0)
	{
		mStr = new wchar_t[1];
		mStr[0] = L'\0';
	}
	else
	{
		mSize = Strlen(str);
		mStr = new wchar_t[mSize + 1];
		Strcpy(mStr, str);
	}
}

String::String(const String& str)
{
	*this = str;
}

String& String::operator= (const String& str)
{
	if (str.mStr == 0)
	{
		mStr = new WCHAR[1];
		mStr[0] = L'\0';
	}
	else
	{
		SAFE_DELETE_ARR(mStr);

		mSize = str.mSize;
		mStr = new WCHAR[mSize + 1];
		Strcpy(mStr, str.mStr);
		mStr[mSize] = L'\0';
	}

	return *this;
}

FOG_API String operator+ (const String& str1, const String& str2)
{
	int len = str1.mSize + str2.mSize;
	wchar_t* buff = new wchar_t[len + 1];

	String::Strcpy(buff, str1.mStr);
	String::Strcat(buff, str2.mStr);

	String res = buff;
	SAFE_DELETE_ARR(buff);

	return res;
}

FOG_API String operator+ (const WCHAR* str1, const String& str2)
{
	String res = String(str1) + str2;
	return res;
}

String String::operator+= (const String& str)
{
	*this = *this + str;

	return *this;
}

String String::ToStr(float f)
{
	WCHAR buffer[50];
	swprintf(buffer, 50, L"%f", f);

	String res = buffer;

	return res;
}

String String::ToStr(Vector3 v)
{
	String res = L"{" + ToStr(v.x) + L"," + ToStr(v.y) + L"," + ToStr(v.z) + L"}";
	return res;
}

String String::ToStr(Color c)
{
	String res = L"{" + ToStr(c.r) + L"," + ToStr(c.g) + L"," + ToStr(c.b) + L"," + ToStr(c.a) + L"}";
	return res;
}

String String::ToStr(FXMVECTOR v)
{
	XMFLOAT4 f;
	XMStoreFloat4(&f, v);

	String res = L"{" + ToStr(f.x) + L"," + ToStr(f.y) + L"," + ToStr(f.z) + L"," + ToStr(f.w) + L"}";
	return res;
}

String String::ToStr(DirectX::XMFLOAT4 v)
{
	String res = L"{" + ToStr(v.x) + L"," + ToStr(v.y) + L"," + ToStr(v.z) + L"," + ToStr(v.w) + L"}";
	return res;
}

String String::ToStr(int i)
{
	WCHAR buffer[50];
	swprintf(buffer, 50, L"%d", i);

	String res = buffer;

	return res;
}


void String::Strcat(WCHAR* dest, const WCHAR* src)
{
	wcscat(dest, src);
}

void String::Strcpy(WCHAR* dest, const WCHAR* src)
{
	wcscpy(dest, src);
}

void String::Strcpy(WCHAR* dest, const WCHAR* src, int start, int end)
{
	dest[end - start + 1] = '\0';
	wcsncpy(dest, src + start, (size_t)end - start + 1);
}

int String::Strlen(const WCHAR* str)
{
	return (int)wcslen(str);
}

int String::Strlen(const CHAR* str)
{
	return (int)strlen(str);
}

bool String::IsFind(const WCHAR* dest, const WCHAR* src)
{
	int destLen = Strlen(dest);
	int srcLen = Strlen(src);

	for (int i = 0; i < destLen; i++)
	{
		for (int j = 0; j < srcLen; j++)
		{
			if (src[j] != dest[i + j]) break;
			if (j == srcLen - 1) return true;
		}
	}
	return false;
}

bool String::IsFindCh(const WCHAR* dest, const WCHAR ch)
{
	int destLen = Strlen(dest);

	for (int i = 0; i < destLen; i++)
	{
		if (dest[i] == ch) return true;
	}
	return false;
}

bool String::Equal(const WCHAR* str1, const WCHAR* str2)
{
	int str1Len = Strlen(str1);
	int str2Len = Strlen(str2);

	if (str1Len != str2Len) return false;

	for (int i = 0; i < str1Len; i++)
	{
		if (str1[i] != str2[i]) return false;
	}

	return true;
}

int String::FindCh(const WCHAR* str, WCHAR ch)
{
	int index = 0;

	int i = 0;
	while (str[i++] != ch) index++;

	return index;
}

int String::CountCh(const WCHAR* dest, const WCHAR ch)
{
	int res = 0;
	int destLen = Strlen(dest);

	for (int i = 0; i < destLen; i++)
	{
		if (dest[i] == ch) res++;
	}
	return res;
}

bool String::IsInt(const WCHAR* str)
{
	int strLen = Strlen(str);

	for (int i = 0; i < strLen; i++)
	{
		if (str[i] > L'9' || str[i] < L'0') return false;
	}
	return true;
}

int String::Atoi(const WCHAR* str)
{
	return wcstol(str, 0, 10);
}

bool String::Atob(const WCHAR* str)
{ 
	if (Equal(str, L"true")) return true;
	if (Equal(str, L"false")) return false;

	return str;
}

int String::FindStr(const WCHAR* dest, const WCHAR* str)
{
	int destLen = Strlen(dest);
	int strLen = Strlen(str);

	for (int i = 0; i < destLen - strLen + 1; i++)
	{
		for (int j = 0; j < strLen; j++)
		{
			if (dest[i + j] != str[j]) break;

			if (j == strLen - 1) return i;
		}
	}
	return 0;
}

CHAR* String::ToUTF8()
{
	int len = WideCharToMultiByte(CP_UTF8, 0, mStr, -1, 0, 0, 0, 0);
	CHAR* utf8 = new CHAR[len];
	WideCharToMultiByte(CP_UTF8, 0, mStr, -1, utf8, len, 0, 0);
	return utf8;
}