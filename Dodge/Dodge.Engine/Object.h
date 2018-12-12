#pragma once
#include "Headers.h"
#include "DeviceResources.h"

class Object
{
protected:
	bool isDead;
	Matrix worldMatrix;
	shared_ptr<EffectTextureFactory> resourceFactory;
	unique_ptr<CommonStates> states;
	unique_ptr<EffectFactory> fxFactory;
	shared_ptr<Model> model;
	vector<shared_ptr<IEffect>> normals;
	DX::DeviceResources * deviceResources;
public:
	Object();
	Object(DX::DeviceResources * deviceResources, wstring modelFileName, Vector3 position, Vector3 rotation, float scale);
	virtual void SetNormals(EffectPipelineStateDescription & psd, EffectPipelineStateDescription & psdAlpha);
	virtual void Update();
	virtual void Draw(Matrix viewMatrix, Matrix perspectiveMatrix);
	virtual void Reset();
	virtual void Kill();

	Vector3 rotation, position, velocity, acceleration;
	float scale;
};