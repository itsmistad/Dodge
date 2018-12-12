#pragma once
#include "Headers.h"
#include "Object.h"
#include "Player.h"

class Throwable : Object
{
private:
	float damage, totalTimeUnhanded;
	Vector3 velocity, gravitationalVelocity;
public:
	using Object::isDead;

	Throwable();

	Throwable(DX::DeviceResources * deviceResources, wstring name, Vector3 position, Vector3 rotation, Vector3 velocity, float damage, float scale = 1.f);
	void Update(float deltaTime, vector<Player*> players);
	void SetNormals(EffectPipelineStateDescription & psd, EffectPipelineStateDescription & psdAlpha);
	void Draw(Matrix viewMatrix, Matrix perspectiveMatrix);
	Vector3 GetPosition();
	void SetPosition(Vector3 pos);
	Vector3 GetRotation();
	void SetRotation(Vector3 rot);
	void Reset();
	void Kill();
};

