#pragma once

#include "Core.h"

#include "CustomString.h"

class PathHelper
{
public:
    static String GetAssetsPath();
    static String GetProjectPath();
    static String GetEnginePath();
    static String GetEditorPath();
};

