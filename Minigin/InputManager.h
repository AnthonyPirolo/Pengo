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
            std::shared_ptr<Command> command;
            KeyState state;
        };

        void BindCommand(SDL_KeyCode key, KeyState state, std::shared_ptr<Command> command);
        bool ProcessInput();
        void UnbindCommand(SDL_KeyCode key) {
			if (m_KeyBindings.find(key) == m_KeyBindings.end())
				return;
            m_KeyBindings.erase(key);
            m_PreviousKeyStates.erase(key);
        }

    private:
        std::unordered_map<SDL_KeyCode, InputBinding> m_KeyBindings;
        std::unordered_map<SDL_KeyCode, bool> m_PreviousKeyStates;
    };
}
