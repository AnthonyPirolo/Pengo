#pragma once
#include "ISoundSystem.h"
#include "ICollisionSystem.h"
#include "InputManager.h"
#include "XInputManager.h"
#include "IFileService.h"

namespace dae {
    class NullSoundSystem final : public ISoundSystem {
    public:
        virtual void Play(const sound_id, const float) override {}
        virtual void LoadSound(const std::string&, const sound_id) override {}
        virtual void SetMasterVolume(float) override {}
        virtual void PlayMusic(const std::string&, float, bool) override {}
        virtual void StopMusic() override {}
        virtual void ToggleMute() override {}
    };

    class NullCollisionSystem final : public ICollisionSystem {
    public:
        virtual void Register(CollisionComponent*) override {}
        virtual void Unregister(CollisionComponent*) override {}
        virtual void CheckAll() override {}
    };

    class NullInputManager final : public InputManager {
    public:
        virtual void BindCommand(SDL_KeyCode, KeyState, std::shared_ptr<Command>) override {}
        virtual bool ProcessInput() override { return true; }
        virtual void UnbindCommand(SDL_KeyCode) override {}
    };

    class NullXInputManager final : public XInputManager {
    public:
        virtual void SetControllerIndex(int) override {}
        virtual void BindCommand(ButtonID, ButtonState, std::shared_ptr<Command>) override {}
        virtual void UnbindCommand(ButtonID) override {}
        virtual bool ProcessInput() override { return true; }
    };

    class NullFileService final : public IFileService {
    public:
        std::string ReadFile(const std::filesystem::path&) const override
        {
            return {};
        }

        void WriteFile(const std::filesystem::path&, const std::string& ) const override
        {

        }

        void AppendToFile(const std::filesystem::path&, const std::string& ) const override
        {

        }
    };
}