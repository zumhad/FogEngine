#include "Application.h"

#include "Definitions.h"
#include "Direct3D.h"
#include "Timer.h"
#include "Camera.h"
#include "Input.h"
#include "GUI.h"
#include "Cursor.h"
#include "CustomString.h"

#include <windowsx.h>

LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    static bool isSnapshot = false;
    static bool isClickWindow = false;

    switch (msg)
    {
        case WM_PAINT:
        {
            if (Time::LockFPS())
            {
                Time::Tick();

                Cursor::Update();
                Input::Update();

                if (isClickWindow)
                {
                    mPaused = false;
                    isClickWindow = false;
                }

                GUI::Update();

                if (mFoo.update)
                    mFoo.update();

                Camera::Update();

                if (mIsGame)
                    Direct3D::DrawGame();
                else
                    Direct3D::DrawEngine();

                Direct3D::Present();

                Input::UpdateChar();
            }

            if (GetAsyncKeyState(VK_SNAPSHOT) & 0x01)
            {
                isSnapshot = true;
                mPaused = true;
            }

            return 0;
        }

        case WM_CHAR:
        {
            Input::UpdateChar(char(wparam));

            return 0;
        }

        case WM_ERASEBKGND:
        {
            if (mIsGame) break;
            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        case WM_NCHITTEST:
        {
            if (!mStarted) break;
            if (mIsGame) return HTCLIENT;
            
            return HitTest();
        }

        case WM_NCCALCSIZE:
        {
            if (mIsGame) break;

            if (wparam)
            {
                NCCALCSIZE_PARAMS* params = (NCCALCSIZE_PARAMS*)(lparam);
                AdjustMaxClient(params->rgrc[0]);
            }
            return 0;
        }

        case WM_ENTERSIZEMOVE:
        {
            mPaused = true;
            mResizing = true;
            return 0;
        }

        case WM_EXITSIZEMOVE:
        {
            mPaused = false;
            mResizing = false;
            return 0;
        }

        case WM_SIZE:
        {
            if (!mIsGame)
            {
                mEditor.width = GET_X_LPARAM(lparam);
                mEditor.height = GET_Y_LPARAM(lparam);
            }

            if (wparam == SIZE_MINIMIZED)
            {
                mPaused = true;
                mMinimized = true;
                mMaximized = false;
            }
            else if (wparam == SIZE_MAXIMIZED)
            {
                mPaused = false;
                mMinimized = false;
                mMaximized = true;
            }
            else if (wparam == SIZE_RESTORED)
            {
                if (mMinimized)
                {
                    mPaused = false;
                    mMinimized = false;
                }
                else if (mMaximized)
                {
                    mMaximized = false;

                }
                else if (mResizing) // move sizebar
                {
                }
                else // call foo()
                {
                }
            }

            if (mStarted && !mIsGame)
            {
                Direct3D::Resize();
                InitBuffers();
            }

            return 0;
        }

        case WM_ACTIVATE:
        {
            static bool cursorShow = Cursor::GetVisible();


            if (LOWORD(wparam) == WA_INACTIVE)
            {
                if (isSnapshot)
                {
                    isSnapshot = false;

                    return 0;
                }

                if(mIsGame)
                {
                    cursorShow = Cursor::GetVisible();
                    Cursor::SetVisible(true);

                    ShowWindow(hwnd, SW_MINIMIZE);
                }

                mPaused = true;
            }
            else
            {
                if (isClickWindow) return 0;

                if (mIsGame)
                {
                    Cursor::SetVisible(cursorShow);

                    int width = GetSystemMetrics(SM_CXSCREEN);
                    int height = GetSystemMetrics(SM_CYSCREEN);

                    SetWindowPos(mHwnd, HWND_TOPMOST, 0, 0, width, height, 0);
                }
                else
                {
                    RECT rect{};
                    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
                    int width = rect.right - rect.left;
                    int height = rect.bottom - rect.top;

                    SetWindowPos(mHwnd, HWND_NOTOPMOST, 0, 0, width, height, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
                    SetForegroundWindow(mHwnd);
                }

                mPaused = false;
            }

            return 0;
        }

        case WM_MOUSEACTIVATE:
        {
            isClickWindow = true;
            return MA_ACTIVATEANDEAT;
        }

        case WM_MENUCHAR:
        {
            return MAKELRESULT(0, MNC_CLOSE);
        }

        case WM_GETMINMAXINFO:
        {
            ((MINMAXINFO*)lparam)->ptMinTrackSize.x = GetSceneWidth() + GetSceneX();
            ((MINMAXINFO*)lparam)->ptMinTrackSize.y = GetSceneHeight() + GetSceneY();

            RECT rect{};
            SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;

            ((MINMAXINFO*)lparam)->ptMaxTrackSize.x = width;
            ((MINMAXINFO*)lparam)->ptMaxTrackSize.y = height;

            return 0;
        }
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Application::AdjustMaxClient(RECT& rect)
{
    WINDOWPLACEMENT placement = {};
    GetWindowPlacement(mHwnd, &placement);

    if (placement.showCmd != SW_MAXIMIZE) return;

    HMONITOR monitor = MonitorFromWindow(mHwnd, MONITOR_DEFAULTTONULL);

    MONITORINFO monitor_info{};
    monitor_info.cbSize = sizeof(monitor_info);

    if (!GetMonitorInfoW(monitor, &monitor_info)) return;

    rect = monitor_info.rcWork;
}

LRESULT Application::HitTest()
{
    static int xBorder = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
    static int yBorder = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
    int x = Cursor::GetPosition(CURSOR_X);
    int y = Cursor::GetPosition(CURSOR_Y);

    int result =
        BIT_1 * (x < xBorder) |
        BIT_2 * (x >= (Application::GetEditorWidth() - xBorder)) |
        BIT_3 * (y < (xBorder)) |
        BIT_4 * (y >= (Application::GetEditorHeight() - yBorder));

    switch (result)
    {
    case BIT_1: return HTLEFT;
    case BIT_2: return HTRIGHT;
    case BIT_3: return HTTOP;
    case BIT_4: return HTBOTTOM;
    case BIT_3 | BIT_1: return HTTOPLEFT;
    case BIT_3 | BIT_2: return HTTOPRIGHT;
    case BIT_4 | BIT_1: return HTBOTTOMLEFT;
    case BIT_4 | BIT_2: return HTBOTTOMRIGHT;
    }

    if (y <= Application::GetCaptionHeight())
    {
        if (GUI::IsEvent()) return HTCLIENT;
        return HTCAPTION;
    }

    return HTCLIENT;
}
