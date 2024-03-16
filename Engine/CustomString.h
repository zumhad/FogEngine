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
	String(WCHAR ch);
	String(const WCHAR* str) : mStr(str) {}
	String(const String& str) : mStr(str.mStr) {}

	operator WCHAR* ();
	operator const WCHAR* () const;

	void Clear();
	void Delete(int i);
	void Delete(int start, int end);
	int Size();

	static String ToString(short _Val);
	static String ToString(unsigned short _Val);
	static String ToString(int _Val);
	static String ToString(unsigned int _Val);
	static String ToString(long long _Val);
	static String ToString(unsigned long long _Val);
	static String ToString(float _Val, int precision);
	static String ToString(const Vector3& _Val, int precision);
	static String ToString(const Vector4& _Val, int precision);
	static String ToString(const Color& _Val, int precision);

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
};

class FOG_API StringConverter
{
public:
	StringConverter(CHAR ch);
	StringConverter(const CHAR* str);
	StringConverter(WCHAR ch);
	StringConverter(const WCHAR* str);
	StringConverter(const String& str);
	
	operator CHAR* ();
	operator const CHAR* () const;
	operator WCHAR* ();
	operator const WCHAR* () const;

private:
	std::string mString1;
	std::wstring mString2;
};

#pragma warning(pop)