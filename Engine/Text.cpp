#include "Text.h"

#include "Timer.h"
#include "GUI.h"
#include "Application.h"

#include <d2d1_3.h>
#include <dwrite.h>

using namespace DirectX;
using namespace D2D1;

Text::Data* Text::mData = 0;

struct Text::Data
{
public:
	Data();
	~Data();

public:
	IDWriteFactory* writeFactory;
	IDWriteTextFormat* textFormat;
};

Text::Data::Data() : writeFactory(0), textFormat(0)
{
	FOG_TRACE(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&writeFactory));

	writeFactory->CreateTextFormat(
		L"Impact", 0,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		24.0f, L"en-US", &textFormat);
}

Text::Data::~Data()
{
	SAFE_RELEASE(writeFactory);
	SAFE_RELEASE(textFormat);
}

Text::Text()
{
	text = L"";
	color = Color(0.0f, 0.0f, 0.0f);

	mBrush = 0;
}

Text::Text(Text& t)
{
	x = t.x;
	y = t.y;
	alignm = t.alignm;
	text = t.text;
	color = t.color;

	D2D1_COLOR_F c;
	c.r = color.r;
	c.g = color.g;
	c.b = color.b;
	c.a = color.a;

	GUI::RenderTarget()->CreateSolidColorBrush(c, &mBrush);
}

Text::~Text()
{
	SAFE_RELEASE(mBrush);
}

void Text::Setup()
{
	mData = new Data;
}

void Text::Shotdown()
{
	SAFE_DELETE(mData);
}

void Text::Draw()
{
	if (alignm.horizontal == ALIGNM_LEFT)
		mData->textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	if (alignm.horizontal == ALIGNM_RIGHT)
		mData->textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	if (alignm.horizontal == ALIGNM_CENTER_H)
		mData->textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	if (alignm.vertical == ALIGNM_TOP)
		mData->textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	if (alignm.vertical == ALIGNM_BOTTOM)
		mData->textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
	if (alignm.vertical == ALIGNM_CENTER_V)
		mData->textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	if (mParent)
	{
		mRect.left = mParent->mRect.left + x;
		mRect.top = mParent->mRect.top + y;
		mRect.right = mParent->mRect.right + x;
		mRect.bottom = mParent->mRect.bottom + y;
	}
	else
	{
		mRect.left = (float)x;
		mRect.top = (float)y;
		mRect.right = (float)Application::GetEditorWidth() + x;
		mRect.bottom = (float)Application::GetEditorHeight() + y;
	}

	GUI::RenderTarget()->DrawText(text, text.Length(), mData->textFormat, &mRect, mBrush);
}