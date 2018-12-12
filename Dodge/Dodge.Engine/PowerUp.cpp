#include "Headers.h"
#include "PowerUp.h"

PowerUp::PowerUp()
{
}

PowerUp::PowerUp(PowerUpType type)
{
	this->type = type;
}

void PowerUp::Update()
{
}

void PowerUp::Draw(Matrix viewMatrix, Matrix perspectiveMatrix)
{
	viewMatrix;
	perspectiveMatrix;
}

void PowerUp::Reset()
{
}

void PowerUp::Kill()
{
}
