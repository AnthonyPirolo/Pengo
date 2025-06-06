#include "InputManager.h"

void dae::InputManager::BindCommand(SDL_KeyCode key, KeyState state, std::shared_ptr<Command> command)
{
    m_KeyBindings[key] = { command, state };
}

bool dae::InputManager::ProcessInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            return false;
        }
    }

    const Uint8* keyState = SDL_GetKeyboardState(nullptr);

    for (auto& [key, binding] : m_KeyBindings)
    {
        bool isPressed = keyState[SDL_GetScancodeFromKey(key)];
        bool wasPressed = m_PreviousKeyStates[key];

        bool shouldExecute = false;

        switch (binding.state)
        {
        case KeyState::Pressed:
            shouldExecute = isPressed && !wasPressed;
            break;
        case KeyState::Released:
            shouldExecute = !isPressed && wasPressed;
            break;
        case KeyState::Held:
            shouldExecute = isPressed;
            break;
        }

        if (shouldExecute && binding.command)
        {
            binding.command->Execute();
        }

        m_PreviousKeyStates[key] = isPressed;
    }

    return true;
}
