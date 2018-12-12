#include "Headers.h"
#include "Player.h"

Player::Player(DX::DeviceResources * deviceResources, Vector3 position, Vector3 rotation, float scale) :
	Object(deviceResources, L"Assets/Models/player.sdkmesh", position, rotation, scale)
{
	health = 100.f;
	totalTimeUngrounded = 0.f;
	isJumping = false;
	isWalking = false;
	isGrounded = true;
	isSprinting = false;
}

void Player::Reset()
{
	Object::Reset();
}

void Player::Hurt(float damage)
{
	health -= damage;
	if (health <= 0.f)
		Kill();
}

void Player::Kill()
{
	Object::Kill();
}

void Player::Update(float deltaTime)
{
	Object::Update();
	deltaTime;
}

void Player::SetNormals(EffectPipelineStateDescription & psd, EffectPipelineStateDescription & psdAlpha)
{
	Object::SetNormals(psd, psdAlpha);
}

void Player::Draw(Matrix viewMatrix, Matrix perspectiveMatrix)
{
	Object::Draw(viewMatrix, perspectiveMatrix);
}

Vector3 Player::GetPosition()
{
	return Object::position;
}

void Player::SetPosition(Vector3 pos)
{
	Object::position = pos;
}

Vector3 Player::GetRotation()
{
	return Object::rotation;
}

void Player::SetRotation(Vector3 rot)
{
	Object::rotation = rot;
}