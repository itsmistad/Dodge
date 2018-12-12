#pragma once
#include "Headers.h"
#include "UIElement.h"
#include "MouseController.h"
#include "SoundManager.h"
#include "Button.h"

class Button;
class Text;
class Image;
class MouseController;

class Menu
{
private:
	Image * background;
	vector<Button*> buttons;
	vector<Text*> texts;
	vector<Image*> images;
	MouseController * mouseController;
	Menu(string name, MouseController * mouseController);
public:
	string name;
	bool isActive;
	Button * currentControlButton, * currentAudioButton;
	Keybind * currentKeybind;
	UIDescriptors currentAudio;
	float startingMouseY;
	static Menu * currentMenu;
	static map<string, Menu*> all;
	
	static Menu * Add(string name, MouseController * mouseController);

	Menu();
	void StartKeybindChange(Button * button, Keybind * keybind);
	void EndKeybindChange(int key);
	void StartVolumeChange(Button * button, UIDescriptors descriptor);
	void EndVolumeChange();
	void SetBackgroundImage(Image * image);
	void AddButton(Button * button);
	void AddText(Text * text);
	void AddImage(Image * image);
	void OnMousePress(Vector2 mousePosition);
	void OnMouseRelease(Vector2 mousePosition);
	void Show();
	void Hide();
	void Draw(SpriteBatch * spriteBatch);
	void Reset();
};