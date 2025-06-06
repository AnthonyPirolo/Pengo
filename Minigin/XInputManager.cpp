#include "XInputManager.h"
#include <iostream>
#include <cstring>
#include <algorithm>

namespace dae
{
    class XInputManager::XInputManagerImpl
    {
    public:
        XInputManagerImpl()
            : controllerIndex(0), m_IsConnected(false), m_Initialized(false)
        {
            ZeroMemory(&currentState, sizeof(XINPUT_STATE));
            ZeroMemory(&previousState, sizeof(XINPUT_STATE));
        }

        void SetControllerIndex(int index)
        {
            controllerIndex = std::clamp(index, 0, 3);
        }

        void BindCommand(ButtonID button, ButtonState state, std::shared_ptr<Command> command)
        {
            m_ButtonBindings[button] = { command, state };
        }

        void UnbindCommand(ButtonID button)
        {
            m_ButtonBindings.erase(button);
        }

        bool ProcessInputFrame()
        {
            if (!m_Initialized)
            {
                m_Initialized = true;
                XINPUT_STATE tempState{};
                DWORD result = XInputGetState(controllerIndex, &tempState);
                m_IsConnected = (result == ERROR_SUCCESS);
                std::cout << "[XInputManager] (startup) Controller " << (m_IsConnected ? "connected" : "not detected")
                    << " (index " << controllerIndex << ")\n";
            }

            previousState = currentState;
            ZeroMemory(&currentState, sizeof(XINPUT_STATE));

            DWORD result = XInputGetState(controllerIndex, &currentState);
            if (result == ERROR_SUCCESS)
            {
                if (!m_IsConnected)
                {
                    m_IsConnected = true;
                    std::cout << "[XInputManager] Controller connected (index " << controllerIndex << ")\n";
                }

                WORD changed = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
                buttonsPressedThisFrame = changed & currentState.Gamepad.wButtons;
                buttonsReleasedThisFrame = changed & ~currentState.Gamepad.wButtons;

                for (const auto& [button, binding] : m_ButtonBindings)
                {
                    bool shouldExecute = false;
                    switch (binding.state)
                    {
                    case ButtonState::Pressed:
                        shouldExecute = IsDownThisFrame(button);
                        break;
                    case ButtonState::Released:
                        shouldExecute = IsUpThisFrame(button);
                        break;
                    case ButtonState::Held:
                        shouldExecute = IsPressed(button);
                        break;
                    }

                    if (shouldExecute && binding.command)
                        binding.command->Execute();
                }
            }
            else
            {
                if (m_IsConnected)
                {
                    m_IsConnected = false;
                    std::cout << "[XInputManager] Controller disconnected (index " << controllerIndex << ")\n";
                }
            }

            return true;
        }

    private:
        int controllerIndex;
        bool m_IsConnected;
        bool m_Initialized;

        XINPUT_STATE previousState{};
        XINPUT_STATE currentState{};
        WORD buttonsPressedThisFrame{};
        WORD buttonsReleasedThisFrame{};

        std::unordered_map<ButtonID, InputBinding> m_ButtonBindings;

        bool IsDownThisFrame(ButtonID button) const
        {
            return (buttonsPressedThisFrame & button) != 0;
        }

        bool IsUpThisFrame(ButtonID button) const
        {
            return (buttonsReleasedThisFrame & button) != 0;
        }

        bool IsPressed(ButtonID button) const
        {
            return (currentState.Gamepad.wButtons & button) != 0;
        }
    };

    XInputManager::XInputManager()
        : m_pImpl(new XInputManagerImpl())
    {
    }

    XInputManager::~XInputManager()
    {
        delete m_pImpl;
        m_pImpl = nullptr;
    }

    void XInputManager::SetControllerIndex(int index)
    {
        m_pImpl->SetControllerIndex(index);
    }

    void XInputManager::BindCommand(ButtonID button, ButtonState state, std::shared_ptr<Command> command)
    {
        m_pImpl->BindCommand(button, state, std::move(command));
    }

    void XInputManager::UnbindCommand(ButtonID button)
    {
        m_pImpl->UnbindCommand(button);
    }

    bool XInputManager::ProcessInput()
    {
        return m_pImpl->ProcessInputFrame();
    }
}
