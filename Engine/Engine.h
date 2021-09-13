#pragma once

#define FOG_LOAD_DLL

#include "Definitions.h"
#include "Application.h"
#include "Input.h"
#include "Time.h"
#include "EditHelper.h"

using Module::Time;



#define CREATE_APPLICATION()\
int32 wmain()\
{\
	Edit::CheckDebug();\
	Editor* app = new Editor;\
	int32 exitCode = app->Run();\
	SAFE_DELETE(app);\
	return exitCode;\
}