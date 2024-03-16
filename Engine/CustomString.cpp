#include "CustomString.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Color.h"
#include "Utility.h"
#include "MathHelper.h"

#include <windows.h>

using namespace std;

String::String(WCHAR ch)
{
	mStr += ch;
}

String::operator WCHAR* ()
{
	return mStr.data();
}

String::operator const WCHAR* () const
{
	return mStr.data();
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

wchar_t String::operator[] (int i)
{
	return mStr[i];
}

int String::Size()
{
	return (int)mStr.size();
}

void String::Delete(int i)
{
	mStr.erase(i);
}

void String::Delete(int start, int end)
{
	mStr.erase(start, end - start + 1);
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
	wchar_t buffer[12];
	swprintf_s(buffer, 12, L"%d", _Val);

	return String(buffer);
}

String String::ToString(unsigned int _Val)
{
	wchar_t buffer[11];
	swprintf_s(buffer, 11, L"%du", _Val);

	return String(buffer);
}

String String::ToString(short _Val)
{
	wchar_t buffer[7];
	swprintf_s(buffer, 7, L"%hd", _Val);

	return String(buffer);
}

String String::ToString(unsigned short _Val)
{
	wchar_t buffer[6];
	swprintf_s(buffer, 6, L"%hu", _Val);

	return String(buffer);
}

String String::ToString(long long _Val)
{
	wchar_t buffer[21];
	swprintf_s(buffer, 21, L"%lld", _Val);

	return String(buffer);
}

String String::ToString(unsigned long long _Val)
{
	wchar_t buffer[21];
	swprintf_s(buffer, 21, L"%llu", _Val);

	return String(buffer);
}

String String::ToString(float _Val, int precision)
{
	FOG_ASSERT(Math::Abs(_Val) <= FLOAT_MAX_NUMBER(precision));
	FOG_ASSERT(precision >= 0 && precision <= FLOAT_MAX_DIGITS);

	wchar_t buffer0[6];
	swprintf_s(buffer0, 6, L"%%%d.%df", FLOAT_MAX_DIGITS - precision, precision);

	wchar_t buffer1[11];
	swprintf_s(buffer1, 9, buffer0, _Val);
	return String(buffer1);
}

String String::ToString(const Vector3& _Val, int precision)
{
	String x = ToString(_Val.x, precision);
	String y = ToString(_Val.y, precision);
	String z = ToString(_Val.z, precision);
	String res = L"(" + x + L", " + y + L", " + z + L")";

	return res;
}

String String::ToString(const Vector4& _Val, int precision)
{
	String x = ToString(_Val.x, precision);
	String y = ToString(_Val.y, precision);
	String z = ToString(_Val.z, precision);
	String w = ToString(_Val.w, precision);
	String res = L"(" + x + L", " + y + L", " + z + L", " + w + L")";

	return res;
}

String String::ToString(const Color& _Val, int precision)
{
	String r = ToString(_Val.r, precision);
	String g = ToString(_Val.g, precision);
	String b = ToString(_Val.b, precision);
	String res = L"(" + r + L", " + g + L", " + b + L")";

	return res;
}

void String::Clear()
{
	mStr.clear();
}

FOG_API bool operator== (const String& left, const WCHAR* right)
{
	return left.mStr == right;
}

FOG_API bool operator== (const String& left, const String& right)
{
	return left.mStr == right.mStr;
}

//////////////////////////////////////////////////////////////////////////

StringConverter::StringConverter(CHAR ch)
{
	mString1 = ch;

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, mString1.data(), -1, mString2.data(), 1);
}

StringConverter::StringConverter(const CHAR* str)
{
	mString1 = str;

	mString2.resize(mString1.size());

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, mString1.data(), -1, mString2.data(), (int)mString2.size());
}

StringConverter::StringConverter(WCHAR ch)
{
	mString2 = ch;

	WideCharToMultiByte(CP_ACP, MB_PRECOMPOSED, mString2.data(), -1, mString1.data(), 1, 0, 0);
}

StringConverter::StringConverter(const WCHAR* str)
{
	mString2 = str;

	mString1.resize(mString2.size());

	WideCharToMultiByte(CP_ACP, 0, mString2.data(), -1, mString1.data(), (int)mString1.size(), 0, 0);
}

StringConverter::StringConverter(const String& str)
{
	mString2 = str;

	mString1.resize(mString2.size());

	WideCharToMultiByte(CP_ACP, 0, mString2.data(), -1, mString1.data(), (int)mString1.size(), 0, 0);
}

StringConverter::operator CHAR* ()
{
	return mString1.data();
}

StringConverter::operator const CHAR* () const
{
	return mString1.data();
}

StringConverter::operator WCHAR* ()
{
	return mString2.data();
}

StringConverter::operator const WCHAR* () const
{
	return mString2.data();
}