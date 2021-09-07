#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#define MIN_WIDTH 1920
#define MIN_HEIGHT 1080

#define APP_NAME L"FogEngine"
#define APP_CLASS APP_NAME L"Class"

#define BORDER_LEFT 0b0001 // window rect border
#define BORDER_RIGHT 0b0010
#define BORDER_TOP 0b0100
#define BORDER_BOTTOM 0b1000

#define SAFE_DELETE(x) if(x) { delete x; x = 0; }
#define SAFE_DELETE_ARR(x) if(x) { delete[] x; x = 0; }
#define SAFE_RELEASE(x) if(x) { x->Release(); x = 0; }

#define RECT_WIDTH(x) (x.right - x.left)
#define RECT_HEIGHT(x) (x.bottom - x.top)
//#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define RGBA(r,g,b,a) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)|(((DWORD)(BYTE)(a))<<24)))
#define SetRValue(c, r) (c = ((COLORREF)((c & 0x00FFFF00) | ((BYTE)(r)))))
#define SetGValue(c, g) (c = ((COLORREF)((c & 0x00FF00FF) | ((BYTE)(g) << 8))))
#define SetBValue(c, b) (c = ((COLORREF)((c & 0x0000FFFF) | ((BYTE)(b) << 16))))

#define EXT L"fog"
#define DOT_EXT L"." EXT