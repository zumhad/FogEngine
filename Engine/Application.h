#pragma once

#include "Core.h"

#include "Input.h"
#include "Timer.h"


class FOG_API Application
{
public:
    static void SetCursorEnabled(bool var);
    static void SetSceneX(int x);
    static void SetSceneY(int y);
    static void SetSceneWidth(int width);
    static void SetSceneHeight(int height);
    static void SetSceneColor(float r, float g, float b);
    static void MoveSceneX(int x);
    static void MoveSceneY(int y);

    static bool CursorInScene();
    static int GetSceneX();
    static int GetSceneY();
    static int GetSceneWidth();
    static int GetSceneHeight();

    static void Exit();
    static bool GetCursorEnabled();
    static bool IsAppPaused();
    static void SaveProject();
    static void RestartShader();

protected:
    static void InitBuffers();

protected:
    static Module::Keyboard* mKeyboard;
    static Module::Mouse* mMouse;

    static bool mIsGame;
    static bool mMinimized;
    static bool mMaximized;
    static bool mResizing;
    static bool mPaused;
    static bool mStarted;
};


