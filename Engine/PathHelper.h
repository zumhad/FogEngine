#pragma once

#include "Core.h"

class PathHelper
{
public:
    static void GetAssetsPath(String& dest);
    static void GetProjectPath(String& dest);
    static void GetEnginePath(String& dest);
    static void GetEditorPath(String& dest);
};

