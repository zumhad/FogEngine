#include "Utility.h"

#include "Application.h"
#include "CustomString.h"

FOG_API void MessageBoxError(const String& file, int line, const String& msg)
{	
	Application::Close();

	String str;
	str += L"File: " + file + L"\n";
	str += L"Line: " + String::ToString(line) + L"\n";
	str += L"Msg: " + msg;

	MessageBox(0, str, L"Error", MB_OK | MB_DEFAULT_DESKTOP_ONLY | MB_ICONERROR);
}