#pragma once

#include <memory>
#include <unordered_map>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <Xinput.h>

#include "Command.h"
#include "Singleton.h"

#pragma comment(lib, "xinput.lib")

namespace dae
{
    class XInputManager final : public Singleton<XInputManager>
    {
    public:
        using ButtonID = WORD;

        enum class ButtonState
        {
            Pressed,
            Released,
            Held
        };

        struct InputBinding
        {
            std::shared_ptr<Command> command;
            ButtonState state;
        };

        void SetControllerIndex(int index);

        void BindCommand(ButtonID button, ButtonState state, std::shared_ptr<Command> command);

        void UnbindCommand(ButtonID button);

        bool ProcessInput();

    private:
        XInputManager();
        ~XInputManager();

        friend class Singleton<XInputManager>;

        class XInputManagerImpl;
        XInputManagerImpl* m_pImpl;
    };
}
