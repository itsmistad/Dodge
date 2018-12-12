#include "Headers.h"
#include "Object.h"

Object::Object()
{
	worldMatrix = Matrix::Identity;
	this->isDead = true;
	resourceFactory = nullptr;
	model = nullptr;
}

Object::Object(DX::DeviceResources * deviceResources, wstring modelFileName, Vector3 position, Vector3 rotation, float scale)
{
	this->deviceResources = deviceResources;
	this->position = position;
	this->rotation = rotation;
	worldMatrix = Matrix::Identity;
	this->isDead = false;
	this->scale = scale;
	this->model = Model::CreateFromSDKMESH(modelFileName.c_str(), deviceResources->GetD3DDevice());
	this->fxFactory = make_unique<EffectFactory>(deviceResources->GetD3DDevice());
}

void Object::SetNormals(EffectPipelineStateDescription & psd, EffectPipelineStateDescription & psdAlpha)
{
	normals = model->CreateEffects(*fxFactory, psd, psdAlpha);
}

void Object::Update()
{
	if (isDead) return;
	worldMatrix = Matrix::CreateFromYawPitchRoll(rotation.y, rotation.z, rotation.x) * Matrix::CreateScale(scale) * Matrix::CreateTranslation(position);
}

void Object::Draw(Matrix viewMatrix, Matrix perspectiveMatrix)
{
	if (isDead) return;
	Model::UpdateEffectMatrices(normals, worldMatrix, viewMatrix, perspectiveMatrix);
	model->Draw(deviceResources->GetCommandList(), normals.cbegin());
}

void Object::Reset()
{
	vector<shared_ptr<DirectX::IEffect>> normalsToCull(normals);
	if (states != NULL)
		states.reset();
	if (resourceFactory != NULL)
		resourceFactory.reset();
	if (fxFactory != NULL)
		fxFactory.reset();
	if (model != NULL)
		model.reset();
	for (auto normal : normalsToCull)
		normal.reset();
	normalsToCull.clear();
	normals.clear();
}

void Object::Kill()
{
	isDead = true;
}