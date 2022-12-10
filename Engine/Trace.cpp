#include "Trace.h"

#include <cstdio>

#define BUFFER_SIZE 3000

HRESULT WINAPI DXTrace(const WCHAR* strFile, DWORD dwLine, HRESULT hr, const WCHAR* strMsg, bool bPopMsgBox)
{
    WCHAR strBufferLine[128];
    WCHAR strBufferError[256];
    WCHAR strBuffer[BUFFER_SIZE];

    swprintf_s(strBufferLine, 128, L"%lu", dwLine);
    if (strFile)
    {
        swprintf_s(strBuffer, BUFFER_SIZE, L"%ls(%ls): ", strFile, strBufferLine);
        OutputDebugStringW(strBuffer);
    }

    size_t nMsgLen = (strMsg) ? wcsnlen_s(strMsg, 1024) : 0;
    if (nMsgLen > 0)
    {
        OutputDebugStringW(strMsg);
        OutputDebugStringW(L" ");
    }

    swprintf_s(strBufferError, 256, L"(0x%0.8x)", hr);
    swprintf_s(strBuffer, BUFFER_SIZE, L"%ls", strBufferError);
    OutputDebugStringW(strBuffer);

    OutputDebugStringW(L"\n");

#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
    if (bPopMsgBox)
    {
        WCHAR strBufferFile[MAX_PATH];
        wcscpy_s(strBufferFile, MAX_PATH, L"");
        if (strFile)
            wcscpy_s(strBufferFile, MAX_PATH, strFile);

        WCHAR strBufferMsg[1024];
        wcscpy_s(strBufferMsg, 1024, L"");
        if (nMsgLen > 0)
            swprintf_s(strBufferMsg, 1024, L"Calling: %ls\n", strMsg);

        swprintf_s(strBuffer, BUFFER_SIZE, L"File: %ls\nLine: %ls\nError Code: %ls\n%lsDo you want to debug the application?",
            strBufferFile, strBufferLine, strBufferError, strBufferMsg);

        int nResult = MessageBoxW(GetForegroundWindow(), strBuffer, L"Unexpected error encountered", MB_YESNO | MB_ICONERROR);
        if (nResult == IDYES)
            DebugBreak();
    }
#else
    UNREFERENCED_PARAMETER(bPopMsgBox);
#endif

    return hr;
}