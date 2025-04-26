#include "SDLSoundSystem.h"
#include <SDL_mixer.h>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <string>
#include "ThreadTask.h"

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
        std::cout << "Loading sound from file: " << filePath << " with ID: " << id << std::endl;
        Mix_Chunk* chunk = Mix_LoadWAV(filePath.data());
        if (!chunk)
        {
            throw std::runtime_error("Failed to load sound: " + filePath + " - " + Mix_GetError());
        }
        m_SoundMap[id] = chunk;
        std::cout << "Sound loaded and added to m_SoundMap with ID: " << id << std::endl;

        for (const auto& [key, value] : m_SoundMap)
        {
            std::cout << "m_SoundMap contains ID: " << key << std::endl;
        }
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
