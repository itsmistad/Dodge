#pragma once
#include "Headers.h"
#include "Object.h"

class Player : Object
{
private:
	float health;
public:
	bool isWalking, isSprinting, isJumping, isGrounded;
	float totalTimeUngrounded;

	using Object::isDead;

	Player(DX::DeviceResources * deviceResources, Vector3 position, Vector3 rotation, float scale = 1.f);
	void Update(float deltaTime);
	void SetNormals(EffectPipelineStateDescription & psd, EffectPipelineStateDescription & psdAlpha);
	void Draw(Matrix viewMatrix, Matrix perspectiveMatrix);
	Vector3 GetPosition();
	void SetPosition(Vector3 pos);
	Vector3 GetRotation();
	void SetRotation(Vector3 rot);
	void Reset();
	void Hurt(float damage);
	void Kill();
};

