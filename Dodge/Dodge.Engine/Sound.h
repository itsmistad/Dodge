#pragma once
#include "Headers.h"

enum SoundType
{
	Music, Effect, UI
};

class Sound
{
private:
	string name;
	wstring filePath;
	SoundType type;
	bool loop;
public:
	unique_ptr<SoundEffect> soundEffect;
	unique_ptr<SoundEffectInstance> soundEffectInstance;

	Sound();
	Sound(string name, wstring filePath, SoundType type, bool loop);
	void Play();
	void Stop();
	string GetName();
	bool DoesLoop();
	bool operator == (const Sound & other) const;
	bool operator != (const Sound & other) const;
};