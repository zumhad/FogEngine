#pragma once

#include "Core.h"
#include "CustomTypes.h"
#include "MathHelper.h"


#define MAX_DIRECTIONAL_LIGHT 16
#define MAX_POINT_LIGHT 16


class FOG_API Properties
{
public:
	static Properties& getInstance()
	{
		static Properties instance;
		return instance;
	}

	struct
	{
		int width = 0;
		int height = 0;
		Color color = 0;
	} editor;

	struct
	{
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
		Color color = 255;
	} scene;

	struct
	{
		int width = 0;
		int height = 0;
		Color color = 255;
	} game;

	struct
	{
		float nearZ = 0.1f;
		float farZ = 1000.0f;
		float fov = 60.0f;
		float rotationSmooth = 1000.0f;
		float moveSmooth = 1000.0f;
	} camera;

	struct
	{
		int width = 0;
		int height = 0;
	} resolution;

	struct
	{
		void (*start)() = 0;
		void (*update)() = 0;
	} foo;

	WCHAR path[MAX_PATH]{};

	int captionHeight = 50;
	int fpsMax = 0;
	bool isGame = false;
	bool cursorShow = true;
};

static Properties& Singlton = Properties::getInstance();

