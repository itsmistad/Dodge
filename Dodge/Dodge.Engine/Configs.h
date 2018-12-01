#pragma once
#include "Headers.h"
#include "Audio.h"
#include "Gameplay.h"
#include "Keybinds.h"
#include "Video.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Configs
{
private:
	static json LoadJSON(string filePath);

	string audioConfigPath, videoConfigPath, gameplayConfigPath, keybindsConfigPath;
	Audio audioConfig;
	Video videoConfig;
	Gameplay gameplayConfig;
	Keybinds keybindsConfig;

	static Configs current;
public:
	Configs();
	static void Load();

	static string GetAudioPath();
	static string GetVideoPath();
	static string GetGameplayPath();
	static string GetKeybindsPath();

	static Audio GetAudio();
	static Video GetVideo();
	static Gameplay GetGameplay();
	static Keybinds GetKeybinds();
};