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

#define FLOAT_TEST_PRECISION_0(x) (x == 0 ? 8388608.0f : 0.0f)
#define FLOAT_TEST_PRECISION_1(x) (x == 1 ? 524287.0f : FLOAT_TEST_PRECISION_0(x))
#define FLOAT_TEST_PRECISION_2(x) (x == 2 ? 65535.0f : FLOAT_TEST_PRECISION_1(x))
#define FLOAT_TEST_PRECISION_3(x) (x == 3 ? 8192.0f : FLOAT_TEST_PRECISION_2(x))
#define FLOAT_TEST_PRECISION_4(x) (x == 4 ? 511.0f : FLOAT_TEST_PRECISION_3(x))
#define FLOAT_TEST_PRECISION_5(x) (x == 5 ? 63.0f : FLOAT_TEST_PRECISION_4(x))
#define FLOAT_TEST_PRECISION_6(x) (x == 6 ? 7.0f : FLOAT_TEST_PRECISION_5(x))
#define FLOAT_MAX_NUMBER(x) FLOAT_TEST_PRECISION_6(x)

#define FLOAT_MAX_DIGITS 7

#define SAFE_DELETE(x) if(x) { delete x; x = 0; }
#define SAFE_DELETE_ARR(x) if(x) { delete[] x; x = 0; }
#define SAFE_RELEASE(x) if(x) { x->Release(); x = 0; }

#define EXT L"fog"

#define STRING(x) #x
#define WSTRING(x) L#x

#define MAX_POINT_LIGHT 4
#define MAX_CASCADES 4

#define ARRAY_SIZE(x) ARRAYSIZE(x)