#include "Headers.h"
#include "Audio.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Audio::Audio()
{
	this->masterVolume = 0.4f;
	this->effectsVolume = 1.f;
	this->musicVolume = 0.5f;
	this->uiVolume = 1.f;
}