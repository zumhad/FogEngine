#pragma once

#include "Core.h"
#include "Application.h"
#include "Properties.h"


class FOG_API ApplicationEngine : public Application
{
public:
    ApplicationEngine() {}
    static int Run();
    static void Shotdown();
    static ApplicationEngine& Get();

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    static void CheckDebug();
    static void DefaultProp();
    static void InitWindow();
    static void AdjustMaxClient(RECT& rect);
    static LRESULT HitTest();
    static void InitApp();
    static void InitModules();
    static HWND GetHWND() { return mHwnd; }
};