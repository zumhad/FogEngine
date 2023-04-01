#include "Picking.h"

#include "Direct3D.h"
#include "Application.h"
#include "Cursor.h"
#include "SelectMap.h"
#include "ObjectManager.h"

#include <DirectXCollision.h>
#include <SimpleMath.h>

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
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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

	D3D11_BOX box{};
	box.left = Cursor::GetPosition(CURSOR_X) - x;
	box.right = box.left + 1;
	box.top = Cursor::GetPosition(CURSOR_Y) - y;
	box.bottom = box.top + 1;
	box.front = 0;
	box.back = 1;

	ID3D11Resource* texture = 0;
	SelectMap::GetSRV()->GetResource(&texture);

	Direct3D::DeviceContext()->CopySubresourceRegion(mStaging, 0, 0, 0, 0, texture, 0, &box);

	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	HRESULT hr = Direct3D::DeviceContext()->Map(mStaging, 0, D3D11_MAP_READ, 0, &mappedBuffer);

	if (SUCCEEDED(hr))
	{
		float* copy = (float*)(mappedBuffer.pData);

		Vector3 pos;
		int id;

		pos.x = *copy++;
		pos.y = *copy++;
		pos.z = *copy++;
		id = int(*copy++);

		if (id >= 0)
		{
			mPickObject = &ObjectManager::Get(id);
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

Object* Picking::GetPickObject()
{
	return mPickObject;
}

Vector3& Picking::GetPickPosition()
{
	return mPickPosition;
}