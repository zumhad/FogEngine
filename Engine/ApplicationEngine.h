#pragma once

#include "Core.h"
#include "Application.h"
#include "Properties.h"


class FOG_API ApplicationEngine : public Application
{
public:
    ~ApplicationEngine();
    int Run();

public:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    void CheckDebug();
    void DefaultProp();
    void InitWindow();
    void AdjustMaxClient(RECT& rect);
    LRESULT HitTest();
    void SetCursorState(bool var);
    bool CursorInScene();
    void InitApp();
    void InitModules();

public:
    static HWND mHwnd;
};