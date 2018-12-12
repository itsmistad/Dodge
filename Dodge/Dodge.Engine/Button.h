#pragma once
#include "Headers.h"
#include "Image.h"
#include "Text.h"
#include "UIElement.h"
#include "MouseController.h"

class Button : UIElement
{
private:
	Text * text;
	Image * image;
	function<void(Button *)> callback;
public:
	Button();
	Button(ID3D12Device * device, ResourceUploadBatch * resourceUpload,
		DescriptorHeap * resourceDescriptors, UIDescriptors descriptor, wstring content,
		wstring buttonFilePath, Align align, Vector2 displacement, Font * font, 
		float scale, int fontSize, XMVECTORF32 color, function<void(Button *)> callback,
		bool isVisible = true, bool hasOutline = false, bool hasDropShadow = false);
	Button(ID3D12Device * device, ResourceUploadBatch * resourceUpload,
		DescriptorHeap * resourceDescriptors, UIDescriptors descriptor, wstring content,
		wstring buttonFilePath, Vector2 position, Font * font, float scale, int fontSize,
		XMVECTORF32 color, function<void(Button *)> callback,
		bool isVisible = true, bool hasOutline = false, bool hasDropShadow = false);
	void SetText(wstring content, Font * font, int fontSize, XMVECTORF32 color);
	float GetWidth();
	float GetHeight();
	bool IsInBounds(Vector2 position);
	Text * GetText();
	void Callback();
	void Draw(SpriteBatch * spriteBatch);
	void Reset();
};