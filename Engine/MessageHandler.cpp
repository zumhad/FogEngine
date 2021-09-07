#include "Application.h"

#include <windowsx.h>

#include "EditHelper.h"
#include "Devices.h"
#include "Direct3D.h"


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
	static int32 xBorder = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
	static int32 yBorder = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
	int32 x = mMouse->GetX();
	int32 y = mMouse->GetY();

	int16 result =
		BORDER_LEFT * (x < xBorder) |
		BORDER_RIGHT * (x >= (RECT_WIDTH(mProperties.editorRect) - xBorder)) |
		BORDER_TOP * (y < (xBorder)) |
		BORDER_BOTTOM * (y >= (RECT_HEIGHT(mProperties.editorRect) - yBorder));

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
	default: return ((y <= mProperties.captionHeight) ? HTCAPTION : HTCLIENT);
	}
}

LRESULT CALLBACK Application::EditorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static Application* pApp = nullptr;

	if (msg == WM_NCCREATE)
	{
		LPCREATESTRUCTW userdata = reinterpret_cast<LPCREATESTRUCTW>(lParam);
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(userdata->lpCreateParams));

		pApp = (Application*)(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
	}

	if (!pApp) return DefWindowProc(hwnd, msg, wParam, lParam);
	static Application& app = *pApp;

	/////////////////////////////////////////////////////////////////////////////////////////////
	switch (msg)
	{
		case WM_ERASEBKGND:
		{
			if (app.mIsGame) break;
			return 0;
		}

		case WM_PAINT:
		{
			if (app.mTime->LockFPS())
			{
				app.mTime->Tick();
				app.CalculateFrameStats(); // fps

				if (app.mProperties.handleKey) app.mProperties.handleKey(); // hotkeys


				app.Update(); //update

				if (app.mIsGame)              //
					app.mDirect->DrawGame();  //
				else                          // draw
					app.mDirect->DrawScene(); //
				app.mDirect->Present();       //


				app.mKeyboard->ResetKeys();
			}


			return 0;
		}

		/*case WM_ENTERSIZEMOVE:
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
			if(!app.mIsGame)
				app.mProperties.editorRect = {0, 0, (int16)LOWORD(lParam), (int16)HIWORD(lParam)};

			if (app.mStarted)
				app.mKeyboard->ResetKeysPress();

			if (wParam == SIZE_MINIMIZED)
			{
				app.mPaused = true;
				app.mMinimized = true;
				app.mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				app.mPaused = false;
				app.mMinimized = false;
				app.mMaximized = true;
			}
			else if (wParam == SIZE_RESTORED)
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
				app.mDirect->ResizeApp(app.mProperties.editorRect.right, app.mProperties.editorRect.bottom);
				app.mDirect->DrawEngine();
				app.mDirect->Present();
			}

			return 0;
		}*/

		case WM_NCCALCSIZE:
		{
			if (app.mIsGame) break;

			if (wParam)
			{
				NCCALCSIZE_PARAMS* params = (NCCALCSIZE_PARAMS*)(lParam);
				//app.AdjustMaxClient(params->rgrc[0]);

				return 0;
			}
			break;
		}

		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				if (app.mStarted)
				{
					if (app.mIsGame)
					{
						app.mDirect->ToggleFullscreen(false);
						app.mDirect->ResizeApp(app.mProperties.gameRect.right, app.mProperties.gameRect.bottom);
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
						//app.mDirect->ToggleFullscreen(true);
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

		case WM_RBUTTONDOWN:
		{
			if (!app.mIsGame && app.CursorInScene())
			{
				SetCapture(hwnd);
				app.mMouse->SetCapture(true);
			}
			return 0;
		}

		case WM_RBUTTONUP:
		{
			if (!app.mIsGame)
			{
				ReleaseCapture();
				app.mMouse->SetCapture(false);
				app.mMouse->SetZeroAxis();
			}

			return 0;
		}

		case WM_NCLBUTTONDOWN: //fix bug
		{
			if (!app.mStarted) break;
			app.mKeyboard->ResetKeysPress();
			break;
		}

		/*case WM_NCHITTEST:
		{
			static POINT p;
			p = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			ScreenToClient(hwnd, &p);
			app.mMouse->SetPos((uint16)p.x, (uint16)p.y);

			if (!app.mStarted) break;
			if (app.mIsGame) return HTCLIENT;

			return app.HitTest();
		}

		case WM_GETMINMAXINFO:
		{
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = MIN_WIDTH;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = MIN_HEIGHT;

			return 0;
		}

		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			if (!(HIWORD(lParam) & KF_REPEAT)) //delete repeat messages
				app.mKeyboard->KeyDown((unsigned int)wParam);			

			return 0;
		}

		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			app.mKeyboard->KeyUp((unsigned int)wParam);
			//if(app.pHandleKey) app.pHandleKey();
			return 0;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(EXIT_SUCCESS);
			return 0;
		}
		*/
		case WM_DESTROY:
		{
			PostQuitMessage(EXIT_SUCCESS);
			return 0;
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}