#include "FrustumCulling.h"

#include "MathHelper.h"
#include "Quaternion.h"

using namespace DirectX;

FrustumCulling::Data* FrustumCulling::mData = 0;

struct FrustumCulling::Data
{
public:
	BoundingFrustum frustum;
	BoundingFrustum lightFrustum;
};

Matrix FrustumCulling::GetCascade()
{

	Vector3 corners[8];
	mData->frustum.GetCorners((XMFLOAT3*)corners);

	Vector3 center;
	for (int i = 0; i < 8; i++)
	{
		center += corners[i];
	}
	center /= 8.0f;

	Vector3 dir = Vector3(-1, -1, -1);
	Vector3 rot;
	rot.x = -Math::ASin(dir.y);
	rot.y = Math::ATan2(dir.x, dir.z);
	Quaternion q = XMQuaternionRotationRollPitchYawFromVector(rot);
	Vector3 up = XMVector3Rotate(XMVectorSet(0, 1, 0, 1), q);

	Matrix lightView = XMMatrixLookAtLH(center + dir, center, up);

	float xMin = 10000, xMax = -10000;
	float zMin = 10000, zMax = -10000;
	float yMin = 10000, yMax = -10000;

	for (int i = 0; i < 8; i++)
	{
		Vector3 trf = XMVector3Transform(corners[i], lightView);

		xMin = Math::Min(trf.x, xMin);
		yMin = Math::Min(trf.y, yMin);
		zMin = Math::Min(trf.z, zMin);
		xMax = Math::Max(trf.x, xMax);
		yMax = Math::Max(trf.y, yMax);
		zMax = Math::Max(trf.z, zMax);
	}

	Matrix lightProj = XMMatrixOrthographicOffCenterLH(xMin, xMax, yMin, yMax, zMin, zMax);

	return lightView * lightProj;
}

void FrustumCulling::Update(Matrix view, Matrix proj)
{
	mData->frustum = BoundingFrustum(proj);
	mData->frustum.Transform(mData->frustum, XMMatrixInverse(0, view));
}

bool FrustumCulling::Intersect(const BoundingBox& bb)
{
	return mData->frustum.Intersects(bb);
}

void FrustumCulling::Setup()
{
	mData = new Data;
}

void FrustumCulling::Shotdown()
{
	SAFE_DELETE(mData);
}