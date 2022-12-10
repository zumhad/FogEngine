#include "ApplicationEngine.h"

#include "Definitions.h"
#include "Devices.h"
#include "Direct3D.h"
#include "TimerEngine.h"
#include "CameraEngine.h"
#include "InputEngine.h"
#include "GUI.h"
#include "Cursor.h"

#include <windowsx.h>
#include <string>



LRESULT CALLBACK ApplicationEngine::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    static ApplicationEngine& app = ApplicationEngine::Get();

    switch (msg)
    {
        case WM_PAINT:
        {
            if (TimeEngine::LockFPS())
            {
                TimeEngine::Tick();

                InputEngine::Update();

                if (Singlton.foo.update)
                    Singlton.foo.update();

                CameraEngine::Update();

                if (app.mIsGame)
                    Direct3D::DrawGame();
                else
                    Direct3D::DrawEngine();

                Direct3D::Present();
            }

            return 0;
        }

        case WM_MOVE:
        {
            if (app.mStarted && !app.mIsGame)
            {
                Direct3D::ResizeEditor();
                app.InitBuffers();
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
            if (!app.mStarted) break;
            if (app.mIsGame) return HTCLIENT;
            if (GUI::ClickTest()) return HTCLIENT;

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
            app.mPaused = true;
            app.mResizing = true;
            return 0;
        }

        case WM_EXITSIZEMOVE:
        {
            app.mPaused = false;
            app.mResizing = false;
            return 0;
        }

        case WM_SIZE:
        {
            if (!app.mIsGame)
            {
                Singlton.editor.width = GET_X_LPARAM(lparam);
                Singlton.editor.height = GET_Y_LPARAM(lparam);
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
                    app.mPaused = false;
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
                Direct3D::ResizeEditor();
                app.InitBuffers();
            }

            return 0;
        }

        case WM_ACTIVATE:
        {
            if (LOWORD(wparam) == WA_INACTIVE)
            {
                if (app.mStarted && app.mIsGame)
                {
                    Singlton.cursorShow = Cursor::GetVisible();
                    Cursor::SetVisible(true);

                    ShowWindow(hwnd, SW_MINIMIZE);
                }

                app.mPaused = true;
            }
            else
            {
                if (app.mStarted && app.mIsGame)
                {
                    Cursor::SetVisible(Singlton.cursorShow);

                    ShowWindow(hwnd, SW_MAXIMIZE);
                }

                app.mPaused = false;
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
           // if (!(HIWORD(lparam) & KF_REPEAT)) //delete repeat messages
                //app.mKeyboard->KeyDown((short)wparam);

            return 0;
        }

        case WM_SYSKEYUP:
        case WM_KEYUP:
        {
            //app.mKeyboard->KeyUp((short)wparam);
            return 0;
        }
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}