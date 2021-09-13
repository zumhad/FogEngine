#pragma once

#include "Core.h"
#include "CustomTypes.h"
#include "MathHelper.h"

class FOG_API ENGINECLASS
{
public:
	static ENGINECLASS& getInstance() 
	{
		static ENGINECLASS instance;
		return instance;
	}

	struct
	{
		int16 width = 0;
		int16 height = 0;
		Color color = 0;
	} editor;

	struct
	{
		int16 x = 0;
		int16 y = 0;
		int16 width = 0;
		int16 height = 0;
		Color color = 255;
	} scene;

	struct
	{
		int16 width = 0;
		int16 height = 0;
		Color color = 255;
	} game;

	struct
	{
		Vector3 position = { 0.0f, 0.0f, -5.0f };
		Vector3 targetPosition = Vector3::Zero();

		float nearZ = 0.1f;
		float farZ = 1000.0f;
		float fov = 45.0f;
		float rotationSmooth = 1000.0f;
		float moveSmooth = 1000.0f;
	} camera;

	struct
	{
		int16 width = 0;
		int16 height = 0;
	} resolution;

	struct
	{
		void (*start)() = 0;
		void (*update)() = 0;
	} foo;

	int16 captionHeight = 50;
	int16 fpsMax = 0;
	bool isGame = false;
	bool cursorShow = true;
};

static ENGINECLASS& Singlton = ENGINECLASS::getInstance();

