#include "Headers.h"
#include "SoundManager.h"

SoundManager * SoundManager::current = NULL;

SoundManager::SoundManager()
{
}

SoundManager::SoundManager(AudioEngine * ae)
{
	audioEngine = ae;
	emptySound = new Sound(string(), wstring(), Music, false);
}

void SoundManager::Initialize(AudioEngine * ae)
{
	current = new SoundManager(ae);
}

void SoundManager::Add(string soundName, wstring filePath, SoundType type, bool loop)
{
	Sound * sound = new Sound(soundName, filePath, type, loop);
	sound->soundEffect = make_unique<SoundEffect>(audioEngine, filePath.c_str());
	if (loop)
		sound->soundEffectInstance = sound->soundEffect->CreateInstance();
	sounds.push_back(sound);
}

void SoundManager::Play(string soundName)
{
	for (auto & sound : sounds)
	{
		if (sound->GetName() == soundName)
		{
			sound->Play();
			break;
		}
	}
}

void SoundManager::StopLoop(string soundName)
{
	auto sound = Get(soundName);
	if (sound != emptySound && sound->DoesLoop())
		sound->Stop();
}

void SoundManager::SetLoopVolume(string soundName, float volume)
{
	auto sound = Get(soundName);
	if (sound != emptySound && sound->DoesLoop())
		sound->soundEffectInstance->SetVolume(volume);
}

void SoundManager::RestartLoops()
{
	for (auto & sound : sounds)
		if (sound->DoesLoop())
			sound->Play();
}

void SoundManager::ResetLoops()
{
	for (auto & sound : sounds)
		if (sound->DoesLoop())
			sound->soundEffectInstance.reset();
}

float SoundManager::GetConfiguredVolume(SoundType type)
{
	auto audio = Configs::GetAudio();
	return
		type == Music ? audio.musicVolume * audio.masterVolume :
		type == Effect ? audio.effectsVolume * audio.masterVolume :
		type == UI ? audio.uiVolume * audio.masterVolume : audio.masterVolume;
}

Sound * SoundManager::Get(string soundName)
{
	for (auto sound : sounds)
		if (sound->GetName() == soundName)
			return sound;
	return emptySound;
}

Sound * SoundManager::GetEmptySound()
{
	return emptySound;
}

SoundManager * SoundManager::GetCurrent()
{
	return current;
}

AudioEngine * SoundManager::GetAudioEngine()
{
	return audioEngine;
}