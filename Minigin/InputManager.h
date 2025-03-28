#pragma once
#include "Singleton.h"
#include "Command.h"
#include <SDL.h>
#include <unordered_map>
#include <memory>

namespace dae
{
    class InputManager final : public Singleton<InputManager>
    {
    public:
        enum class KeyState
        {
            Pressed,
            Released,
            Held
        };

        struct InputBinding
        {
            std::shared_ptr<Command> command; // Use shared_ptr instead of unique_ptr
            KeyState state;
        };

        void BindCommand(SDL_KeyCode key, KeyState state, std::shared_ptr<Command> command); // Update to shared_ptr
        bool ProcessInput();

    private:
        std::unordered_map<SDL_KeyCode, InputBinding> m_KeyBindings;
        std::unordered_map<SDL_KeyCode, bool> m_PreviousKeyStates;
    };
}
