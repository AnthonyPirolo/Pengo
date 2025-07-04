#pragma once
#include <string>

using sound_id = int;

class ISoundSystem
{
public:
	virtual ~ISoundSystem() = default;
	virtual void Play(const sound_id id, const float volume) = 0;
	virtual void LoadSound(const std::string& filePath, const sound_id id) = 0;
	virtual void SetMasterVolume(float volume) = 0;

	virtual void PlayMusic(const std::string& musicFile, float volume, bool loop = true) = 0;
	virtual void StopMusic() = 0;

	virtual void ToggleMute() = 0;
};
