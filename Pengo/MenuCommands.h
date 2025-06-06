#pragma once
#include "Command.h"
#include <memory>
#include "MainMenuState.h"

class MenuUpCommand : public Command
{
public:
    explicit MenuUpCommand(MainMenuState* menuState)
        : m_MenuState(menuState) {
    }

    virtual void Execute() override
    {
        if (m_MenuState)
            m_MenuState->MoveSelectionUp();
    };

private:
    MainMenuState* m_MenuState;
};

class MenuDownCommand : public Command
{
public:
    explicit MenuDownCommand(MainMenuState* menuState)
        : m_MenuState(menuState) {
    }

    virtual void Execute() override
    {
        if (m_MenuState)
            m_MenuState->MoveSelectionDown();
    };

private:
    MainMenuState* m_MenuState;
};

class MenuSelectCommand : public Command
{
public:
    explicit MenuSelectCommand(MainMenuState* menuState)
        : m_MenuState(menuState) {
    }

    virtual void Execute() override
    {
        if (m_MenuState)
            m_MenuState->ConfirmSelection();
    };

private:
    MainMenuState* m_MenuState;
};
