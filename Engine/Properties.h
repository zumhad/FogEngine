#pragma once

#include "Core.h"
#include "MathHelper.h"

struct CAMERACLASS
{
	Vector3 position = { 0.0f, 5.0f, -5.0f };
	Vector3 targetPosition = Vector3::Zero();

	float nearZ = 0.1f;
	float farZ = 1000.0f;
	float fov = 45.0f;
	float rotationSmooth = 1000.0f;
	float moveSmooth = 1000.0f;
};


struct ENGINECLASS
{
	Rect sceneRect = { 0, 0, 800, 600 };
	COLORREF sceneColor = RGB(255, 255, 255);

	Rect editorRect = { 0, 0, 1280, 720 };
	COLORREF editorColor = RGB(0, 0, 0);

	Rect gameRect = { 0, 0, 800, 600 };
	COLORREF gameColor = RGB(255, 255, 255);

	uint16 captionHeight = 0;
	bool showCursor = true;
	int16 maxFps = 0;
	bool isGame = false;

	CAMERACLASS camera;
	void (*handleKey)() = 0;
	void (*init)() = 0;
};

struct GAMECLASS
{
	COLORREF gameColor = RGB(0, 0, 0);
	bool showCursor = true;
	int16 maxFps = 0;

	CAMERACLASS camera;
	void (*handleKey)() = 0;
};