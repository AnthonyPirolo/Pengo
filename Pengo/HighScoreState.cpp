#include "HighScoreState.h"
#include "InputManager.h"
#include "XInputManager.h"
#include "ServiceLocator.h"
#include "GameStateManager.h"
#include "MainMenuState.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameCommands.h"
#include "StateComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Scene.h"

HighScoreState::HighScoreState(int score, std::shared_ptr<HighscoreManager> highscoreMgr, bool viewOnly)
    : m_Score(score), m_HighscoreMgr(std::move(highscoreMgr)), m_ViewOnly(viewOnly)
{
}

void HighScoreState::OnEnter()
{
    auto& currentScene = dae::SceneManager::GetInstance().GetActiveScene();
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);

    auto& input = ServiceLocator::GetInputManager();
    auto& xi = ServiceLocator::GetXInputManager();

    if (m_ViewOnly) {
        m_HighscoreMgr->Load();
        m_TopScores = m_HighscoreMgr->GetTopN(3);
        ShowTopScoresUI();
        m_Phase = EntryPhase::ShowTopScores;
        m_EntrySubmitted = true;
        xi.BindCommand(XINPUT_GAMEPAD_B, dae::XInputManager::ButtonState::Pressed, std::make_shared<LambdaCommand>([this] { BackToMenu(); }));
        input.BindCommand(SDLK_ESCAPE, dae::InputManager::KeyState::Pressed, std::make_shared<LambdaCommand>([this] { BackToMenu(); }));
        return;
    }

    m_Phase = EntryPhase::EnterInitials;
    m_CurrentLetter = 0;
    m_EntrySubmitted = false;
    m_Initials = { 'A', 'A', 'A' };

    // Score display
    m_ScoreText = std::make_shared<dae::GameObject>();
    m_ScoreText->AddComponent<dae::TextComponent>(m_ScoreText.get(), "Your Score: " + std::to_string(m_Score), font);
    m_ScoreText->SetLocalPosition({ 20.f, 60.f, 0.f });
    currentScene.Add(m_ScoreText);

    // Initials entry display
    m_InitialsText = std::make_shared<dae::GameObject>();
    m_InitialsText->AddComponent<dae::TextComponent>(m_InitialsText.get(), "", font);
    m_InitialsText->SetLocalPosition({ 20.f, 120.f, 0.f });
    currentScene.Add(m_InitialsText);

    // Prompt
    m_PromptText = std::make_shared<dae::GameObject>();
    m_PromptText->AddComponent<dae::TextComponent>(m_PromptText.get(), "(Use arrows/DPAD to change, Enter/Start to submit, Esc/B to cancel)", font);
    m_PromptText->SetLocalPosition({ 20.f, 180.f, 0.f });
    currentScene.Add(m_PromptText);

    UpdateInitialsText();

    input.BindCommand(SDLK_UP, dae::InputManager::KeyState::Pressed, std::make_shared<LambdaCommand>([this] { NextChar(); }));
    input.BindCommand(SDLK_DOWN, dae::InputManager::KeyState::Pressed, std::make_shared<LambdaCommand>([this] { PrevChar(); }));
    input.BindCommand(SDLK_LEFT, dae::InputManager::KeyState::Pressed, std::make_shared<LambdaCommand>([this] { PrevLetter(); }));
    input.BindCommand(SDLK_RIGHT, dae::InputManager::KeyState::Pressed, std::make_shared<LambdaCommand>([this] { NextLetter(); }));
    input.BindCommand(SDLK_RETURN, dae::InputManager::KeyState::Pressed, std::make_shared<LambdaCommand>([this] { if (!m_EntrySubmitted) SubmitScore(); }));
    input.BindCommand(SDLK_ESCAPE, dae::InputManager::KeyState::Pressed, std::make_shared<LambdaCommand>([this] { BackToMenu(); }));

    xi.BindCommand(XINPUT_GAMEPAD_DPAD_UP, dae::XInputManager::ButtonState::Pressed, std::make_shared<LambdaCommand>([this] { NextChar(); }));
    xi.BindCommand(XINPUT_GAMEPAD_DPAD_DOWN, dae::XInputManager::ButtonState::Pressed, std::make_shared<LambdaCommand>([this] { PrevChar(); }));
    xi.BindCommand(XINPUT_GAMEPAD_DPAD_LEFT, dae::XInputManager::ButtonState::Pressed, std::make_shared<LambdaCommand>([this] { PrevLetter(); }));
    xi.BindCommand(XINPUT_GAMEPAD_DPAD_RIGHT, dae::XInputManager::ButtonState::Pressed, std::make_shared<LambdaCommand>([this] { NextLetter(); }));
    xi.BindCommand(XINPUT_GAMEPAD_START, dae::XInputManager::ButtonState::Pressed, std::make_shared<LambdaCommand>([this] { if (!m_EntrySubmitted) SubmitScore(); }));
    xi.BindCommand(XINPUT_GAMEPAD_B, dae::XInputManager::ButtonState::Pressed, std::make_shared<LambdaCommand>([this] { BackToMenu(); }));
}

