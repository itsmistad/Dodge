#include "Headers.h"
#include "Configs.h"
#include "JSONConverters.hpp"
#include <Log.h>
using json = nlohmann::json;

Configs * Configs::Current = NULL;

Configs::Configs()
{
	this->audioConfigPath = "Config/Audio.json";
	this->videoConfigPath = "Config/Video.json";
	this->gameplayConfigPath = "Config/Gameplay.json";
	this->keybindsConfigPath = "Config/Keybinds.json";
}

json Configs::LoadJSON(string filePath)
{
	json j;
	ifstream in(filePath);
	if (in.is_open())
		in >> j;
	else Log::Error("Failed to load config file (%s).", filePath.c_str());
	return j;
}

bool Configs::SaveJSON(string filePath, json j)
{
	ofstream out(filePath);
	if (out.is_open() && !j.empty())
	{
		out << j;
		return true;
	}
	else
		Log::Error("Failed to load config file (%s).", filePath.c_str());
	return false;
}

void Configs::Load()
{
	Current = new Configs();

	auto temp = Configs::LoadJSON(Current->audioConfigPath);
	if (temp.empty()) Current->audioConfig = Audio();
	else Current->audioConfig = temp;

	temp = Configs::LoadJSON(Current->videoConfigPath);
	if (temp.empty()) Current->videoConfig = Video();
	else Current->videoConfig = temp;

	temp = Configs::LoadJSON(Current->gameplayConfigPath);
	if (temp.empty()) Current->gameplayConfig = Gameplay();
	else Current->gameplayConfig = temp;

	temp = Configs::LoadJSON(Current->keybindsConfigPath);
	if (temp.empty()) Current->keybindsConfig = Keybinds();
	else Current->keybindsConfig = temp;
}

void Configs::Save()
{
	Configs::SaveJSON(Current->audioConfigPath, Current->audioConfig);
	Configs::SaveJSON(Current->videoConfigPath, Current->videoConfig);
	Configs::SaveJSON(Current->gameplayConfigPath, Current->gameplayConfig);
	Configs::SaveJSON(Current->keybindsConfigPath, Current->keybindsConfig);
}

string Configs::GetAudioPath()
{
	return Current->audioConfigPath;
}

string Configs::GetVideoPath()
{
	return Current->videoConfigPath;
}

string Configs::GetGameplayPath()
{
	return Current->gameplayConfigPath;
}

string Configs::GetKeybindsPath()
{
	return Current->keybindsConfigPath;
}

Audio Configs::GetAudio()
{
	return Current->audioConfig;
}

Video Configs::GetVideo()
{
	return Current->videoConfig;
}

Gameplay Configs::GetGameplay()
{
	return Current->gameplayConfig;
}

Keybinds Configs::GetKeybinds()
{
	return Current->keybindsConfig;
}