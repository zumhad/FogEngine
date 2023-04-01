#pragma once

#ifdef FOG_LOAD_DLL
#define FOG_API __declspec(dllimport)
#define FOG_EXTERN extern
#else
#define FOG_API __declspec(dllexport)
#define FOG_EXTERN 
#endif

#include "Definitions.h"

#include <windows.h>