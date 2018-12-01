#include "Headers.h"
#include "Menu.h"

Menu * Menu::currentMenu = NULL;
map<string, Menu*> Menu::all;

Menu Menu::Add(string name, MouseController * mouseController)
{
	auto menu = new Menu(name, mouseController);
	all[name] = menu;
	return *menu;
}

Menu::Menu()
{
	name = "";
	background = NULL;
	isActive = false;
	this->mouseController = NULL;
}

Menu::Menu(string name, MouseController * mouseController)
{
	this->name = name;
	background = NULL;
	isActive = false;
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

void Menu::OnMouseRelease(Vector2 mousePosition) {}

void Menu::Show()
{
	isActive = true;
	currentMenu = this;
	mouseController->SetLock(true);
}

void Menu::Hide()
{
	isActive = false;
	currentMenu = NULL;
	mouseController->SetLock(false);
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
