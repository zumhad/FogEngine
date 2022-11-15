#include "ApplicationEngine.h"

#include "Definitions.h"
#include "Devices.h"
#include "Direct3D.h"
#include "Time.h"
#include "CameraEngine.h"
#include "GUI.h"

#include <windowsx.h>


void ApplicationEngine::AdjustMaxClient(RECT& rect)
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

LRESULT ApplicationEngine::HitTest()
{
    static int xBorder = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
    static int yBorder = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
    int x = mMouse->GetX();
    int y = mMouse->GetY();

    int result =
        BIT_1 * (x < xBorder) |
        BIT_2 * (x >= (Singlton.editor.width - xBorder)) |
        BIT_3 * (y < (xBorder)) |
        BIT_4 * (y >= (Singlton.editor.height - yBorder));

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
        default: return ((y <= Singlton.captionHeight) ? HTCAPTION : HTCLIENT);
    }
}


LRESULT CALLBACK ApplicationEngine::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    static ApplicationEngine* pApp = 0;

    if (msg == WM_NCCREATE)
    {
        LPCREATESTRUCTW userdata = reinterpret_cast<LPCREATESTRUCTW>(lparam);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(userdata->lpCreateParams));
        pApp = (ApplicationEngine*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (!pApp) return DefWindowProc(hwnd, msg, wparam, lparam);
    static ApplicationEngine& app = *pApp;

    switch (msg)
    {
        case WM_PAINT:
        {
            if (app.mTime->LockFPS())
            {
                app.mTime->Tick();

                app.mMouse->Update();

                if (Singlton.foo.update)
                    Singlton.foo.update();

                CameraEngine::Update(Module::Time::DeltaTime());
                
                if (app.mIsGame)
                    Direct3D::DrawGame();
                else
                    Direct3D::DrawEngine();
                
                Direct3D::Present();

                app.mKeyboard->ResetKeys();
                app.mMouse->ResetKeys();
            }

            return 0;
        }

        case WM_MOUSEMOVE:
        {
            app.mMouse->SetPos(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            return 0;
        }

        case WM_MOVE:
        {
            if (app.mStarted)
            {
                app.mKeyboard->ResetKeysPress();
                app.mMouse->ResetKeysPress();
            }

            return 0;
        }

        case WM_ERASEBKGND:
        {
            if (app.mIsGame) break;
            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        case WM_NCHITTEST:
        {
            static POINT p;
            p = { GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
            ScreenToClient(hwnd, &p);
            app.mMouse->SetPos((short)p.x, (short)p.y);

            if (!app.mStarted) break;
            if (app.mIsGame) return HTCLIENT;
            if (GUI::ClickTest(p.x, p.y)) return HTCLIENT;

            return app.HitTest();
        }

        case WM_NCCALCSIZE:
        {
            if (app.mIsGame) break;

            if (wparam)
            {
                NCCALCSIZE_PARAMS* params = (NCCALCSIZE_PARAMS*)(lparam);
                app.AdjustMaxClient(params->rgrc[0]);

                return 0;
            }
            break;
        }

        case WM_ENTERSIZEMOVE:
        {
            app.mResizing = true;
            return 0;
        }

        case WM_EXITSIZEMOVE:
        {
            app.mResizing = false;
            return 0;
        }

        case WM_SIZE:
        {
            if (!app.mIsGame)
            {
                Singlton.editor.width = (short)LOWORD(lparam);
                Singlton.editor.height = (short)HIWORD(lparam);
            }

            if (app.mStarted)
            {
                app.mKeyboard->ResetKeysPress();
                app.mMouse->ResetKeysPress();
            }

            if (wparam == SIZE_MINIMIZED)
            {
                app.mPaused = true;
                app.mMinimized = true;
                app.mMaximized = false;
            }
            else if (wparam == SIZE_MAXIMIZED)
            {
                app.mPaused = false;
                app.mMinimized = false;
                app.mMaximized = true;
            }
            else if (wparam == SIZE_RESTORED)
            {
                if (app.mMinimized)
                {
                    app.mMinimized = false;
                }
                else if (app.mMaximized)
                {
                    app.mMaximized = false;

                }
                else if (app.mResizing) // move sizebar
                {
                }
                else // call foo()
                {
                }
            }

            if (app.mStarted && !app.mIsGame)
            {
                CameraEngine::RestartMatrix();
                Direct3D::ResizeEditor();
                app.InitBuffers();
            }

            return 0;
        }

        case WM_ACTIVATE:
        {
            if (LOWORD(wparam) == WA_INACTIVE)
            {
                if (app.mStarted)
                {
                    if (app.mIsGame)
                        ShowWindow(hwnd, SW_MINIMIZE);

                    if (app.mMouse->GetState() == false)
                        app.SetCursorState(true);
                }

                app.mPaused = true;
            }
            else
            {
                if (app.mStarted)
                {
                    if (app.mMouse->GetEnabled() == false && app.mMouse->GetState() == true)
                        app.SetCursorState(false);
                }

                app.mPaused = false;
            }
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            app.mMouse->KeyDown(0);

            if (!app.mIsGame)
                SetCapture(hwnd);

            return 0;
        }

        case WM_LBUTTONUP:
        {
            app.mMouse->KeyUp(0);

            if (!app.mIsGame)
                ReleaseCapture();

            if (!app.mIsGame)
                GUI::Click(app.mMouse->GetX(), app.mMouse->GetY());

            return 0;
        }

        case WM_RBUTTONDOWN:
        {
            app.mMouse->KeyDown(1);

            if (!app.mIsGame)
            {
                SetCapture(hwnd);

                if(app.CursorInScene())
                    app.mMouse->SetCapture(true);
            }
            return 0;
        }

        case WM_RBUTTONUP:
        {
            app.mMouse->KeyUp(1);

            if (!app.mIsGame)
            {
                ReleaseCapture();
                app.mMouse->SetCapture(false);
                app.mMouse->SetZeroAxis();
            }

            return 0;
        }

        case WM_GETMINMAXINFO:
        {
            ((MINMAXINFO*)lparam)->ptMinTrackSize.x = Singlton.scene.width;
            ((MINMAXINFO*)lparam)->ptMinTrackSize.y = Singlton.scene.height + Singlton.captionHeight;

            return 0;
        }

        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            if (!(HIWORD(lparam) & KF_REPEAT)) //delete repeat messages
                app.mKeyboard->KeyDown((short)wparam);

            return 0;
        }

        case WM_SYSKEYUP:
        case WM_KEYUP:
        {
            app.mKeyboard->KeyUp((short)wparam);
            return 0;
        }
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}