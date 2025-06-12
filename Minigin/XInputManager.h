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
    class XInputManager
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

        virtual void SetControllerIndex(int index);

        virtual void BindCommand(ButtonID button, ButtonState state, std::shared_ptr<Command> command);

        virtual void UnbindCommand(ButtonID button);

        virtual bool ProcessInput();

        XInputManager();
        ~XInputManager();
    protected:

        class XInputManagerImpl;
        XInputManagerImpl* m_pImpl;
    };
}
