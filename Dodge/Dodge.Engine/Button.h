#pragma once
#include "Headers.h"
#include "Image.h"
#include "Text.h"
#include "UIElement.h"
#include "MouseController.h"

class Button : UIElement
{
private:
	Image image;
	Text text;
	function<void()> callback;
public:
	Button();
	Button(ID3D12Device * device, ResourceUploadBatch * resourceUpload,
		DescriptorHeap * resourceDescriptors, UIDescriptors descriptor, wstring content,
		wstring buttonFilePath, Align align, Vector2 displacement, Font * font, 
		float scale, float fontSize, XMVECTORF32 color, function<void()> callback,
		bool isVisible = true, bool hasOutline = false, bool hasDropShadow = false);
	Button(ID3D12Device * device, ResourceUploadBatch * resourceUpload,
		DescriptorHeap * resourceDescriptors, UIDescriptors descriptor, wstring content,
		wstring buttonFilePath, Vector2 position, Font * font, float scale, float fontSize,
		XMVECTORF32 color, function<void()> callback,
		bool isVisible = true, bool hasOutline = false, bool hasDropShadow = false);
	float GetWidth();
	float GetHeight();
	bool IsInBounds(Vector2 position);
	void Callback();
	void Draw(SpriteBatch * spriteBatch);
	void Reset();
};