#include "MainMenuState.h"
#include "MenuCommands.h"
#include "InputManager.h"
#include "XInputManager.h"
#include "GameStateManager.h"
#include "SinglePlayerState.h"
//#include "CoOpState.h"
//#include "VersusState.h"
//#include "HighscoreState.h"
#include <SDL.h>
#include <Xinput.h>
#include <iostream>
#include "GameTime.h"

MainMenuState::MainMenuState(dae::Scene* scene)
    : m_Scene(scene)
    , m_SelectedIndex(0)
{
}

void MainMenuState::OnEnter()
{
    m_Scene->RemoveAll();

    std::vector<std::string> texts = {
        "Single Player",
        "Co-Op (2 Players)",
        "Versus (Pengo vs SnoBee)",
        "Highscores",
        "Quit"
    };

    int startY = 100;
    for (int i = 0; i < (int)texts.size(); ++i) {
        auto go = std::make_shared<dae::GameObject>();
        //auto txt = go->AddComponent<dae::TextComponent>(go.get(), texts[i], );
        go->SetLocalPosition(glm::vec3(100, startY + i * 30, 0));
        m_Scene->Add(go);
        m_MenuItems.push_back(go);
    }

    //m_MenuItems[0]->GetComponent<dae::TextComponent>()->SetColor({ 255,255,0 });

    m_CmdUp = std::make_shared<MenuUpCommand>(this);
    m_CmdDown = std::make_shared<MenuDownCommand>(this);
    m_CmdSelect = std::make_shared<MenuSelectCommand>(this);

    dae::InputManager::GetInstance().BindCommand(SDLK_UP, dae::InputManager::KeyState::Pressed, m_CmdUp);
    dae::InputManager::GetInstance().BindCommand(SDLK_DOWN, dae::InputManager::KeyState::Pressed, m_CmdDown);
    dae::InputManager::GetInstance().BindCommand(SDLK_RETURN, dae::InputManager::KeyState::Pressed, m_CmdSelect);

    dae::XInputManager::GetInstance().BindCommand(XINPUT_GAMEPAD_DPAD_UP, dae::XInputManager::ButtonState::Pressed, m_CmdUp);
    dae::XInputManager::GetInstance().BindCommand(XINPUT_GAMEPAD_DPAD_DOWN, dae::XInputManager::ButtonState::Pressed, m_CmdDown);
    dae::XInputManager::GetInstance().BindCommand(XINPUT_GAMEPAD_A, dae::XInputManager::ButtonState::Pressed, m_CmdSelect);

    // dae::XInputManager::GetInstance().SetControllerIndex(1);

}

void MainMenuState::OnExit()
{
    dae::InputManager::GetInstance().UnbindCommand(SDLK_UP);
    dae::InputManager::GetInstance().UnbindCommand(SDLK_DOWN);
    dae::InputManager::GetInstance().UnbindCommand(SDLK_RETURN);

    dae::XInputManager::GetInstance().UnbindCommand(XINPUT_GAMEPAD_DPAD_UP);
    dae::XInputManager::GetInstance().UnbindCommand(XINPUT_GAMEPAD_DPAD_DOWN);
    dae::XInputManager::GetInstance().UnbindCommand(XINPUT_GAMEPAD_A);

    for (auto& go : m_MenuItems) {
        go->MarkForDestroy();
    }
    m_MenuItems.clear();

    m_CmdUp.reset();
    m_CmdDown.reset();
    m_CmdSelect.reset();
}

void MainMenuState::Update(float deltaTime)
{
    deltaTime;

    m_Scene->FixedUpdate(dae::GameTime::GetInstance().GetDeltaTime());
    m_Scene->Update();
    m_Scene->LateUpdate();
}

void MainMenuState::Render()
{
    m_Scene->Render();
}

void MainMenuState::MoveSelectionUp()
{
    //m_MenuItems[m_SelectedIndex]->GetComponent<dae::TextComponent>()->SetColor({ 255,255,255 });

    m_SelectedIndex = (m_SelectedIndex - 1 + (int)m_MenuItems.size()) % (int)m_MenuItems.size();

    //m_MenuItems[m_SelectedIndex]->GetComponent<dae::TextComponent>()->SetColor({ 255,255,0 });
}

void MainMenuState::MoveSelectionDown()
{
    //m_MenuItems[m_SelectedIndex]->GetComponent<dae::TextComponent>()->SetColor({ 255,255,255 });

    m_SelectedIndex = (m_SelectedIndex + 1) % (int)m_MenuItems.size();

    //m_MenuItems[m_SelectedIndex]->GetComponent<dae::TextComponent>()->SetColor({ 255,255,0 });
}

void MainMenuState::ConfirmSelection()
{
    switch (m_SelectedIndex) {
    case 0:
        //dae::GameStateManager::GetInstance().ChangeState(new SinglePlayerState(m_Scene));
        break;
    case 1:
        //dae::GameStateManager::GetInstance().ChangeState(new CoOpState(m_Scene));
        break;
    case 2:
        //dae::GameStateManager::GetInstance().ChangeState(new VersusState(m_Scene));
        break;
    case 3:
        //dae::GameStateManager::GetInstance().ChangeState(new HighscoreState(m_Scene, &m_HighscoreMgr));
        break;
    case 4:
        std::exit(0);
        break;
    }
}
