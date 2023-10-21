#include "InputLayout.h"

#include "Direct3D.h"
#include "Utility.h"
#include "CustomArray.h"

InputLayout::InputLayout()
{
	mInputLayout = 0;
}

void InputLayout::Create(ID3D10Blob* blob, Array<String>& name)
{
	Array<D3D11_INPUT_ELEMENT_DESC> layout;

	int size = name.Size();
	for (int i = 0; i < size; i++)
	{
		D3D11_INPUT_ELEMENT_DESC desc;

		if (name[i] == L"POSITION")
		{
			desc = { name[i].GetCHAR(), 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			layout.Add(desc);
		}
		else if (name[i] == L"TEXCOORD")
		{
			desc = { name[i].GetCHAR(), 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			layout.Add(desc);
		}
		else if (name[i] == L"NORMAL")
		{
			desc = { name[i].GetCHAR(), 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			layout.Add(desc);
		}
		else if (name[i] == L"TEXPOS")
		{
			desc = { name[i].GetCHAR(), 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			layout.Add(desc);
		}
	}

	FOG_TRACE(Direct3D::Device()->CreateInputLayout(layout.Data(), layout.Size(), blob->GetBufferPointer(), blob->GetBufferSize(), &mInputLayout));
}

void InputLayout::Release()
{
	SAFE_RELEASE(mInputLayout);
}

ID3D11InputLayout* InputLayout::Get()
{
	return mInputLayout;
}