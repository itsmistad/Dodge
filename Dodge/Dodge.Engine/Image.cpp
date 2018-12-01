#include "Headers.h"
#include "Game.h"
#include "Image.h"

Image::Image() {}

Image::Image(ID3D12Device * device, ResourceUploadBatch * resourceUpload,
	DescriptorHeap * resourceDescriptors, UIDescriptors descriptor,
	wstring imageFilePath, Align align, Vector2 displacement, float scale, XMVECTORF32 color,
	bool isVisible)
{
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, *resourceUpload, imageFilePath.c_str(),
			texture.ReleaseAndGetAddressOf()));

	CreateShaderResourceView(device, texture.Get(),
		resourceDescriptors->GetCpuHandle(descriptor));
	this->resourceDescriptors = resourceDescriptors;
	this->descriptor = descriptor;
	this->align = align;
	this->displacement = displacement;
	this->color = color;
	this->scale = scale;
	this->isVisible = isVisible;
	this->origin = Measure();
	this->position = CalculatePosition();
	this->stretch = false;
}

Image::Image(ID3D12Device * device, ResourceUploadBatch * resourceUpload,
	DescriptorHeap * resourceDescriptors, UIDescriptors descriptor,
	wstring imageFilePath, Vector2 position, float scale, XMVECTORF32 color,
	bool isVisible, bool stretch)
{
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, *resourceUpload, imageFilePath.c_str(),
			texture.ReleaseAndGetAddressOf()));

	CreateShaderResourceView(device, texture.Get(),
		resourceDescriptors->GetCpuHandle(descriptor));
	this->resourceDescriptors = resourceDescriptors;
	this->descriptor = descriptor;
	this->position = position;
	this->align = None;
	this->color = color;
	this->scale = scale;
	this->isVisible = isVisible;
	this->origin = Measure();
	this->stretch = stretch;
}

Vector2 Image::Measure()
{
	Vector2 result;
	XMUINT2 size = GetTextureSize(texture.Get());
	result.x = (float)size.x / 2.f;
	result.y = (float)size.y / 2.f;

	return result;
}

Vector2 Image::GetOrigin()
{
	return origin;
}

float Image::GetWidth()
{
	return origin.x * 2 * scale;
}

float Image::GetHeight()
{
	return origin.y * 2 * scale;
}

Vector2 Image::CalculatePosition()
{
	Vector2 pos;
	if (align != None)
	{
		auto windowSize = Game::Current->GetCurrentSize();
		auto adjust = origin * scale;
		switch (align)
		{
		case Center:
			pos = Vector2(windowSize.x / 2, windowSize.y / 2);
			break;
		case TopLeft:
			pos = adjust;
			break;
		case TopCenter:
			pos = Vector2(windowSize.x / 2, adjust.y);
			break;
		case TopRight:
			pos = Vector2(windowSize.x - adjust.x, adjust.y);
			break;
		case CenterRight:
			pos = Vector2(windowSize.x - adjust.x, windowSize.y / 2);
			break;
		case BottomRight:
			pos = Vector2(windowSize.x - adjust.x, windowSize.y - adjust.y);
			break;
		case BottomCenter:
			pos = Vector2(windowSize.x / 2, windowSize.y - adjust.y);
			break;
		case BottomLeft:
			pos = Vector2(adjust.x, windowSize.y - adjust.y);
			break;
		case CenterLeft:
			pos = Vector2(adjust.x, windowSize.y / 2);
			break;
		}

		pos += displacement;
	}

	return pos;
}

void Image::Draw(SpriteBatch * spriteBatch)
{
	if (!isVisible) return;

	if (stretch)
	{
		auto x = Game::Current->GetCurrentSize().x;
		auto y = Game::Current->GetCurrentSize().y;
		spriteBatch->Draw(resourceDescriptors->GetGpuHandle(descriptor),
			GetTextureSize(texture.Get()),
			RECT{0, 0, (LONG)x, (LONG)y});
		return;
	}

	origin = Measure();
	position = CalculatePosition();

	spriteBatch->Draw(resourceDescriptors->GetGpuHandle(descriptor),
		GetTextureSize(texture.Get()),
		position, nullptr, color, 0.f, origin, scale);
}

void Image::Reset()
{
	texture.Reset();
}