#pragma once

#include "Core.h"
#include "Object.h"


class FOG_API Cube : public Object
{
public:
	Cube()
	{
		mType = tCube;
		rCube = this;
	}

	void Load();
	void Set();

	int VertexCount() { return 36; }

    ~Cube()
    {
        SAFE_RELEASE(mIndexBuffer);
        SAFE_RELEASE(mVertexBuffer);
    }

	Vector3 mMaterial;

private:
	ID3D11Buffer* mVertexBuffer = 0;
	ID3D11Buffer* mIndexBuffer = 0;
};

