#include "Headers.h"
#include "Sound.h"

Sound::Sound()
{
}

Sound::Sound(string name, wstring filePath, SoundType type, bool loop)
{
	this->name = name;
	this->filePath = filePath;
	this->type = type;
	this->loop = loop;
	soundEffect = NULL;
	soundEffectInstance = NULL;
}

void Sound::Play()
{
	if (loop)
		soundEffectInstance->Play(true);
	else
	{
		auto audio = Configs::GetAudio();
		soundEffect->Play(
			type == Music ? audio.musicVolume * audio.masterVolume :
			type == Effect ? audio.effectsVolume * audio.masterVolume :
			type == UI ? audio.uiVolume * audio.masterVolume :
			audio.masterVolume, 0, 0);
	}
}

void Sound::Stop()
{
	soundEffectInstance->Stop();
}

string Sound::GetName()
{
	return name;
}

bool Sound::DoesLoop()
{
	return loop;
}

bool Sound::operator == (const Sound & other) const
{
	return
		this->name == other.name &&
		this->filePath == other.filePath &&
		this->type == other.type &&
		this->loop == other.loop &&
		this->soundEffect.get() == other.soundEffect.get() &&
		this->soundEffectInstance.get() == other.soundEffectInstance.get();
}

bool Sound::operator != (const Sound & other) const
{
	return !(*this == other);
}