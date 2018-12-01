#include "Headers.h"
#include "Button.h"

Button::Button() {}

Button::Button(ID3D12Device * device, ResourceUploadBatch * resourceUpload, DescriptorHeap * resourceDescriptors, UIDescriptors descriptor, wstring content, wstring buttonFilePath, Align align, Vector2 displacement, Font * font, float scale, float fontSize, XMVECTORF32 color, function<void()> callback, bool isVisible, bool hasOutline, bool hasDropShadow)
{
	this->callback = callback;
	this->align = None;
	this->scale = scale;
	this->color = color;
	this->isVisible = isVisible;
	if (!buttonFilePath.empty())
		image = Image(device, resourceUpload, resourceDescriptors,
			descriptor, buttonFilePath, align, displacement, scale, Colors::White, isVisible);
	Vector2 adjust;
	if (!buttonFilePath.empty())
	{
		this->position = image.position;
		adjust = image.GetOrigin() * scale;
		Vector2 measure = font->Measure(content, fontSize) * (fontSize / 32.f) / 2.f;
		switch (align)
		{
		case Center:
			adjust = Vector2();
			break;
		case TopLeft:
			adjust = Vector2(adjust.x - measure.x + 2, adjust.y - measure.y + 2);
			break;
		case TopCenter:
			adjust = Vector2(0, adjust.y - measure.y + 2);
			break;
		case TopRight:
			adjust = Vector2(-adjust.x + measure.x + 2, adjust.y - measure.y + 2);
			break;
		case CenterRight:
			adjust = Vector2(-adjust.x + measure.x + 2, 0);
			break;
		case BottomRight:
			adjust = Vector2(-adjust.x + measure.x + 2, -adjust.y + measure.y + 2);
			break;
		case BottomCenter:
			adjust = Vector2(0, -adjust.y + measure.y + 2);
			break;
		case BottomLeft:
			adjust = Vector2(adjust.x - measure.x + 2, -adjust.y + measure.y + 2);
			break;
		case CenterLeft:
			adjust = Vector2(adjust.x - measure.x + 2, 0);
			break;
		}
		adjust.y -= 2;
	}
	//else
	//	this->position = 
	adjust += displacement;
	this->displacement = adjust;
	text = Text(content, align, adjust, font, fontSize, color, isVisible, hasOutline, hasDropShadow);
}

Button::Button(ID3D12Device * device, ResourceUploadBatch * resourceUpload, DescriptorHeap * resourceDescriptors, UIDescriptors descriptor, wstring content, wstring buttonFilePath, Vector2 position, Font * font, float scale, float fontSize, XMVECTORF32 color, function<void()> callback, bool isVisible, bool hasOutline, bool hasDropShadow)
{
	this->callback = callback;
	this->position = position;
	this->align = None;
	this->scale = scale;
	this->color = color;
	this->isVisible = isVisible;
	if (!buttonFilePath.empty())
		image = Image(device, resourceUpload, resourceDescriptors,
			descriptor, buttonFilePath, position, scale, Colors::White, isVisible);
	position.y -= 2;
	text = Text(content, position, font, fontSize, color, isVisible, hasOutline, hasDropShadow);
}

float Button::GetWidth()
{
	return image.GetWidth();
}

float Button::GetHeight()
{
	return image.GetHeight();
}

bool Button::IsInBounds(Vector2 point)
{
	auto width = GetWidth() / 2;
	auto height = GetHeight() / 2;
	return (point.x > position.x - width &&
		point.x < position.x + width &&
		point.y > position.y - height &&
		point.y < position.y + height);
}

void Button::Callback()
{
	callback();
}

void Button::Draw(SpriteBatch * spriteBatch)
{
	if (!isVisible) return;

	position = image.position;

	image.Draw(spriteBatch);
	text.Draw(spriteBatch);
}

void Button::Reset()
{
	image.Reset();
	text.Reset();
}
