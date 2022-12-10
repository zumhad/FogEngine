#include "FrustumCulling.h"



using namespace DirectX;

FrustumCulling::Data* FrustumCulling::mData = 0;

struct FrustumCulling::Data
{
public:
	BoundingFrustum frustum;
};

void FrustumCulling::Update(Matrix view, Matrix proj)
{
	mData->frustum = BoundingFrustum(proj);
	mData->frustum.Transform(mData->frustum, XMMatrixInverse(0, view));
}

bool FrustumCulling::Intersect(const DirectX::BoundingBox& bb)
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