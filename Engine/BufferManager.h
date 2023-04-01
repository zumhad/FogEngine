#pragma once

#include "Core.h"

#include <vector>
#include <functional>

template class FOG_API std::function<void(bool)>;


enum TypeBuffer
{
	DepthMap,
	SelectMap,
	ColorMap
};

class FOG_API BufferManager
{
public:
	static void SetEnable(TypeBuffer type, bool enable);
	static void SetDraw(TypeBuffer type);

private:
	static std::function<void(bool)> mCallback;
};

