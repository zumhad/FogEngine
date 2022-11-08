#pragma once

#ifdef FOG_LOAD_DLL
#define FOG_API __declspec(dllimport)
#define EXTERN_TEMPLATE extern
#else
#define FOG_API __declspec(dllexport)
#define EXTERN_TEMPLATE
#endif

#include "Definitions.h"

#include "CustomTypes.h"
#include "CustomString.h"
#include "CustomArray.h"