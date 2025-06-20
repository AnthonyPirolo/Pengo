#include "SinglePlayerState.h"
#include "InputManager.h"
#include "XInputManager.h"
#include "GameStateManager.h"
#include "GameOverState.h"
#include "ResourceManager.h"
#include "ServiceLocator.h"
#include "GridViewComponent.h"
#include "LivesComponent.h"
#include "ScoreComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "TextComponent.h"
#include "Scene.h"
#include "EnemyAIComponent.h"
#include <SDL.h>
#include <Xinput.h>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include "GameCommands.h"
#include "CollisionSystem.h"
#include "PlayerCollisionListener.h"
#include "GameManager.h"
#include "ScoreObserver.h"
#include "TextureComponent.h"
#include "HighscoreManager.h"
#include "HighScoreState.h"
#include "StateComponent.h"
#include "SoundPlayer.h"

SinglePlayerState::SinglePlayerState(dae::Scene* scene, std::shared_ptr<dae::GameObject> grid, std::shared_ptr<HighscoreManager> highscoreMgr)
    : m_Scene(scene), m_Grid(std::move(grid)), m_GridView(nullptr), m_ScoreComp(nullptr), m_HighscoreMgr(std::move(highscoreMgr)), m_LevelTimer(0),
    m_LivesComp(nullptr), m_TimerRunning(false)
{
    m_GridView = m_Grid->GetComponent<dae::GridViewComponent>();
    m_ScoreComp = m_GridView->GetScoreComponent();

    m_LevelMgr = std::make_unique<LevelManager>(std::vector<std::string>{
        "Level1.json", "Level2.json", "Level3.json"
    });
}

void SinglePlayerState::OnEnter()
{
    m_HighscoreMgr->Load();
    ServiceLocator::GetSoundSystem().PlayMusic("PengoMain.ogg", 0.5f, true);

    InitHUD();
    InitGridAndLevel();
    InitInput();

    m_LevelTimer = 0.0f;
    m_TimerRunning = true;
}

void SinglePlayerState::InitHUD()
{
    auto background = std::make_shared<dae::GameObject>();
    background->AddComponent<dae::TextureComponent>(background.get(), "LevelBG.png");
    background->GetComponent<dae::TextureComponent>()->SetScale(2.f, 2.f);
    background->SetLocalPosition({ 0.f, 47.f, 0.f });
    m_Scene->Add(background);
}

void SinglePlayerState::InitGridAndLevel()
{
    if (!m_LevelMgr->LoadNextLevel(m_GridView)) {
        std::cerr << "[SinglePlayerState] Failed to load first level!\n";
        return;
    }

    m_PlayerGO = m_GridView->GetSpawnedPlayers()[0];
    m_EnemyGOs = m_GridView->GetSpawnedEnemies();

    m_GameManager = std::make_shared<dae::GameObject>();
    auto gameManagerComp = m_GameManager->AddComponent<dae::GameManager>(m_GameManager.get());

    InitPlayerComponents();

    for (const auto& enemyGO : m_EnemyGOs) {
        if (enemyGO) {
            auto eComp = enemyGO->GetComponent<dae::EnemyAIComponent>();
            gameManagerComp->RegisterEnemy(eComp);
        }
    }
}

