#include "SDLSoundSystem.h"
#include <SDL_mixer.h>
#include <unordered_map>
#include <stdexcept>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

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
        for (auto& [id, chunk] : m_SoundMap) 
        {
            Mix_FreeChunk(chunk);
        }
        Mix_CloseAudio();
    }

    void Play(const sound_id id, const float volume) 
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_EventQueue.push([=]() 
            {
            auto it = m_SoundMap.find(id);
            if (it != m_SoundMap.end()) 
            {
                Mix_VolumeChunk(it->second, static_cast<int>(volume * MIX_MAX_VOLUME));
                Mix_PlayChannel(-1, it->second, 0);
            }
            });
        m_Condition.notify_one();
    }

    void LoadSound(const std::string& filePath, const sound_id id) 
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
        if (!chunk) 
        {
            throw std::runtime_error("Failed to load sound: " + filePath + " - " + Mix_GetError());
        }
        m_SoundMap[id] = chunk;
    }

    void ProcessEvents() 
    {
        while (m_Running) 
        {
            std::function<void()> event;
            {
                std::unique_lock<std::mutex> lock(m_Mutex);
                m_Condition.wait(lock, [this]() { return !m_EventQueue.empty() || !m_Running; });
                if (!m_Running) break;
                event = std::move(m_EventQueue.front());
                m_EventQueue.pop();
            }
            event();
        }
    }

    void Start() 
    {
        m_Running = true;
        m_Thread = std::thread(&Impl::ProcessEvents, this);
    }

    void Stop() 
    {
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Running = false;
        }
        m_Condition.notify_one();
        if (m_Thread.joinable()) 
        {
            m_Thread.join();
        }
    }

private:
    std::unordered_map<sound_id, Mix_Chunk*> m_SoundMap;
    std::queue<std::function<void()>> m_EventQueue;
    std::mutex m_Mutex;
    std::condition_variable m_Condition;
    std::thread m_Thread;
    bool m_Running{ false };
};

SDLSoundSystem::SDLSoundSystem() : m_pImpl(std::make_unique<Impl>()) 
{
    m_pImpl->Start();
}

SDLSoundSystem::~SDLSoundSystem() 
{
    m_pImpl->Stop();
}

void SDLSoundSystem::Play(const sound_id id, const float volume) 
{
    m_pImpl->Play(id, volume);
}

void SDLSoundSystem::LoadSound(const std::string& filePath, const sound_id id) 
{
    m_pImpl->LoadSound(filePath, id);
}
