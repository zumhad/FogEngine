#pragma once

#include "Core.h"

#include "Color.h"

struct FOG_API APPCLASS
{
    struct
    {
        Color color;
    } editor;

    struct
    {
        int width = 0;
        int height = 0;
        Color color;
    } game;

    struct
    {
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
        Color color;
    } scene;

    struct
    {
        float nearZ = 0.1f;
        float farZ = 10000.0f;
        float fov = 60.0f;
        float rotationSmooth = 1000.0f;
        float moveSmooth = 1000.0f;
    } camera;

    struct
    {
        void (*start)() = 0;
        void (*update)() = 0;
    } foo;

    int captionHeight = 0;
    int fpsMax = 0;
    bool isGame = false;
    bool cursorShow = true;
};