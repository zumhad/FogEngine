#pragma once

#define FOG_LOAD_DLL

#include "Definitions.h"
#include "MathHelper.h"
#include "Vector3.h"
#include "Application.h"
#include "Input.h"
#include "Timer.h"
#include "Camera.h"
#include "GUI.h"
#include "ObjectManager.h"
#include "Light.h"
#include "Mesh.h"
#include "Cursor.h"
#include "Quaternion.h"
#include "Button.h"
#include "Static.h"
#include "Picking.h"
#include "Text.h"
#include "BufferManager.h"
#include "RayCasting.h"


#define CREATE_APPLICATION(x)\
int wmain()\
{\
	return Engine::Start(x());\
}