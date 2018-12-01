#pragma once
#include "Headers.h"
#include "Font.h"
#include "UIElement.h"

class Text : UIElement
{
private:
	Font * font;
public:
	int size;
	wstring content;
	bool hasOutline, hasDropShadow;

	Text();
	Text(wstring content, Align align, Vector2 displacement, 
		Font * font, int size, XMVECTORF32 color,
		bool isVisible = true, bool hasOutline = false, bool hasDropShadow = false);
	Text(wstring content, Vector2 position,
		Font * font, int size, XMVECTORF32 color,
		bool isVisible = true, bool hasOutline = false, bool hasDropShadow = false);
	float GetWidth();
	float GetHeight();
	void Draw(SpriteBatch * spriteBatch);
	void Reset();
};