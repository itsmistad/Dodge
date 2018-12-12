#pragma once
#include "Object.h"
#include "DeviceResources.h"
#include "Player.h"
#include "ObjectManager.h"

class Camera
{
private:
	DX::DeviceResources * deviceResources;
	float fov, goalFov, oldFov, goalFovTicks;
	Vector3 up, right, forward;
public:
	static Camera * Main;
	Vector3 gravitationalVelocity, afflictedVelocity, jumpVelocity, movementVelocity, position, oldPosition;
	Matrix viewMatrix, perspectiveMatrix;
	float pitch, yaw, oldYaw, oldPitch;
	bool canFreeCam;

	Camera();
	Camera(DX::DeviceResources * deviceResources);
	void Update(float elapsedTime, float deltaTime);
	void LookAt(Vector3 position, bool forceUpdateRot = false);
	void SetFOV(float fov);
	void SetGoalFOV(float value);
	float GetDefaultFOV();
	void TickFOVToGoal(float deltaTime);
	float GetFOV();
	Vector3 GetUp();
	Vector3 GetRight();
	Vector3 GetForward();
};