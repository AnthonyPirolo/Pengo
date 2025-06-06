#pragma once
#include "ISoundSystem.h"
#include <memory>

class SDLSoundSystem final : public ISoundSystem {
public:
    SDLSoundSystem();
    ~SDLSoundSystem();

    void Play(const sound_id id, const float volume) override;
    void LoadSound(const std::string& filePath, const sound_id id) override;
    void SetMasterVolume(float volume) override;

    void PlayMusic(const std::string& musicFile, float volume, bool loop = true) override;
    void StopMusic() override;
	void ToggleMute() override;

private:
    class Impl;
    std::unique_ptr<Impl> m_pImpl;
};
