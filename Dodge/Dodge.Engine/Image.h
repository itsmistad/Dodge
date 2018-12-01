#pragma once
#include "Headers.h"
#include "UIElement.h"

class Image : UIElement
{
private:
	DescriptorHeap * resourceDescriptors;
	UIDescriptors descriptor;
public:
	ComPtr<ID3D12Resource> texture;
	Vector2 position;
	bool stretch;
	Image();
	Image(ID3D12Device * device, ResourceUploadBatch * resourceUpload,
		DescriptorHeap * resourceDescriptors, UIDescriptors descriptor,
		wstring imageFilePath, Align align, Vector2 displacement, float scale, XMVECTORF32 color,
		bool isVisible = true);
	Image(ID3D12Device * device, ResourceUploadBatch * resourceUpload,
		DescriptorHeap * resourceDescriptors, UIDescriptors descriptor,
		wstring imageFilePath, Vector2 position, float scale, XMVECTORF32 color,
		bool isVisible = true, bool stretch = false);
	Vector2 Measure();
	Vector2 GetOrigin();
	float GetWidth();
	float GetHeight();
	Vector2 CalculatePosition();
	void Draw(SpriteBatch * spriteBatch);
	void Reset();
};