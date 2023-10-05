#pragma once

#ifdef FOG_LOAD_DLL
#define FOG_API __declspec(dllimport)
#else
#define FOG_API __declspec(dllexport)
#endif

#include "Definitions.h"

typedef char				CHAR;
typedef wchar_t				WCHAR;