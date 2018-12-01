#pragma once
#include "Headers.h"

class UIElement
{
public:
	Vector2 position, displacement, origin;
	Align align;
	XMVECTORF32 color;
	float scale;
	bool isVisible;

	virtual void Draw(SpriteBatch * spriteBatch);
	virtual void Reset();
};