#include "MainMenuState.h"
#include "Scene.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "InputManager.h"
#include "XInputManager.h"
#include "ResourceManager.h"
#include "GameCommands.h"
#include "StateComponent.h"

MainMenuState::MainMenuState(dae::Scene* scene, std::shared_ptr<HighscoreManager> highscoreMgr)
    : m_Scene(scene), m_HighscoreManager(std::move(highscoreMgr))
{
}

void MainMenuState::OnEnter()
{
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

    m_ModeText = std::make_shared<dae::GameObject>();
    m_ModeText->AddComponent<dae::TextComponent>(m_ModeText.get(), "", font);
    m_ModeText->SetLocalPosition({ 70.f, 100.f, 0.f });
    m_Scene->Add(m_ModeText);

    m_PromptText = std::make_shared<dae::GameObject>();
    m_PromptText->AddComponent<dae::TextComponent>(m_PromptText.get(), "Press A/Enter to start", font);
    m_PromptText->SetLocalPosition({ 70.f, 150.f, 0.f });
    m_Scene->Add(m_PromptText);

    UpdateTexts();

    auto& inputMgr = ServiceLocator::GetInputManager();
    auto& xi = ServiceLocator::GetXInputManager();

    inputMgr.BindCommand(SDLK_LEFT, dae::InputManager::KeyState::Pressed, std::make_shared<LambdaCommand>([this] { PrevMode(); }));
    inputMgr.BindCommand(SDLK_RIGHT, dae::InputManager::KeyState::Pressed, std::make_shared<LambdaCommand>([this] { NextMode(); }));
    inputMgr.BindCommand(SDLK_RETURN, dae::InputManager::KeyState::Pressed, std::make_shared<LambdaCommand>([this] { Confirm(); }));

    xi.BindCommand(XINPUT_GAMEPAD_DPAD_LEFT, dae::XInputManager::ButtonState::Pressed, std::make_shared<LambdaCommand>([this] { PrevMode(); }));
    xi.BindCommand(XINPUT_GAMEPAD_DPAD_RIGHT, dae::XInputManager::ButtonState::Pressed, std::make_shared<LambdaCommand>([this] { NextMode(); }));
    xi.BindCommand(XINPUT_GAMEPAD_A, dae::XInputManager::ButtonState::Pressed, std::make_shared<LambdaCommand>([this] { Confirm(); }));
}

void MainMenuState::OnExit()
{
    auto& inputMgr = ServiceLocator::GetInputManager();
    auto& xi = ServiceLocator::GetXInputManager();

    inputMgr.UnbindCommand(SDLK_LEFT);
    inputMgr.UnbindCommand(SDLK_RIGHT);
    inputMgr.UnbindCommand(SDLK_RETURN);

    xi.UnbindCommand(XINPUT_GAMEPAD_DPAD_LEFT);
    xi.UnbindCommand(XINPUT_GAMEPAD_DPAD_RIGHT);
    xi.UnbindCommand(XINPUT_GAMEPAD_A);

    m_ModeText->MarkForDestroy();
    m_PromptText->MarkForDestroy();
}

void MainMenuState::Update(float)
{
    // No-op
}

void MainMenuState::Render()
{
    // No-op
}

void MainMenuState::NextMode()
{
    m_SelectedMode = (m_SelectedMode + 1) % static_cast<int>(m_GameModes.size());
    UpdateTexts();
}

void MainMenuState::PrevMode()
{
    m_SelectedMode = (m_SelectedMode - 1 + static_cast<int>(m_GameModes.size())) % static_cast<int>(m_GameModes.size());
    UpdateTexts();
}

void MainMenuState::Confirm()
{
    switch (m_SelectedMode)
    {
    case 0:
        m_RequestedTransition = StateTransition::ToSinglePlayer;
        break;
    case 1:
        m_RequestedTransition = StateTransition::ToCoOp;
        break;
    case 3:
        m_RequestedTransition = StateTransition::ToHighScore;
        break;
    default:
        break;
    }
}

void MainMenuState::UpdateTexts()
{
    auto modeText = "Game Mode: " + m_GameModes[m_SelectedMode];
    m_ModeText->GetComponent<dae::TextComponent>()->SetText(modeText);
}