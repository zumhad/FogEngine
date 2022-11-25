#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#define APP_NAME L"FogEngine"
#define APP_CLASS APP_NAME L"Class"

#define BIT_1 0b00000001
#define BIT_2 0b00000010
#define BIT_3 0b00000100
#define BIT_4 0b00001000
#define BIT_5 0b00010000
#define BIT_6 0b00100000
#define BIT_7 0b01000000
#define BIT_8 0b10000000

#define SAFE_DELETE(x) if(x) { delete x; x = 0; }
#define SAFE_DELETE_ARR(x) if(x) { delete[] x; x = 0; }
#define SAFE_RELEASE(x) if(x) { x->Release(); x = 0; }

#define EXT L"fog"
#define DOT_EXT L"." EXT