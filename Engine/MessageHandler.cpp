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


LRESULT CALLBACK ApplicationEngine::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case WM_PAINT:
        {
            if (TimeEngine::LockFPS())
            {
                TimeEngine::Tick();

                InputEngine::Update();
                GUI::Click();

                if (Singlton.foo.update)
                    Singlton.foo.update();

                CameraEngine::Update();

                if (mIsGame)
                    Direct3D::DrawGame();
                else
                    Direct3D::DrawEngine();

                Direct3D::Present();
            }

            return 0;
        }

        case WM_MOVE:
        {
            if (mStarted && !mIsGame)
            {
                Direct3D::ResizeEditor();
                InitBuffers();
            }

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
            if (GUI::ClickTest()) return HTCLIENT;

            return HitTest();
        }

        case WM_NCCALCSIZE:
        {
            if (mIsGame) break;

            if (wparam)
            {
                NCCALCSIZE_PARAMS* params = (NCCALCSIZE_PARAMS*)(lparam);
                AdjustMaxClient(params->rgrc[0]);

                return 0;
            }
            break;
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
                Singlton.editor.width = GET_X_LPARAM(lparam);
                Singlton.editor.height = GET_Y_LPARAM(lparam);
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
                Direct3D::ResizeEditor();
                InitBuffers();
            }

            return 0;
        }

        case WM_ACTIVATE:
        {
            if (LOWORD(wparam) == WA_INACTIVE)
            {
                if (mStarted && mIsGame)
                {
                    Singlton.cursorShow = Cursor::GetVisible();
                    Cursor::SetVisible(true);

                    ShowWindow(hwnd, SW_MINIMIZE);
                }

                mPaused = true;
            }
            else
            {
                if (mStarted && mIsGame)
                {
                    Cursor::SetVisible(Singlton.cursorShow);

                    ShowWindow(hwnd, SW_MAXIMIZE);
                }

                mPaused = false;
            }

            return 0;
        }

        case WM_GETMINMAXINFO:
        {
            ((MINMAXINFO*)lparam)->ptMinTrackSize.x = Singlton.scene.width;
            ((MINMAXINFO*)lparam)->ptMinTrackSize.y = Singlton.scene.height + Singlton.captionHeight;
            ((MINMAXINFO*)lparam)->ptMaxTrackSize.x = Singlton.resolution.width;
            ((MINMAXINFO*)lparam)->ptMaxTrackSize.y = Singlton.resolution.height;

            return 0;
        }
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}