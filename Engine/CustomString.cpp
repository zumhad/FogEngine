#include "CustomString.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Color.h"

#include <windows.h>

using namespace std;

String::String(const CHAR* str)
{
	int size = (int)std::strlen(str);

	mStr.resize(size);

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, mStr.data(), size);
}

String::operator const char* ()
{
	return GetCHAR();
}

String& String::operator= (const String& str)
{
	mStr = str.mStr;

	return *this;
}

String& String::operator= (WCHAR ch)
{
	mStr = ch;

	return *this;
}

String& String::operator= (const WCHAR* str)
{
	mStr = str;

	return *this;
}

FOG_API String operator+ (const String& str1, const String& str2)
{
	String str;
	str.mStr = str1.mStr + str2.mStr;

	return str;
}

String& String::operator+= (WCHAR ch)
{
	mStr += ch;

	return *this;
}

String& String::operator+= (const WCHAR* str)
{
	mStr += str;

	return *this;
}

String& String::operator+= (const String& str)
{
	mStr += str.mStr;

	return *this;
}

int String::Size()
{
	return (int)mStr.size();
}


CHAR* String::GetCHAR()
{
	int size = (int)mStr.size();
	mBuffer.resize(size);

	WideCharToMultiByte(CP_ACP, 0, mStr.c_str(), -1, mBuffer.data(), size + 1, 0, 0);

	return mBuffer.data();
}

WCHAR* String::GetWCHAR()
{
	return mStr.data();
}

int String::Find(const String& str, const String& f)
{
	return (int)str.mStr.find(f.mStr);
}

void String::Copy(String& dest, const String& src, int start, int end)
{
	dest.mStr = src.mStr.substr(start, size_t(end) - start + 1);
}

String String::ToString(int _Val)
{
	return to_wstring(_Val).c_str();
}

String String::ToString(unsigned int _Val)
{
	return to_wstring(_Val).c_str();
}

String String::ToString(long _Val)
{
	return to_wstring(_Val).c_str();
}

String String::ToString(unsigned long _Val)
{
	return to_wstring(_Val).c_str();
}

String String::ToString(long long _Val)
{
	return to_wstring(_Val).c_str();
}

String String::ToString(unsigned long long _Val)
{
	return to_wstring(_Val).c_str();
}

String String::ToString(double _Val)
{
	return to_wstring(_Val).c_str();
}

String String::ToString(float _Val)
{
	return to_wstring(_Val).c_str();
}

String String::ToString(long double _Val)
{
	return to_wstring(_Val).c_str();
}

String String::ToString(const Vector3& _Val)
{
	wstring x = to_wstring(_Val.x);
	wstring y = to_wstring(_Val.y);
	wstring z = to_wstring(_Val.z);
	wstring res = L"(" + x + L", " + y + L", " + z + L")";

	return res.c_str();
}

String String::ToString(const Vector4& _Val)
{
	wstring x = to_wstring(_Val.x);
	wstring y = to_wstring(_Val.y);
	wstring z = to_wstring(_Val.z);
	wstring w = to_wstring(_Val.w);
	std::wstring res = L"(" + x + L", " + y + L", " + z + L", " + w + L")";

	return res.c_str();
}

String String::ToString(const Color& _Val)
{
	wstring r = to_wstring(_Val.r);
	wstring g = to_wstring(_Val.g);
	wstring b = to_wstring(_Val.b);
	wstring res = L"(" + r + L", " + g + L", " + b + L")";

	return res.c_str();
}

bool String::operator== (const WCHAR* str)
{
	return mStr == str;
}

bool String::operator== (const String& str)
{
	return mStr == str.mStr;
}