void SinglePlayerState::InitPlayerComponents()
{
    auto soundObserver = std::make_shared<dae::SoundPlayer>();

    if (m_ScoreText) {
        m_ScoreText->MarkForDestroy();
        m_ScoreText = nullptr;
    }
    if (m_LivesText) {
        m_LivesText->MarkForDestroy();
        m_LivesText = nullptr;
    }
    if (!m_PlayerGO) return;

    m_LivesComp = m_PlayerGO->GetComponent<dae::LivesComponent>();
    if (!m_LivesComp) {
        m_LivesComp = m_PlayerGO->AddComponent<dae::LivesComponent>(m_PlayerGO.get(), m_Lives);
    }

    m_LivesComp->AddGameOverObserver([this]() { OnPlayerDead(); });

    m_LivesComp->AddObserver([this](int lives) {
        if (m_LivesText)
            m_LivesText->GetComponent<dae::TextComponent>()->SetText("Lives: " + std::to_string(lives));
        });

    if (!m_ScoreObserver)
        m_ScoreObserver = std::make_shared<ScoreObserver>(m_ScoreComp);

    if (auto playerComp = m_PlayerGO->GetComponent<dae::CollisionComponent>())
        playerComp->AttachObserver(soundObserver);

    m_ScoreText = std::make_shared<dae::GameObject>();
    m_ScoreText->AddComponent<dae::TextComponent>(m_ScoreText.get(), "Score: 0", dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
    m_ScoreText->SetLocalPosition({ 10.f, 10.f, 0.f });
    m_Scene->Add(m_ScoreText);

    m_LivesText = std::make_shared<dae::GameObject>();
    m_LivesText->AddComponent<dae::TextComponent>(m_LivesText.get(), "Lives: " + std::to_string(m_LivesComp->GetLives()), dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
    m_LivesText->SetLocalPosition({ 200.f, 10.f, 0.f });
    m_Scene->Add(m_LivesText);

    auto gameManagerComp = m_GameManager->GetComponent<dae::GameManager>();

    if (auto playerComp = m_PlayerGO->GetComponent<dae::PlayerComponent>())
        gameManagerComp->RegisterPlayer(playerComp);

    for (const auto& enemyGO : m_EnemyGOs) {
        if (enemyGO) {
            auto eComp = enemyGO->GetComponent<dae::EnemyAIComponent>();
            if (eComp) {
                eComp->AttachObserver(m_ScoreObserver);
				eComp->AttachObserver(soundObserver);
            }
        }
    }
}

void SinglePlayerState::InitInput()
{
    auto& inputMgr = ServiceLocator::GetInputManager();
    auto& xi = ServiceLocator::GetXInputManager();

    const float speed = 100.0f;
    const std::vector<std::pair<SDL_KeyCode, glm::vec3>> keyDirections = {
        {SDLK_UP,    {0.0f, -1.0f, 0.0f}},
        {SDLK_DOWN,  {0.0f,  1.0f, 0.0f}},
        {SDLK_LEFT,  {-1.0f, 0.0f, 0.0f}},
        {SDLK_RIGHT, {1.0f,  0.0f, 0.0f}},
    };

    for (const auto& [key, dir] : keyDirections) {
        auto moveCmd = std::make_shared<MoveCommand>(m_PlayerGO.get(), speed);
        moveCmd->SetDirection(dir);
        inputMgr.BindCommand(key, dae::InputManager::KeyState::Held, moveCmd);
    }

    const std::vector<std::pair<dae::XInputManager::ButtonID, glm::vec3>> gpDirections = {
        {XINPUT_GAMEPAD_DPAD_UP,    {0.0f, -1.0f, 0.0f}},
        {XINPUT_GAMEPAD_DPAD_DOWN,  {0.0f,  1.0f, 0.0f}},
        {XINPUT_GAMEPAD_DPAD_LEFT,  {-1.0f, 0.0f, 0.0f}},
        {XINPUT_GAMEPAD_DPAD_RIGHT, {1.0f,  0.0f, 0.0f}},
    };

    for (const auto& [btn, dir] : gpDirections) {
        auto moveCmd = std::make_shared<MoveCommand>(m_PlayerGO.get(), speed);
        moveCmd->SetDirection(dir);
        xi.BindCommand(btn, dae::XInputManager::ButtonState::Held, moveCmd);
    }

    auto soundMuteCmd = std::make_shared<SoundCommand>(m_PlayerGO.get(), 0, 0.f);
    inputMgr.BindCommand(SDLK_F2, dae::InputManager::KeyState::Pressed, soundMuteCmd);

    inputMgr.BindCommand(SDLK_F1, dae::InputManager::KeyState::Pressed,
        std::make_shared<LambdaCommand>([this]() { this->SkipToNextLevel(); }));
}

void SinglePlayerState::OnExit()
{
    ServiceLocator::GetSoundSystem().StopMusic();
    if (m_LivesText) m_LivesText->MarkForDestroy();
    m_PlayerGO.reset();
    m_EnemyGOs.clear();
    m_LivesComp = nullptr;
    m_ScoreComp = nullptr;
    m_GridView = nullptr;
    m_LevelMgr->Reset();
}

void SinglePlayerState::Update(float)
{
    auto gameManagerComp = m_GameManager->GetComponent<dae::GameManager>();

    if (m_PlayerGO) {
        if (auto playerComp = m_PlayerGO->GetComponent<dae::PlayerComponent>()) {
            if (!playerComp->IsAlive()) {
                OnPlayerDead();
                m_ResetTimer = 0.0f;
                gameManagerComp->ResetRound();
                return;
            }
        }
    }

    m_ScoreText->GetComponent<dae::TextComponent>()->SetText(
        "Score: " + std::to_string(m_ScoreComp ? m_ScoreComp->GetScore() : 0)
    );

    if (m_TimerRunning)
        m_LevelTimer += dae::GameTime::GetInstance().GetDeltaTime();

    if (!m_GridView->HasEnemiesRemaining())
        OnLevelComplete();

    ServiceLocator::GetCollisionSystem().CheckAll();
}

void SinglePlayerState::Render() {}

void SinglePlayerState::OnPlayerDead()
{
    if (m_LivesComp) {
        m_LivesComp->LoseLife();
		m_Lives = m_LivesComp->GetLives();
        if (m_LivesComp->GetLives() <= 0) {
            if (m_ScoreComp)
                HighscoreManager::SetPendingScore(m_ScoreComp->GetScore());
            m_RequestedTransition = StateTransition::ToHighScore;
        } else {
            m_TimerRunning = false;
        }
    }
}

void SinglePlayerState::OnLevelComplete()
{
    if (m_LevelTimer < 60.0f && m_ScoreComp) {
        int bonus = static_cast<int>((60.0f - m_LevelTimer) * 10.0f);
        m_ScoreComp->AddPoints(bonus);
    }

    if (m_LevelMgr->LoadNextLevel(m_GridView)) {
        auto gameManagerComp = m_GameManager->GetComponent<dae::GameManager>();
        gameManagerComp->UnregisterPlayer();
        gameManagerComp->UnregisterEnemies();

        m_LevelTimer = 0.0f;
        m_TimerRunning = true;
        m_PlayerGO = m_GridView->GetSpawnedPlayers().empty() ? nullptr : m_GridView->GetSpawnedPlayers()[0];
        m_EnemyGOs = m_GridView->GetSpawnedEnemies();

        InitPlayerComponents();

        gameManagerComp->RegisterPlayer(m_PlayerGO->GetComponent<dae::PlayerComponent>());

        for (const auto& enemyGO : m_EnemyGOs) {
            if (enemyGO) {
                auto eComp = enemyGO->GetComponent<dae::EnemyAIComponent>();
                gameManagerComp->RegisterEnemy(eComp);
                if (eComp && m_ScoreObserver)
                    eComp->AttachObserver(m_ScoreObserver);
            }
        }
        UnbindKeys();
        InitInput();
    }
    else {
        m_Scene->RemoveAll();
        m_TimerRunning = false;
        if (m_ScoreComp)
            HighscoreManager::SetPendingScore(m_ScoreComp->GetScore());

        m_RequestedTransition = StateTransition::ToHighScore;
    }
}

void SinglePlayerState::ToggleMute()
{
    static bool muted = false;
    muted = !muted;
    ServiceLocator::GetSoundSystem().SetMasterVolume(muted ? 0.0f : 1.0f);
}

void SinglePlayerState::UnbindKeys()
{
    auto& inputMgr = ServiceLocator::GetInputManager();
    auto& xi = ServiceLocator::GetXInputManager();

    inputMgr.UnbindCommand(SDLK_UP);
    inputMgr.UnbindCommand(SDLK_DOWN);
    inputMgr.UnbindCommand(SDLK_LEFT);
    inputMgr.UnbindCommand(SDLK_RIGHT);

    inputMgr.UnbindCommand(SDLK_F2);
	inputMgr.UnbindCommand(SDLK_F1);

    xi.UnbindCommand(XINPUT_GAMEPAD_DPAD_UP);
    xi.UnbindCommand(XINPUT_GAMEPAD_DPAD_DOWN);
    xi.UnbindCommand(XINPUT_GAMEPAD_DPAD_LEFT);
    xi.UnbindCommand(XINPUT_GAMEPAD_DPAD_RIGHT);
}

void SinglePlayerState::SkipToNextLevel()
{
    if (m_LevelMgr->LoadNextLevel(m_GridView)) {
        auto gameManagerComp = m_GameManager->GetComponent<dae::GameManager>();
        gameManagerComp->UnregisterPlayer();
        gameManagerComp->UnregisterEnemies();

        m_LevelTimer = 0.0f;
        m_TimerRunning = true;
        m_PlayerGO = m_GridView->GetSpawnedPlayers().empty() ? nullptr : m_GridView->GetSpawnedPlayers()[0];
        m_EnemyGOs = m_GridView->GetSpawnedEnemies();

        InitPlayerComponents();

        gameManagerComp->RegisterPlayer(m_PlayerGO->GetComponent<dae::PlayerComponent>());

        for (const auto& enemyGO : m_EnemyGOs) {
            if (enemyGO) {
                auto eComp = enemyGO->GetComponent<dae::EnemyAIComponent>();
                gameManagerComp->RegisterEnemy(eComp);
                if (eComp && m_ScoreObserver)
                    eComp->AttachObserver(m_ScoreObserver);
            }
        }
        UnbindKeys();
        InitInput();
    }
    else {
        m_Scene->RemoveAll();
        m_TimerRunning = false;
        if (m_ScoreComp)
            HighscoreManager::SetPendingScore(m_ScoreComp->GetScore());

        m_RequestedTransition = StateTransition::ToHighScore;
    }
}