#include "Headers.h"
#include "Game.h"
#include "Text.h"

Text::Text() {}

Text::Text(wstring content, Align align, Vector2 displacement, Font * font, int size, XMVECTORF32 color, bool isVisible, bool hasOutline, bool hasDropShadow)
{
	this->content = content;
	this->align = align;
	this->displacement = displacement;
	this->font = font;
	this->size = size;
	this->color = color;
	this->isVisible = isVisible;
	this->hasOutline = hasOutline;
	this->hasDropShadow = hasDropShadow;
	this->position = CalculatePosition();
}

Text::Text(wstring content, Vector2 position, Font * font, int size, XMVECTORF32 color, bool isVisible, bool hasOutline, bool hasDropShadow)
{
	this->content = content;
	this->position = position;
	this->align = None;
	this->font = font;
	this->size = size;
	this->color = color;
	this->isVisible = isVisible;
	this->hasOutline = hasOutline;
	this->hasDropShadow = hasDropShadow;
}

float Text::GetHeight()
{
	Vector2 dimensions = font->Measure(content) * ((float)size / 32.f);
	return dimensions.y * .8f;
}

float Text::GetWidth()
{
	Vector2 dimensions = font->Measure(content) * ((float)size / 32.f);
	return dimensions.x;
}

Vector2 Text::CalculatePosition()
{
	Vector2 pos;
	if (align != None)
	{
		auto windowSize = Game::Current->GetCurrentSize();
		Vector2 o = font->Measure(content) / 2;
		Vector2 adjust = o * ((float)size / 32.f);
		switch (align)
		{
		case Center:
			pos = Vector2(windowSize.x / 2, windowSize.y / 2);
			break;
		case TopLeft:
			pos = Vector2(adjust.x - 2, adjust.y - 2);
			break;
		case TopCenter:
			pos = Vector2(windowSize.x / 2, adjust.y - 2);
			break;
		case TopRight:
			pos = Vector2(windowSize.x - adjust.x - 2, adjust.y - 2);
			break;
		case CenterRight:
			pos = Vector2(windowSize.x - adjust.x - 2, windowSize.y / 2);
			break;
		case BottomRight:
			pos = Vector2(windowSize.x - adjust.x - 2, windowSize.y - adjust.y - 2);
			break;
		case BottomCenter:
			pos = Vector2(windowSize.x / 2, windowSize.y - adjust.y - 2);
			break;
		case BottomLeft:
			pos = Vector2(adjust.x - 2, windowSize.y - adjust.y - 2);
			break;
		case CenterLeft:
			pos = Vector2(adjust.x - 2, windowSize.y / 2);
			break;
		}

		pos += displacement;
	}
	return pos;
}

void Text::Draw(SpriteBatch * spriteBatch)
{
	if (!isVisible) return;

	position = CalculatePosition();

	if (hasDropShadow)
		font->Draw(spriteBatch, content, size, position - Vector2(2, -2), Colors::Black);
	else if (hasOutline)
	{
		font->Draw(spriteBatch, content, size, position + Vector2(1, 1), Colors::Black);
		font->Draw(spriteBatch, content, size, position + Vector2(-1, 1), Colors::Black);
		font->Draw(spriteBatch, content, size, position + Vector2(-1, -1), Colors::Black);
		font->Draw(spriteBatch, content, size, position + Vector2(1, -1), Colors::Black);
	}
	font->Draw(spriteBatch, content, size, position, color);
}

void Text::Reset() { }
