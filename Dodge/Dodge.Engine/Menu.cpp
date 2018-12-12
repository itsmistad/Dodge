#include "Headers.h"
#include "Menu.h"

Menu * Menu::currentMenu = NULL;
map<string, Menu*> Menu::all;

Menu * Menu::Add(string name, MouseController * mouseController)
{
	auto menu = new Menu(name, mouseController);
	all[name] = menu;
	return menu;
}

Menu::Menu()
{
	name = "";
	background = NULL;
	isActive = false;
	mouseController = NULL;
	currentAudioButton = currentControlButton = NULL;
	currentKeybind = NULL;
}

void Menu::StartKeybindChange(Button * button, Keybind * keybind)
{
	if (name == "controls" && currentControlButton == NULL && currentKeybind == NULL)
	{
		Menu::currentMenu->currentControlButton = button;
		Menu::currentMenu->currentKeybind = keybind;
		button->SetText(L"?", button->GetText()->font, button->GetText()->size, Colors::CornflowerBlue);
	}
}

void Menu::EndKeybindChange(int key)
{
	if (name == "controls")
	{
		auto button = Menu::currentMenu->currentControlButton;
		auto text = button->GetText();
		auto keybind = Menu::currentMenu->currentKeybind;
		keybind->SetKey(key);
		button->SetText(keybind->GetName(), text->font, text->size, Colors::White);
		Configs::Save();
		Menu::currentMenu->currentControlButton = NULL;
		Menu::currentMenu->currentKeybind = NULL;
	}
}

void Menu::StartVolumeChange(Button * button, UIDescriptors descriptor)
{
	switch (descriptor)
	{
	case MasterAudioButton:
	case MusicAudioButton:
	case SFXAudioButton:
	case UIAudioButton:
		Menu::currentMenu->currentAudioButton = button;
		Menu::currentMenu->currentAudio = descriptor;
		break;
	}
}

void Menu::EndVolumeChange()
{
	Configs::Save();
	Menu::currentMenu->currentAudioButton = NULL;
	Menu::currentMenu->currentAudio = Count;
}

Menu::Menu(string name, MouseController * mouseController)
{
	this->name = name;
	background = NULL;
	isActive = false;
	currentAudioButton = currentControlButton = NULL;
	currentKeybind = NULL;
	this->mouseController = mouseController;
}

void Menu::SetBackgroundImage(Image * image)
{
	background = image;
}

void Menu::AddButton(Button * button)
{
	buttons.push_back(button);
}

void Menu::AddText(Text * text)
{
	texts.push_back(text);
}

void Menu::AddImage(Image * image)
{
	images.push_back(image);
}

void Menu::OnMousePress(Vector2 mousePosition)
{
	for (auto button : buttons)
	{
		if (button->IsInBounds(mousePosition))
		{
			SoundManager::GetCurrent()->Play("ui_click");
			button->Callback();
			break;
		}
	}
}

void Menu::OnMouseRelease(Vector2 mousePosition)
{
	if (Menu::currentMenu->currentAudioButton != NULL)
		Menu::currentMenu->EndVolumeChange();
	mousePosition;
}

void Menu::Show()
{
	isActive = true;
	Menu::currentMenu = this;
}

void Menu::Hide()
{
	isActive = false;
	Menu::currentMenu = NULL;
}

void Menu::Draw(SpriteBatch * spriteBatch)
{
	if (!isActive) return;

	if (background != NULL)
		background->Draw(spriteBatch);
	for (auto button : buttons)
		button->Draw(spriteBatch);
	for (auto image : images)
		image->Draw(spriteBatch);
	for (auto text : texts)
		text->Draw(spriteBatch);
}

void Menu::Reset()
{
	if (background != NULL)
		background->Reset();
	for (auto button : buttons)
		button->Reset();
	for (auto image : images)
		image->Reset();
}
