#pragma once
#include "Headers.h"

class Font
{
private:
	unique_ptr<SpriteFont> sprite;
public:
	static Font * Default;

	Font(unique_ptr<SpriteFont> sprite);
	void Draw(SpriteBatch * spriteBatch, wstring text, int fontSize, Vector2 position, XMVECTORF32 color);
	Vector2 Measure(wstring text);
	void Reset();
};