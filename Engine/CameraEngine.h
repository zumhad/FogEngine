#pragma once

#include "Core.h"

#include "Camera.h"

class CameraEngine : public Camera
{
public:
	static Matrix GetViewMatrix() { return mView; }
	static Matrix GetProjMatrix() { return mProj; }
	static void Setup();
	static void Update();
	static void Set3D(bool var);

private:
	static void UpdateProperties();
};

