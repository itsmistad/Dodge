#include "Headers.h"
#include "Configs.h"
#include "JSONConverters.hpp"
#include <Log.h>
using json = nlohmann::json;

Configs Configs::current;

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

void Configs::Load()
{
	auto temp = Configs::LoadJSON(current.audioConfigPath);
	if (temp.empty()) current.audioConfig = Audio();
	else current.audioConfig = temp;

	temp = Configs::LoadJSON(current.videoConfigPath);
	if (temp.empty()) current.videoConfig = Video();
	else current.videoConfig = temp;

	temp = Configs::LoadJSON(current.gameplayConfigPath);
	if (temp.empty()) current.gameplayConfig = Gameplay();
	else current.gameplayConfig = temp;

	temp = Configs::LoadJSON(current.keybindsConfigPath);
	if (temp.empty()) current.keybindsConfig = Keybinds();
	else current.keybindsConfig = temp;
}

string Configs::GetAudioPath()
{
	return current.audioConfigPath;
}

string Configs::GetVideoPath()
{
	return current.videoConfigPath;
}

string Configs::GetGameplayPath()
{
	return current.gameplayConfigPath;
}

string Configs::GetKeybindsPath()
{
	return current.keybindsConfigPath;
}

Audio Configs::GetAudio()
{
	return current.audioConfig;
}

Video Configs::GetVideo()
{
	return current.videoConfig;
}

Gameplay Configs::GetGameplay()
{
	return current.gameplayConfig;
}

Keybinds Configs::GetKeybinds()
{
	return current.keybindsConfig;
}