#include "InputLayout.h"

#include "Direct3D.h"
#include "Utility.h"
#include "CustomArray.h"

InputLayout::InputLayout()
{
	mInputLayout = 0;
}

void InputLayout::Add(const String& name, int index)
{
	FOG_ASSERT(index >= 0);

	D3D11_INPUT_ELEMENT_DESC desc{};

	if (name == L"POSITION")
	{
		desc = { "POSITION", (unsigned int)index, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		mArr.Add(desc);
	}
	else if (name == L"TEXCOORD")
	{
		desc = { "TEXCOORD", (unsigned int)index, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		mArr.Add(desc);
	}
	else if (name == L"NORMAL")
	{
		desc = { "NORMAL", (unsigned int)index, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		mArr.Add(desc);
	}
}

void InputLayout::Create(ID3D10Blob* blob)
{
	FOG_TRACE(Direct3D::Device()->CreateInputLayout(mArr.Data(), mArr.Size(), blob->GetBufferPointer(), blob->GetBufferSize(), &mInputLayout));
}

void InputLayout::Release()
{
	SAFE_RELEASE(mInputLayout);
}

ID3D11InputLayout* InputLayout::Get()
{
	return mInputLayout;
}