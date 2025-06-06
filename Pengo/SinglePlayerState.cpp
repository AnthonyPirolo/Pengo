#include "SinglePlayerState.h"

#include "InputManager.h"
#include "XInputManager.h"
#include "GameStateManager.h"
#include "GameOverState.h"
#include "ResourceManager.h"

#include "GridViewComponent.h"
#include "LivesComponent.h"
#include "ScoreComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Renderer.h"
#include "TextComponent.h"
#include "Scene.h"
#include "Components.h"

#include <SDL.h>
#include <Xinput.h>
#include <iostream>

#include "GameCommands.h"

#include <memory>

SinglePlayerState::SinglePlayerState(dae::Scene* scene)
    : m_Scene(scene)
    , m_GridView(nullptr)
    , m_LivesComp(nullptr)
    , m_ScoreComp(nullptr)
    , m_LevelTimer(0.0f)
    , m_TimerRunning(false)
{
    m_LevelMgr = std::make_unique<LevelManager>(std::vector<std::string>{
        "Level1.json", "Level2.json", "Level3.json"
    });
    m_HighscoreMgr = std::make_unique<HighscoreManager>();
}

void SinglePlayerState::OnEnter()
{
    m_Scene->RemoveAll();
    m_HighscoreMgr->Load();

    auto& soundSystem = ServiceLocator::GetSoundSystem();
    soundSystem.PlayMusic("PengoMain.ogg", 0.5f, true);

    InitHUD();
    InitGridAndLevel();
    InitPlayerComponents();
    InitInput();

    m_LevelTimer = 0.0f;
    m_TimerRunning = true;
}

void SinglePlayerState::InitHUD()
{
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    auto background = std::make_shared<dae::GameObject>();
    background->AddComponent<dae::TextureComponent>(background.get(), "LevelBG.png");
    background->GetComponent<dae::TextureComponent>()->SetScale(2.f, 2.f);
    background->SetLocalPosition(glm::vec3(0, 47, 0));
    m_Scene->Add(background);

    auto fps = std::make_shared<dae::GameObject>();
    fps->AddComponent<dae::TextComponent>(fps.get(), "FPS: 0", font);
    fps->AddComponent<dae::FpsComponent>(fps.get());
    fps->SetLocalPosition(glm::vec3(10, 20, 0));
    m_Scene->Add(fps);
}

void SinglePlayerState::InitGridAndLevel()
{
    auto rootGO = std::make_shared<dae::GameObject>();
    m_GridView = rootGO->AddComponent<dae::GridViewComponent>(rootGO.get(), 32, glm::vec3{ -16.0f, 30.0f, 0.0f });
    m_Scene->Add(rootGO);

    if (!m_LevelMgr->LoadNextLevel(m_GridView)) {
        std::cerr << "[SinglePlayerState] Failed to load first level!\n";
        return;
    }

    m_PlayerGO = m_GridView->GetPlayerGameObject();
    m_EnemyGOs = m_GridView->GetSpawnedEnemies();
}

void SinglePlayerState::InitPlayerComponents()
{
    if (!m_PlayerGO) return;

    m_LivesComp = m_PlayerGO->GetComponent<dae::LivesComponent>();
    if (m_LivesComp) {
        m_LivesComp->AddGameOverObserver([this]() { OnPlayerDead(); });
    }

    m_ScoreComp = m_PlayerGO->AddComponent<dae::ScoreComponent>(m_PlayerGO.get());
}

