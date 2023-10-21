#pragma once

#include "Core.h"

#include "ApplicationHelper.h"

#include <windows.h>

class FOG_API String;
class FOG_API Engine;
class FOG_API Cursor;
class FOG_API Direct3D;

class FOG_API Application
{
    friend class Engine; 
    friend class Cursor;
    friend class Direct3D;

public:
    static void SetSceneX(int x);
    static void SetSceneY(int y);
    static void SetSceneWidth(int width);
    static void SetSceneHeight(int height);
    static void SetSceneColor(Color color);

    static int GetSceneX();
    static int GetSceneY();
    static int GetSceneWidth();
    static int GetSceneHeight();
    static Color GetSceneColor();

    static void SetEditorColor(Color color);

    static int GetEditorWidth();
    static int GetEditorHeight();
    static Color GetEditorColor();

    static void SetGameWidth(int width);
    static void SetGameHeight(int height);
    static void SetGameColor(Color color);

    static int GetGameWidth();
    static int GetGameHeight();
    static Color GetGameColor();

    static int GetFpsMax();
    static void SetFpsMax(int fps);

    static int GetCaptionHeight();
    static void SetCaptionHeight(int height);

    static void Minimize();
    static void Restore();
    static void Close();

    static void SaveProject();
    static void OpenFileDialog();

    static bool CursorInScene();
    static bool IsPaused();
    static bool IsResized();
    static bool IsGame();
    static String& GetPath();

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    static int Run(APPCLASS app);
    static void Shotdown();
    static void CheckDebug();
    static void InitWindow();
    static void AdjustMaxClient(RECT& rect);
    static LRESULT HitTest();
    static void InitProp(APPCLASS app);
    static void InitApp(APPCLASS app);
    static void InitModules(APPCLASS app);
    static HWND GetHWND() { return mHwnd; }
    static void InitBuffers();
    //static bool IsMouseActivate();

private:
    static bool mIsGame;
    static bool mMinimized;
    static bool mMaximized;
    static bool mResizing;
    static bool mPaused;
    static bool mStarted;
    static String mPath;
    static int mCaptionHeight;
    static int mFpsMax;
    static bool isMouseActivate;

    static struct
    {
        int width;
        int height;
        Color color;
    } mEditor;

    static struct
    {
        int width;
        int height;
        Color color;
    } mGame;

    static struct
    {
        int x;
        int y;
        int width;
        int height;
        Color color;
    } mScene;

    static struct
    {
        void (*start)();
        void (*update)();
    } mFoo;

    static HWND mHwnd;
};

class FOG_API Engine
{
public:
    static int Start(APPCLASS app)
    {
        int exitCode = Application::Run(app);
        Application::Shotdown();
        
        return exitCode;
    }
};
