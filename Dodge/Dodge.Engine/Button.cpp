#include "Headers.h"
#include "Button.h"

Button::Button() 
{
	text = NULL;
	image = NULL;
}

Button::Button(ID3D12Device * device, ResourceUploadBatch * resourceUpload, DescriptorHeap * resourceDescriptors, UIDescriptors descriptor, wstring content, wstring buttonFilePath, Align align, Vector2 displacement, Font * font, float scale, int fontSize, XMVECTORF32 color, function<void(Button *)> callback, bool isVisible, bool hasOutline, bool hasDropShadow)
{
	this->callback = callback;
	this->align = align;
	this->scale = scale;
	this->color = color;
	this->isVisible = isVisible;
	if (!buttonFilePath.empty())
		image = new Image(device, resourceUpload, resourceDescriptors,
			descriptor, buttonFilePath, align, displacement, scale, Colors::White, isVisible);
	Vector2 adjust;
	if (!buttonFilePath.empty())
	{
		this->position = image->position;
		adjust = image->GetOrigin() * scale;
		Vector2 measure = font->Measure(content) * ((float)fontSize / 32.f) / 2.f;
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
	adjust += displacement;
	this->displacement = adjust;
	text = new Text(content, align, adjust, font, fontSize, color, isVisible, hasOutline, hasDropShadow);
}

Button::Button(ID3D12Device * device, ResourceUploadBatch * resourceUpload, DescriptorHeap * resourceDescriptors, UIDescriptors descriptor, wstring _content, wstring buttonFilePath, Vector2 _position, Font * font, float _scale, int fontSize, XMVECTORF32 _color, function<void(Button *)> _callback, bool _isVisible, bool _hasOutline, bool _hasDropShadow)
{
	callback = _callback;
	position = _position;
	align = None;
	scale = _scale;
	color = _color;
	isVisible = _isVisible;
	if (!buttonFilePath.empty())
		image = new Image(device, resourceUpload, resourceDescriptors,
			descriptor, buttonFilePath, position, scale, Colors::White, isVisible);
	position.y -= 2;
	text = new Text(_content, position, font, fontSize, color, isVisible, _hasOutline, _hasDropShadow);
}

void Button::SetText(wstring content, Font * font, int fontSize, XMVECTORF32 _color)
{
	color = _color;
	Vector2 adjust;
	if (align != None)
	{
		adjust = displacement;
		text = new Text(content, align, adjust, font, fontSize, color, isVisible, text->hasOutline, text->hasDropShadow);
	}
	else
		text = new Text(content, position, font, fontSize, color, isVisible, text->hasOutline, text->hasDropShadow);
}

float Button::GetWidth()
{
	return image->GetWidth();
}

float Button::GetHeight()
{
	return image->GetHeight();
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

Text * Button::GetText()
{
	return text;
}

void Button::Callback()
{
	callback(this);
}

void Button::Draw(SpriteBatch * spriteBatch)
{
	if (!isVisible) return;

	position = image->position;

	image->Draw(spriteBatch);
	text->Draw(spriteBatch);
}

void Button::Reset()
{
	image->Reset();
	text->Reset();
}
