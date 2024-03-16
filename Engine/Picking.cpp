#include "Picking.h"

#include "Direct3D.h"
#include "Application.h"
#include "Cursor.h"
#include "ObjectManager.h"
#include "Vector3.h"
#include "MathHelper.h"
#include "PrePass.h"
#include "Camera.h"
#include "Utility.h"

#include <DirectXCollision.h>

ID3D11Texture2D* Picking::mStagingPosition = 0;
ID3D11Texture2D* Picking::mStagingID = 0;

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
	Direct3D::Device()->CreateTexture2D(&desc, 0, &mStagingPosition);

	desc.Format = DXGI_FORMAT_R32_UINT;
	Direct3D::Device()->CreateTexture2D(&desc, 0, &mStagingID);
}

void Picking::Shotdown()
{
	SAFE_RELEASE(mStagingPosition);
	SAFE_RELEASE(mStagingID);
}

Vector3 Picking::ReadPosition()
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
	PrePass::GetPositionMaterialSRV()[0]->GetResource(&texture);

	Direct3D::DeviceContext()->CopySubresourceRegion(mStagingPosition, 0, 0, 0, 0, texture, 0, &box);
	SAFE_RELEASE(texture);

	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	FOG_TRACE(Direct3D::DeviceContext()->Map(mStagingPosition, 0, D3D11_MAP_READ, 0, &mappedBuffer));
	Direct3D::DeviceContext()->Unmap(mStagingPosition, 0);

	unsigned short* copy = (unsigned short*)mappedBuffer.pData;

	Vector3 res = Camera::GetOffsetPosition();
	res.x += Math::F16ToF32(*(copy + 0));
	res.y += Math::F16ToF32(*(copy + 1));
	res.z += Math::F16ToF32(*(copy + 2));

	return res;
}

int Picking::ReadID()
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
	PrePass::GetIDSRV()[0]->GetResource(&texture);

	Direct3D::DeviceContext()->CopySubresourceRegion(mStagingID, 0, 0, 0, 0, texture, 0, &box);
	SAFE_RELEASE(texture);

	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	FOG_TRACE(Direct3D::DeviceContext()->Map(mStagingID, 0, D3D11_MAP_READ, 0, &mappedBuffer));
	Direct3D::DeviceContext()->Unmap(mStagingID, 0);

	return *(unsigned int*)mappedBuffer.pData;
}

void Picking::Pick()
{
	Vector3 pos = ReadPosition();
	int id = ReadID();

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