#pragma once

#pragma warning(push)
#pragma warning(disable : 4251)

#include "Core.h"

#include <string>

class FOG_API Vector3;
class FOG_API Vector4;
class FOG_API Color;

class FOG_API String
{
public:
	String() : mStr() {}
	String(CHAR ch);
	String(WCHAR ch);
	String(const CHAR* str);
	String(const WCHAR* str) : mStr(str) {}
	String(const String& str) : mStr(str.mStr) {}

	operator const char* ();
	operator const wchar_t* ();

	void Delete(int i);
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

	wchar_t operator[] (int i);

	FOG_API friend String operator+ (const String& str1, const String& str2);
	FOG_API friend bool operator== (const String& str1, const WCHAR* str2);
	FOG_API friend bool operator== (const String& str1, const String& str2);

private:
	std::wstring mStr;
	std::string mBuffer;
};

#pragma warning(pop)