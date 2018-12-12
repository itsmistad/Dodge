#include "Headers.h"
#include "Throwable.h"
#include "SoundManager.h"
#include "Camera.h"

Throwable::Throwable()
{
}

Throwable::Throwable(DX::DeviceResources * deviceResources, wstring name, Vector3 position, Vector3 rotation, Vector3 velocity, float damage, float scale) :
	Object(deviceResources, L"Assets/Models/" + name + L".sdkmesh", position, rotation, scale)
{
	this->damage = damage;
	this->velocity = velocity;
}

void Throwable::Update(float deltaTime, vector<Player*> players)
{
	position += velocity * deltaTime;
	Object::Update();
	for (auto player : players)
		if (Vector3::Distance(GetPosition(), player->GetPosition() + Vector3(0, 5.6f, 0)) <= 5.2f)
		{
			Kill();
			player->Hurt(damage);
			SoundManager::GetCurrent()->Play("hit_" + to_string((int)((rand() % 4) + 1)));
		}
	auto distanceFromCamera = Vector3::Distance(GetPosition(), Camera::Main->position);
	if (distanceFromCamera > 750)
		Kill();
}

void Throwable::SetNormals(EffectPipelineStateDescription & psd, EffectPipelineStateDescription & psdAlpha)
{
	Object::SetNormals(psd, psdAlpha);
}

void Throwable::Draw(Matrix viewMatrix, Matrix perspectiveMatrix)
{
	Object::Draw(viewMatrix, perspectiveMatrix);
}

Vector3 Throwable::GetPosition()
{
	return Object::position;
}

void Throwable::SetPosition(Vector3 pos)
{
	Object::position = pos;
}

Vector3 Throwable::GetRotation()
{
	return Object::rotation;
}

void Throwable::SetRotation(Vector3 rot)
{
	Object::rotation = rot;
}

void Throwable::Reset()
{
	Object::Reset();
}

void Throwable::Kill()
{
	Object::Kill();
}
