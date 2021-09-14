#include "Application.h"

#include "Definitions.h"
#include "EditHelper.h"
#include "Devices.h"
#include "Direct3D.h"
#include "Time.h"

#include <windowsx.h>

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
    static short xBorder = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
    static short yBorder = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
    short x = mMouse->GetX();
    short y = mMouse->GetY();

    short result =
        BORDER_LEFT * (x < xBorder) |
        BORDER_RIGHT * (x >= (Singlton.editor.width - xBorder)) |
        BORDER_TOP * (y < (xBorder)) |
        BORDER_BOTTOM * (y >= (Singlton.editor.height - yBorder));

    switch (result)
    {
    case BORDER_LEFT: return HTLEFT;
    case BORDER_RIGHT: return HTRIGHT;
    case BORDER_TOP: return HTTOP;
    case BORDER_BOTTOM: return HTBOTTOM;
    case BORDER_TOP | BORDER_LEFT: return HTTOPLEFT;
    case BORDER_TOP | BORDER_RIGHT: return HTTOPRIGHT;
    case BORDER_BOTTOM | BORDER_LEFT: return HTBOTTOMLEFT;
    case BORDER_BOTTOM | BORDER_RIGHT: return HTBOTTOMRIGHT;
    default: return ((y <= Singlton.captionHeight) ? HTCAPTION : HTCLIENT);
    }
}


LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    static Application* pApp = 0;

    if (msg == WM_NCCREATE)
    {
        LPCREATESTRUCTW userdata = reinterpret_cast<LPCREATESTRUCTW>(lparam);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(userdata->lpCreateParams));
        pApp = (Application*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (!pApp) return DefWindowProc(hwnd, msg, wparam, lparam);
    static Application& app = *pApp;

    switch (msg)
    {
        case WM_PAINT:
        {
            if (app.mTime->LockFPS())
            {
                app.mTime->Tick();

                if (Singlton.foo.update)
                    Singlton.foo.update();

                app.mMouse->Update();
                app.mCamera->Update(Module::Time::DeltaTime());
                app.mDirect->UpdateViewMatrix(app.mCamera->GetViewMatrix());

                

                if (app.mIsGame)
                    app.mDirect->DrawGame();
                else
                    app.mDirect->DrawScene();



                app.mDirect->Present();

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
                app.mDirect->ResizeEditor();
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
                    {
                        //app.mDirect->ResizeApp(app.mProperties.gameRect.right, app.mProperties.gameRect.bottom);
                        ShowWindow(hwnd, SW_MINIMIZE);
                    }

                    if (app.mMouse->GetState() == false)
                        app.SetCursorState(true);

                    //app.mTimer->Stop();
                }

                app.mPaused = true;
            }
            else
            {
                if (app.mStarted)
                {
                    if (app.mIsGame)
                    {
                        //app.mDirect->ResizeApp(app.mProperties.gameRect.right, app.mProperties.gameRect.bottom);
                    }

                    if (app.mMouse->GetEnabled() == false && app.mMouse->GetState() == true)
                        app.SetCursorState(false);

                    //app.mTimer->Start();
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
            ((MINMAXINFO*)lparam)->ptMinTrackSize.x = 800;
            ((MINMAXINFO*)lparam)->ptMinTrackSize.y = 600;

            return 0;
        }

        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            if (!(HIWORD(lparam) & KF_REPEAT)) //delete repeat messages
                app.mKeyboard->KeyDown((unsigned int)wparam);

            return 0;
        }

        case WM_SYSKEYUP:
        case WM_KEYUP:
        {
            app.mKeyboard->KeyUp((unsigned int)wparam);
            return 0;
        }
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}