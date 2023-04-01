#include "BufferManager.h"

#include "DepthMap.h"
#include "SelectMap.h"
#include "ObjectManager.h"
#include "PassMap.h"

std::function<void(bool)> BufferManager::mCallback;

void BufferManager::SetEnable(TypeBuffer type, bool enable)
{
	switch (type)
	{
		case TypeBuffer::DepthMap:
		{
			DepthMap::SetEnable(enable);
			break;
		}

		case TypeBuffer::SelectMap:
		{
			SelectMap::SetEnable(enable);
			break;
		}

		case TypeBuffer::ColorMap:
		{
			break;
		}
	}

	PassMap::UpdateShader();
}

void BufferManager::SetDraw(TypeBuffer type)
{
	switch (type)
	{
		case TypeBuffer::DepthMap:
		{
			if (mCallback) mCallback(false);
			mCallback = DepthMap::SetDraw;

			DepthMap::SetDraw(true);

			break;
		}

		case TypeBuffer::SelectMap:
		{
			if (mCallback) mCallback(false);
			mCallback = SelectMap::SetDraw;

			SelectMap::SetDraw(true);

			break;
		}

		case TypeBuffer::ColorMap:
		{
			break;
		}
	}

	PassMap::UpdateShader();
}