void SinglePlayerState::InitInput()
{
    auto& inputMgr = dae::InputManager::GetInstance();
    auto& xi = dae::XInputManager::GetInstance();

    const float speed = 100.0f;
    const std::vector<std::pair<SDL_Keycode, glm::vec3>> keyDirections = {
        {SDLK_UP,    {0.0f, -1.0f, 0.0f}},
        {SDLK_DOWN,  {0.0f,  1.0f, 0.0f}},
        {SDLK_LEFT,  {-1.0f, 0.0f, 0.0f}},
        {SDLK_RIGHT, {1.0f,  0.0f, 0.0f}},
    };

    for (const std::pair<SDL_Keycode, glm::vec3>& direction : keyDirections) {
        auto moveCmd = std::make_shared<MoveCommand>(m_PlayerGO.get(), speed);
        moveCmd->SetDirection(direction.second);
        inputMgr.BindCommand(static_cast<SDL_KeyCode>(direction.first), dae::InputManager::KeyState::Held, moveCmd);
    }

    const std::vector<std::pair<dae::XInputManager::ButtonID, glm::vec3>> gpDirections = {
        {XINPUT_GAMEPAD_DPAD_UP,    {0.0f, -1.0f, 0.0f}},
        {XINPUT_GAMEPAD_DPAD_DOWN,  {0.0f,  1.0f, 0.0f}},
        {XINPUT_GAMEPAD_DPAD_LEFT,  {-1.0f, 0.0f, 0.0f}},
        {XINPUT_GAMEPAD_DPAD_RIGHT, {1.0f,  0.0f, 0.0f}},
    };

    for (const std::pair<SDL_Keycode, glm::vec3>& direction : gpDirections) {
        auto moveCmd = std::make_shared<MoveCommand>(m_PlayerGO.get(), speed);
        moveCmd->SetDirection(direction.second);
        xi.BindCommand(static_cast<dae::XInputManager::ButtonID>(direction.first), dae::XInputManager::ButtonState::Held, moveCmd);
    }

    inputMgr.BindCommand(SDLK_F2, dae::InputManager::KeyState::Pressed, std::make_shared<SoundCommand>(m_PlayerGO.get(), 0, 0.0f));
}

void SinglePlayerState::OnExit()
{
    m_Scene->RemoveAll();
    m_PlayerGO.reset();
    m_EnemyGOs.clear();
    m_LivesComp = nullptr;
    m_ScoreComp = nullptr;
    m_GridView = nullptr;
    m_LevelMgr->Reset();
}

void SinglePlayerState::Update(float)
{
    if (m_TimerRunning)
        m_LevelTimer += dae::GameTime::GetInstance().GetDeltaTime();

    if (std::none_of(m_EnemyGOs.begin(), m_EnemyGOs.end(), [](const auto& e) {
        return e && !e->IsMarkedForDestroy();
        })) {
        OnLevelComplete();
    }

    m_Scene->FixedUpdate(dae::GameTime::GetInstance().GetDeltaTime());
    m_Scene->Update();
    m_Scene->LateUpdate();
}

void SinglePlayerState::Render()
{
    m_Scene->Render();
}

void SinglePlayerState::OnPlayerDead()
{
    m_TimerRunning = false;
    int finalScore = m_ScoreComp ? m_ScoreComp->GetScore() : 0;

    dae::GameStateManager::GetInstance().ChangeState(
        new GameOverState(m_Scene, finalScore, m_HighscoreMgr.get())
    );
}

void SinglePlayerState::OnLevelComplete()
{
    if (m_LevelTimer < 60.0f && m_ScoreComp) {
        int bonus = static_cast<int>((60.0f - m_LevelTimer) * 10.0f);
        m_ScoreComp->AddPoints(bonus);
    }

    if (m_LevelMgr->LoadNextLevel(m_GridView)) {
        m_LevelTimer = 0.0f;
        m_TimerRunning = true;

        m_PlayerGO = m_GridView->GetPlayerGameObject();
        m_EnemyGOs = m_GridView->GetSpawnedEnemies();

        if (m_LivesComp)
            m_LivesComp->ResetLives(4);
    }
    else {
        m_TimerRunning = false;
        int finalScore = m_ScoreComp ? m_ScoreComp->GetScore() : 0;
        m_HighscoreMgr->AddEntry({ "AAA", finalScore });

        dae::GameStateManager::GetInstance().ChangeState(
            new GameOverState(m_Scene, finalScore, m_HighscoreMgr.get())
        );
    }
}

void SinglePlayerState::ToggleMute()
{
    static bool muted = false;
    muted = !muted;
    ServiceLocator::GetSoundSystem().SetMasterVolume(muted ? 0.0f : 1.0f);
}
