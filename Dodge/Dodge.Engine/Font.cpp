#include "Headers.h"
#include "Font.h"

Font * Font::Default = NULL;

Font::Font(unique_ptr<SpriteFont> sprite)
{
	this->sprite = move(sprite);
}

void Font::Draw(SpriteBatch * spriteBatch, wstring text, int fontSize, Vector2 position, XMVECTORF32 color)
{
	sprite->DrawString(spriteBatch, text.c_str(), position, color, 0,
		Measure(text) / 2.f, (float)fontSize / 32.f, SpriteEffects_None, 0);
}

Vector2 Font::Measure(wstring text)
{
	auto bounds = sprite->MeasureDrawBounds(text.c_str(), XMFLOAT2(0, 0));
	return Vector2(
		(float)bounds.right,
		(float)bounds.bottom);
}

void Font::Reset()
{
	sprite.reset();
}