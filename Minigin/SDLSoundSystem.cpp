#include "SDLSoundSystem.h"
#include <SDL_mixer.h>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <string>
#include "ThreadTask.h"
#include "ResourceManager.h"

class SDLSoundSystem::Impl {
public:
    Impl()
    {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            throw std::runtime_error("Failed to initialize SDL_mixer: " + std::string(Mix_GetError()));
        }
    }

    ~Impl()
    {
        m_TaskQueue.Stop(); // stop thread first

        for (auto& [id, chunk] : m_SoundMap)
        {
            Mix_FreeChunk(chunk);
        }
        Mix_CloseAudio();
    }

	//Lots of debugging to make sure soundsystem worked properly
    void Play(const sound_id id, const float volume)
    {
        m_TaskQueue.Enqueue([this, id, volume]()
            {
                std::cout << "Attempting to play sound with ID: " << id << std::endl;
                if (m_SoundMap.empty())
                {
                    std::cout << "m_SoundMap is empty!" << std::endl;
                }

                auto it = m_SoundMap.find(id);
                if (it == m_SoundMap.end())
                {
                    std::cout << "Sound ID " << id << " not found in m_SoundMap!" << std::endl;

                    for (const auto& [key, value] : m_SoundMap)
                    {
                        std::cout << "m_SoundMap contains ID: " << key << std::endl;
                    }
                    return;
                }

                std::cout << "Sound ID " << id << " found. Playing sound." << std::endl;
                Mix_VolumeChunk(it->second, static_cast<int>(volume * MIX_MAX_VOLUME));
                Mix_PlayChannel(-1, it->second, 0);
            });
    }

    void LoadSound(const std::string& filePath, const sound_id id)
    {
        m_TaskQueue.Enqueue([this, filePath, id]()
            {
                const auto& dataPath = dae::ResourceManager::GetInstance().m_dataPath;
                const auto fullPath = dataPath / filePath;

                Mix_Chunk* chunk = Mix_LoadWAV(fullPath.string().c_str());
                if (!chunk)
                {
                    std::cerr << "Failed to load sound: " << filePath << " - " << Mix_GetError() << std::endl;
                    return;
                }

                m_SoundMap.emplace(id, chunk);
            });
    }

	void SetMasterVolume(const float volume)
	{
		m_TaskQueue.Enqueue([volume]()
			{
				Mix_Volume(-1, static_cast<int>(volume * MIX_MAX_VOLUME));
			});
	}

private:
    std::unordered_map<sound_id, Mix_Chunk*> m_SoundMap;
    ThreadTask m_TaskQueue;
};

SDLSoundSystem::SDLSoundSystem() : m_pImpl(std::make_unique<Impl>())
{
}

SDLSoundSystem::~SDLSoundSystem()
{
}

void SDLSoundSystem::Play(const sound_id id, const float volume)
{
    m_pImpl->Play(id, volume);
}

void SDLSoundSystem::LoadSound(const std::string& filePath, const sound_id id)
{
    m_pImpl->LoadSound(filePath, id);
}

void SDLSoundSystem::SetMasterVolume(const float volume)
{
	m_pImpl->SetMasterVolume(volume);
}
