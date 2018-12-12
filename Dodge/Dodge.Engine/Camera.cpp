#include "Headers.h"
#include "Camera.h"
#include "SoundManager.h"

Camera * Camera::Main = NULL;

Camera::Camera() { }

Camera::Camera(DX::DeviceResources * _deviceResources)
{
	deviceResources = _deviceResources;
	position = Vector3();
	right = Vector3::Right;
	up = Vector3::Up;
	forward = Vector3::Forward;
	SetFOV(GetDefaultFOV());
	SetGoalFOV(fov);
	goalFovTicks = 0;
	LookAt(Vector3(1, 0, 1), true);
	canFreeCam = false;
}

void Camera::Update(float elapsedTime, float deltaTime)
{
	auto you = ObjectManager::You;
	auto rotationMatrix = Matrix::CreateFromYawPitchRoll(yaw, pitch, 0);
	auto targetVector = Vector3::TransformNormal(Vector3::Forward, rotationMatrix);
	auto predictedPosition = Vector3();
	if (canFreeCam)
	{
		right = Vector3::Transform(Vector3::Right, rotationMatrix);
		forward = Vector3::Transform(Vector3::Forward, rotationMatrix);
		up = Vector3::Up;
		predictedPosition = position + (movementVelocity * deltaTime);
	}
	else
	{
		if (!you->isGrounded)
		{
			you->totalTimeUngrounded += elapsedTime;
			gravitationalVelocity.y = -Configs::GetGameplay().gravity * you->totalTimeUngrounded * you->totalTimeUngrounded * deltaTime;
		}

		if (afflictedVelocity != Vector3::Zero)
			afflictedVelocity = Vector3::Lerp(afflictedVelocity, Vector3::Zero, 0.05f);

		predictedPosition = position + (gravitationalVelocity + (jumpVelocity + movementVelocity + afflictedVelocity) * deltaTime);
		if (predictedPosition.y < 5.6f && !you->isGrounded)
		{
			you->isGrounded = true;
			gravitationalVelocity.y = 0.f;
			you->totalTimeUngrounded = 0.f;
			if (you->isJumping)
			{
				jumpVelocity.y = 0.f;
				you->isJumping = false;
			}
			predictedPosition.y = 5.61f;
			SoundManager::GetCurrent()->Play("land");
		}

		if (predictedPosition.x < -203.f)
		{
			predictedPosition.x = -203.f;
			if (!you->isGrounded)
			{
				you->totalTimeUngrounded = 0.f;
				gravitationalVelocity.y = 0.f;
			}
			afflictedVelocity = Vector3(10.f, 0, 0);
			SoundManager::GetCurrent()->Play("jump");
		}
		if (predictedPosition.z < -203.f)
		{
			predictedPosition.z = -203.f;
			if (!you->isGrounded)
			{
				you->totalTimeUngrounded = 0.f;
				gravitationalVelocity.y = 0.f;
			}
			afflictedVelocity = Vector3(0, 0, 10.f);
			SoundManager::GetCurrent()->Play("jump");
		}
		if (predictedPosition.x > 202.5f)
		{
			predictedPosition.x = 202.5f;
			if (!you->isGrounded)
			{
				you->totalTimeUngrounded = 0.f;
				gravitationalVelocity.y = 0.f;
			}
			afflictedVelocity = Vector3(-10.f, 0, 0);
			SoundManager::GetCurrent()->Play("jump");
		}
		if (predictedPosition.z > 203.f)
		{
			predictedPosition.z = 203.f;
			if (!you->isGrounded)
			{
				you->totalTimeUngrounded = 0.f;
				gravitationalVelocity.y = 0.f;
			}
			afflictedVelocity = Vector3(0, 0, -10.f);
			SoundManager::GetCurrent()->Play("jump");
		}

		auto tempMatrix = Matrix::CreateRotationY(yaw);
		right = Vector3::Transform(Vector3::Right, tempMatrix);
		forward = Vector3::Transform(Vector3::Forward, tempMatrix);
		up = Vector3::Transform(Vector3::Up, tempMatrix);
	}

	position = predictedPosition;
	LookAt(position + targetVector);

	if (fov != goalFov)
		TickFOVToGoal(deltaTime);
}

void Camera::LookAt(Vector3 pos, bool forceUpdateRot)
{
	viewMatrix = Matrix::CreateLookAt(position, pos, up);
	if (forceUpdateRot)
	{
		pitch = -asinf(viewMatrix._32);
		yaw = acosf(viewMatrix._33 / cosf(pitch));
		auto tempMatrix = Matrix::CreateRotationY(yaw);
		right = Vector3::Transform(Vector3::Right, tempMatrix);
		forward = Vector3::Transform(Vector3::Forward, tempMatrix);
		up = Vector3::Transform(Vector3::Up, tempMatrix);
	}
}

void Camera::SetFOV(float value)
{
	this->fov = value;
	auto size = deviceResources->GetOutputSize();
	perspectiveMatrix = XMMatrixPerspectiveFovRH(
		value * XM_PI / 180.f,
		(float)size.right / (float)size.bottom,
		.1f,
		10000.f);
}

void Camera::SetGoalFOV(float value)
{
	goalFovTicks = 0;
	oldFov = fov;
	goalFov = value;
}

float Camera::GetDefaultFOV()
{
	return 80.f;
}

void Camera::TickFOVToGoal(float deltaTime)
{
	auto end = 10.f;
	if (goalFovTicks >= end)
		SetFOV(goalFov);
	else
	{
		goalFovTicks += deltaTime;
		SetFOV((1 - (goalFovTicks / end)) * oldFov + (goalFovTicks / end) * goalFov);
	}
}

float Camera::GetFOV()
{
	return fov;
}

Vector3 Camera::GetUp()
{
	return up;
}

Vector3 Camera::GetRight()
{
	return right;
}

Vector3 Camera::GetForward()
{
	return forward;
}
