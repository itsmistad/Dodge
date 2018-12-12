#pragma once
#include "Headers.h"
#include "Object.h"

enum PowerUpType {
	Speed,
	Jump,
	Throw,
	Cooldown
};

class PowerUp : Object
{
private:
	PowerUpType type;
public:
	using Object::isDead;

	PowerUp();
	PowerUp(PowerUpType type);

	void Update();
	void Draw(Matrix viewMatrix, Matrix perspectiveMatrix);
	void Reset();
	void Kill();
};