void HighScoreState::OnExit()
{
    auto& input = ServiceLocator::GetInputManager();
    auto& xi = ServiceLocator::GetXInputManager();

    input.UnbindCommand(SDLK_UP);
    input.UnbindCommand(SDLK_DOWN);
    input.UnbindCommand(SDLK_LEFT);
    input.UnbindCommand(SDLK_RIGHT);
    input.UnbindCommand(SDLK_RETURN);
    input.UnbindCommand(SDLK_ESCAPE);

    xi.UnbindCommand(XINPUT_GAMEPAD_DPAD_UP);
    xi.UnbindCommand(XINPUT_GAMEPAD_DPAD_DOWN);
    xi.UnbindCommand(XINPUT_GAMEPAD_DPAD_LEFT);
    xi.UnbindCommand(XINPUT_GAMEPAD_DPAD_RIGHT);
    xi.UnbindCommand(XINPUT_GAMEPAD_START);
    xi.UnbindCommand(XINPUT_GAMEPAD_B);

    // Mark all UI objects for destruction
    if (m_ScoreText) m_ScoreText->MarkForDestroy();
    if (m_InitialsText) m_InitialsText->MarkForDestroy();
    if (m_PromptText) m_PromptText->MarkForDestroy();
    for (auto& go : m_HighscoreTextGOs) go->MarkForDestroy();
    m_HighscoreTextGOs.clear();
}

void HighScoreState::Update(float)
{

}

void HighScoreState::NextLetter()
{
    if (m_Phase != EntryPhase::EnterInitials) return;
    m_CurrentLetter = (m_CurrentLetter + 1) % 3;
    UpdateInitialsText();
}

void HighScoreState::PrevLetter()
{
    if (m_Phase != EntryPhase::EnterInitials) return;
    m_CurrentLetter = (m_CurrentLetter + 2) % 3;
    UpdateInitialsText();
}

void HighScoreState::NextChar()
{
    if (m_Phase != EntryPhase::EnterInitials) return;
    m_Initials[m_CurrentLetter] = (m_Initials[m_CurrentLetter] == 'Z') ? 'A' : m_Initials[m_CurrentLetter] + 1;
    UpdateInitialsText();
}

void HighScoreState::PrevChar()
{
    if (m_Phase != EntryPhase::EnterInitials) return;
    m_Initials[m_CurrentLetter] = (m_Initials[m_CurrentLetter] == 'A') ? 'Z' : m_Initials[m_CurrentLetter] - 1;
    UpdateInitialsText();
}

void HighScoreState::SubmitScore()
{
    std::string initials(m_Initials.begin(), m_Initials.end());
    m_HighscoreMgr->AddEntry({ initials, m_Score });
    m_TopScores = m_HighscoreMgr->GetTopN(3);
    m_Phase = EntryPhase::ShowTopScores;
    m_EntrySubmitted = true;

	if (m_ScoreText) m_ScoreText->MarkForDestroy();
    if (m_InitialsText) m_InitialsText->MarkForDestroy();
    if (m_PromptText) m_PromptText->MarkForDestroy();

    ShowTopScoresUI();
}

void HighScoreState::BackToMenu()
{
    auto& sceneMgr = dae::SceneManager::GetInstance();
    auto& newScene = sceneMgr.CreateScene("MainMenu");
    auto* menuState = new MainMenuState(&newScene);
    dae::GameStateManager::GetInstance().ChangeState(menuState);

    auto state = std::make_shared<dae::GameObject>();
    state->AddComponent<dae::StateComponent>(state.get(), menuState);
    newScene.Add(state);
}

void HighScoreState::Render()
{

}

void HighScoreState::UpdateInitialsText()
{
    std::string initialsStr;
    for (int i = 0; i < 3; ++i) {
        if (i == m_CurrentLetter)
            initialsStr += "[" + std::string(1, m_Initials[i]) + "]";
        else
            initialsStr += " " + std::string(1, m_Initials[i]) + " ";
    }
    if (m_InitialsText)
        m_InitialsText->GetComponent<dae::TextComponent>()->SetText("Enter Initials: " + initialsStr);
}

void HighScoreState::ShowTopScoresUI()
{
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 22);
    auto& currentScene = dae::SceneManager::GetInstance().GetActiveScene();

    auto titleGO = std::make_shared<dae::GameObject>();
    titleGO->AddComponent<dae::TextComponent>(titleGO.get(), "Top 3 High Scores:", font);
    titleGO->SetLocalPosition({ 20.f, 60.f, 0.f });
    currentScene.Add(titleGO);
    m_HighscoreTextGOs.push_back(titleGO);

    float y = 110.f;
    for (const auto& entry : m_TopScores) {
        auto scoreGO = std::make_shared<dae::GameObject>();
        scoreGO->AddComponent<dae::TextComponent>(scoreGO.get(), entry.initials + " " + std::to_string(entry.score), font);
        scoreGO->SetLocalPosition({ 20.f, y, 0.f });
        currentScene.Add(scoreGO);
        m_HighscoreTextGOs.push_back(scoreGO);
        y += 40.f;
    }

    auto promptGO = std::make_shared<dae::GameObject>();
    promptGO->AddComponent<dae::TextComponent>(promptGO.get(), "(Press ESC or B to return)", font);
    promptGO->SetLocalPosition({ 20.f, y + 20.f, 0.f });
    currentScene.Add(promptGO);
    m_HighscoreTextGOs.push_back(promptGO);
}