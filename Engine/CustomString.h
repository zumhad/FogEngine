#pragma once

#pragma warning(push)
#pragma warning(disable : 4251)

#include "Core.h"

#include <string>

class Vector3;
class Vector4;
class Color;

class FOG_API String
{
public:
	String() : mStr() {}
	String(const CHAR* str);
	String(const WCHAR* str) : mStr(str) {}
	String(const String& str) : mStr(str.mStr) {}

	operator const char* ();

	int Size();
	WCHAR* GetWCHAR();
	CHAR* GetCHAR();

	static String ToString(int _Val);
	static String ToString(unsigned int _Val);
	static String ToString(long _Val);
	static String ToString(unsigned long _Val);
	static String ToString(long long _Val);
	static String ToString(unsigned long long _Val);
	static String ToString(double _Val);
	static String ToString(float _Val);
	static String ToString(long double _Val);
	static String ToString(const Vector3& _Val);
	static String ToString(const Vector4& _Val);
	static String ToString(const Color& _Val);

	static int Find(const String& str, const String& f);
	static void Copy(String& dest, const String& src, int start, int end);

	String& operator+= (WCHAR ch);
	String& operator+= (const WCHAR* str);
	String& operator+= (const String& str);

	String& operator= (WCHAR ch);
	String& operator= (const WCHAR* str);
	String& operator= (const String& str);

	bool operator== (const WCHAR* str);
	bool operator== (const String& str);

	FOG_API friend String operator+ (const String& str1, const String& str2);

private:
	std::wstring mStr;
	std::string mBuffer;
};

#pragma warning(pop)