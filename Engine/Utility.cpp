#include "Utility.h"

#include "Application.h"

#include <cstdio>

FOG_API void MessageBoxError(const WCHAR* file, int line, const WCHAR* msg)
{	
	Application::Close();

	WCHAR fileBuffer[MAX_PATH];
	WCHAR lineBuffer[MAX_PATH];
	WCHAR msgBuffer[MAX_PATH];
	WCHAR buffer[3 * MAX_PATH];

	swprintf_s(fileBuffer, MAX_PATH, L"File: %s\n", file);
	swprintf_s(lineBuffer, MAX_PATH, L"Line: %i\n", line);
	swprintf_s(msgBuffer, MAX_PATH, L"Msg: %s\n", msg);

	swprintf_s(buffer, 3 * MAX_PATH, L"%s%s%s", fileBuffer, lineBuffer, msgBuffer);

	MessageBox(0, buffer, L"Error", MB_OK | MB_DEFAULT_DESKTOP_ONLY | MB_ICONERROR);
}