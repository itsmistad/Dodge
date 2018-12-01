#pragma once
#include "Headers.h"
#include "Sound.h"

class SoundManager
{
private:
	static SoundManager * current;
	AudioEngine * audioEngine;
	vector<Sound*> sounds;
	Sound * emptySound;

public:
	SoundManager();
	SoundManager(AudioEngine * ae);
	static void Initialize(AudioEngine * ae);
	void Add(string soundName, wstring filePath, SoundType type, bool loop);
	void Play(string soundName);
	void StopLoop(string soundName);
	void SetLoopVolume(string soundName, float volume);
	void RestartLoops();
	void ResetLoops();
	float GetConfiguredVolume(SoundType type);
	Sound * Get(string soundName);
	Sound * GetEmptySound();
	static SoundManager * GetCurrent();
	AudioEngine * GetAudioEngine();
};