#pragma once
#include "Headers.h"
#include "Font.h"
#include "UIElement.h"

class Text : UIElement
{
public:
	int size;
	Font * font;
	wstring content;
	bool hasOutline, hasDropShadow;
	using UIElement::displacement;
	using UIElement::position;

	Text();
	Text(wstring content, Align align, Vector2 displacement, 
		Font * font, int size, XMVECTORF32 color,
		bool isVisible = true, bool hasOutline = false, bool hasDropShadow = false);
	Text(wstring content, Vector2 position,
		Font * font, int size, XMVECTORF32 color,
		bool isVisible = true, bool hasOutline = false, bool hasDropShadow = false);
	Vector2 CalculatePosition();
	float GetWidth();
	float GetHeight();
	void Draw(SpriteBatch * spriteBatch);
	void Reset();
};