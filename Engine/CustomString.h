#pragma once

#include "Core.h"

#include <DirectXMath.h>

class FOG_API String
{
public:
	String();
	String(const CHAR* str);
	String(const WCHAR* str);
	String(const String& str);
	~String();

	static void Strcat(WCHAR* dest, const WCHAR* src);
	static void Strcpy(WCHAR* dest, const WCHAR* src);
	static void Strcpy(WCHAR* dest, const WCHAR* src, int start, int end);
	static int Strlen(const WCHAR* str);
	static int Strlen(const CHAR* str);
	static bool IsFind(const WCHAR* dest, const WCHAR* src);
	static bool IsFindCh(const WCHAR* dest, const WCHAR ch);
	static bool Equal(const WCHAR* str1, const WCHAR* str2);
	static int FindCh(const WCHAR* str, const WCHAR ch);
	static int Atoi(const WCHAR* str);
	static bool Atob(const WCHAR* str);
	static int CountCh(const WCHAR* dest, const WCHAR ch);
	static bool IsInt(const WCHAR* str);
	static int FindStr(const WCHAR* dest, const WCHAR* str);
	static String ToStr(int value);
	static String ToStr(DirectX::FXMVECTOR v);
	static String ToStr(DirectX::XMFLOAT3 v);
	static String ToStr(DirectX::XMFLOAT4 v);
	static String ToStr(float f);
	int Length() { return mSize; }
	const WCHAR* Str() { return mStr; }
	CHAR* ToUTF8();

	operator WCHAR* () { return mStr; }
	String operator+= (const String& str);
	String& operator= (const String& str);

	friend String operator+ (const String& str1, const String& str2);
	friend String operator+ (const WCHAR* str1, const String& str2);

private:
	WCHAR* mStr = 0;
	int mSize = 0;
};

String operator+ (const String& str1, const String& str2);
String operator+ (const WCHAR* str1, const String& str2);
