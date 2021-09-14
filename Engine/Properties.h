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
		short width = 0;
		short height = 0;
		Color color = 0;
	} editor;

	struct
	{
		short x = 0;
		short y = 0;
		short width = 0;
		short height = 0;
		Color color = 255;
	} scene;

	struct
	{
		short width = 0;
		short height = 0;
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
		short width = 0;
		short height = 0;
	} resolution;

	struct
	{
		void (*start)() = 0;
		void (*update)() = 0;
	} foo;

	short captionHeight = 50;
	short fpsMax = 0;
	bool isGame = false;
	bool cursorShow = true;
};

static ENGINECLASS& Singlton = ENGINECLASS::getInstance();

