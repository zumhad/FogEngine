#pragma once

#include "Core.h"

#include "Properties.h"
#include "EditHelper.h"

#include <windows.h>

namespace Module
{
    class Time;
    class Direct3D;
    class Mouse;
    class Keyboard;
    class Camera;
}


class FOG_API Application
{
public:
    ~Application();
    Application();

    int32 Run();

private:
    friend void Edit::SetCursorEnabled(bool var);
    friend void Edit::SetSceneX(int16);
    friend void Edit::SetSceneY(int16);
    friend void Edit::SetSceneWidth(int16);
    friend void Edit::SetSceneHeight(int16);
    friend void Edit::SetSceneColor(int16 red, int16 green, int16 blue);
    friend void Edit::MoveSceneX(int16 x);
    friend void Edit::MoveSceneY(int16 y);
    friend HWND Edit::GetHWND();
    friend bool Edit::GetCursorEnabled();
    friend bool Edit::IsAppPaused();
    friend void Edit::CameraRotate(float x, float y, float z);
    friend void Edit::CameraSetTarget(Vector3 target);
    friend void Edit::CameraLookAt();
    friend void Edit::CameraSetRotation(float x, float y, float z);
    friend float Edit::CameraGetRotateX();
    friend float Edit::CameraGetRotateY();
    friend float Edit::CameraGetRotateZ();
    friend void Edit::CameraSetRotationX(float x);
    friend void Edit::CameraSetRotationY(float y);
    friend void Edit::CameraSetRotationZ(float z);
    friend void Edit::CameraMoveLocal(float x, float y, float z);
    friend void Edit::CameraMoveGlobal(float x, float y, float z);

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    void InitWindow();
    void AdjustMaxClient(RECT& rect);
    LRESULT HitTest();
    void SetCursorState(bool var);
    bool CursorInScene();
    void InitApp();
    void InitModules();
    void InitBuffers();

protected:
    ENGINECLASS& prop = Singlton;

private:
    HWND mHwnd = 0;
    
    Module::Time* mTime = 0;
    Module::Keyboard* mKeyboard = 0;
    Module::Direct3D* mDirect = 0;
    Module::Camera* mCamera = 0;
    Module::Mouse* mMouse = 0;

    bool mIsGame = false;
    bool mMinimized = false;
    bool mMaximized = false;
    bool mResizing = false;
    bool mPaused = false;
    bool mStarted = false;
};


