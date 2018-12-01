#include "Headers.h"
#include "Audio.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Audio::Audio()
{
	this->masterVolume = 0.7f;
	this->effectsVolume = 1.f;
	this->musicVolume = 1.f;
	this->uiVolume = 1.f;
}