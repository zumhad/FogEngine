#include "Picking.h"

#include "Direct3D.h"
#include "Application.h"
#include "Cursor.h"
#include "ObjectManager.h"
#include "Vector3.h"
#include "MathHelper.h"
#include "PrePass.h"

#include <DirectXCollision.h>

using namespace DirectX;

ID3D11Texture2D* Picking::mStaging = 0;

Object* Picking::mPickObject = 0;
Vector3 Picking::mPickPosition;

void Picking::Setup()
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = 1;
	desc.Height = 1;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.MiscFlags = 0;

	Direct3D::Device()->CreateTexture2D(&desc, 0, &mStaging);
}

void Picking::Shotdown()
{
	SAFE_RELEASE(mStaging);
}

float decode(unsigned short float16_value)
{
	unsigned int sign = float16_value >> 15;
	unsigned int exponent = (float16_value >> 10) & 0x1F;
	unsigned int fraction = (float16_value & 0x3FF);
	unsigned int float32_value;
	if (exponent == 0)
	{
		if (fraction == 0)
		{
			float32_value = (sign << 31);
		}
		else
		{
			exponent = 127 - 14;
			while ((fraction & (1 << 10)) == 0)
			{
				exponent--;
				fraction <<= 1;
			}
			fraction &= 0x3FF;
			float32_value = (sign << 31) | (exponent << 23) | (fraction << 13);
		}
	}
	else if (exponent == 0x1F)
	{
		float32_value = (sign << 31) | (0xFF << 23) | (fraction << 13);
	}
	else
	{
		float32_value = (sign << 31) | ((exponent + (127 - 15)) << 23) | (fraction << 13);
	}

	return *((float*)&float32_value);
}


void Picking::Pick()
{
	int x, y;

	if (Application::IsGame())
	{
		x = 0;
		y = 0;
	}
	else
	{
		x = Application::GetSceneX();
		y = Application::GetSceneY();
	}

	static D3D11_BOX box{};
	box.left = Cursor::GetPosition(CURSOR_X) - x;
	box.right = box.left + 1;
	box.top = Cursor::GetPosition(CURSOR_Y) - y;
	box.bottom = box.top + 1;
	box.front = 0;
	box.back = 1;

	ID3D11Resource* texture = 0;
	(*PrePass::GetPositionIDSRV())->GetResource(&texture);

	Direct3D::DeviceContext()->CopySubresourceRegion(mStaging, 0, 0, 0, 0, texture, 0, &box);

	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	HRESULT hr = Direct3D::DeviceContext()->Map(mStaging, 0, D3D11_MAP_READ, 0, &mappedBuffer);

	if (SUCCEEDED(hr))
	{
		unsigned short* copy = (unsigned short*)mappedBuffer.pData;

		Vector3 pos;
		int id;

		pos.x = decode(*(copy + 0));
		pos.y = decode(*(copy + 1));
		pos.z = decode(*(copy + 2));
		id = (int)decode(*(copy + 3));

		if (id > 0)
		{
			mPickObject = ObjectManager::GetWithID<Object>(id);
			mPickPosition = pos;
		}
		else
		{
			mPickObject = 0;
			mPickPosition = Vector3::Zero();
		}

		Direct3D::DeviceContext()->Unmap(mStaging, 0);
	}

	SAFE_RELEASE(texture);
}

void Picking::SetPickObject(Object* obj)
{
	mPickObject = obj;
}

Object* Picking::GetPickObject()
{
	return mPickObject;
}

Vector3& Picking::GetPickPosition()
{
	return mPickPosition;
}