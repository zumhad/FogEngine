#pragma once

#include "Core.h"

class FOG_API String;

class PathHelper
{
public:
    static String GetAssetsPath();
    static String GetProjectPath();
    static String GetEnginePath();
    static String GetEditorPath();
};

