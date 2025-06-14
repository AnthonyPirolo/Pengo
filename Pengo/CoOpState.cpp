#include "CoOpState.h"
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
#include <random>
#include "GameCommands.h"
#include "CollisionSystem.h"
#include "PlayerCollisionListener.h"
#include <TextureComponent.h>
#include "SoundPlayer.h"

CoOpState::CoOpState(dae::Scene* scene, std::shared_ptr<dae::GameObject> grid, std::shared_ptr<HighscoreManager> highscoreMgr)
    : m_Scene(scene)
    , m_Grid(std::move(grid))
    , m_GridView(m_Grid->GetComponent<dae::GridViewComponent>())
    , m_ScoreComp(m_GridView->GetScoreComponent())
    , m_LevelTimer(0.f)
    , m_TimerRunning(false)
    , m_HighscoreMgr(std::move(highscoreMgr))
{
    m_LevelMgr = std::make_unique<LevelManager>(
        std::vector<std::string>{ "Level1Co.json", "Level2Co.json", "Level3Co.json" });
}

void CoOpState::OnEnter()
{
    m_HighscoreMgr->Load();
    ServiceLocator::GetSoundSystem().PlayMusic("PengoMain.ogg", 0.5f, true);

    m_SharedLives = 4;

    InitHUD();
    InitGridAndLevel();
    InitInput();

    m_LevelTimer = 0.f;
    m_TimerRunning = true;
}

void CoOpState::InitHUD()
{
    auto bg = std::make_shared<dae::GameObject>();
    bg->AddComponent<dae::TextureComponent>(bg.get(), "LevelBG.png");
    bg->GetComponent<dae::TextureComponent>()->SetScale(2.f, 2.f);
    bg->SetLocalPosition({ 0.f, 47.f, 0.f });
    m_Scene->Add(bg);
}

void CoOpState::InitGridAndLevel()
{
    if (!m_LevelMgr->LoadNextLevel(m_GridView)) {
        std::cerr << "[CoOpState] Failed to load level!\n";
        return;
    }

    auto allPlayers = m_GridView->GetSpawnedPlayers();
    m_EnemyGOs = m_GridView->GetSpawnedEnemies();

    if (allPlayers.size() == 2) {
        if (allPlayers[0]->GetWorldPosition().x < allPlayers[1]->GetWorldPosition().x) {
            m_Player1GO = allPlayers[0];
            m_Player2GO = allPlayers[1];
        }
        else {
            m_Player1GO = allPlayers[1];
            m_Player2GO = allPlayers[0];
        }
    }
    else {
        m_Player1GO = allPlayers.size() > 0 ? allPlayers[0] : nullptr;
        m_Player2GO = allPlayers.size() > 1 ? allPlayers[1] : nullptr;
    }
    m_PlayerGOs = { m_Player1GO, m_Player2GO };

    m_GameManager = std::make_shared<dae::GameObject>();
    auto gmComp = m_GameManager->AddComponent<dae::GameManager>(m_GameManager.get());

    InitPlayerComponents();

    for (auto& eGO : m_EnemyGOs) {
        if (auto eComp = eGO->GetComponent<dae::EnemyAIComponent>()) {
            gmComp->RegisterEnemy(eComp);
        }
    }
}

void CoOpState::InitPlayerComponents()
{
    m_LivesComps.clear();
    auto soundObserver = std::make_shared<dae::SoundPlayer>();

    if (m_ScoreText) {
        m_ScoreText->MarkForDestroy();
        m_ScoreText = nullptr;
    }
    if (m_LivesText) {
        m_LivesText->MarkForDestroy();
        m_LivesText = nullptr;
    }
    auto gmComp = m_GameManager->GetComponent<dae::GameManager>();

    for (size_t i = 0; i < m_PlayerGOs.size(); ++i) {
        auto pGO = m_PlayerGOs[i];
        if (!pGO) continue;

        auto lives = pGO->GetComponent<dae::LivesComponent>();
        if (lives) pGO->RemoveComponent(lives);

        m_LivesComps.push_back(nullptr);
        if (auto pc = pGO->GetComponent<dae::PlayerComponent>())
            gmComp->RegisterPlayer(pc);
    }

    if (!m_ScoreObserver)
        m_ScoreObserver = std::make_shared<ScoreObserver>(m_ScoreComp);

    m_ScoreText = std::make_shared<dae::GameObject>();
    m_ScoreText->AddComponent<dae::TextComponent>(
        m_ScoreText.get(),
        "Score: 0",
        dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36)
    );
    m_ScoreText->SetLocalPosition({ 10.f, 10.f, 0.f });
    m_Scene->Add(m_ScoreText);

    m_LivesText = std::make_shared<dae::GameObject>();
    m_LivesText->AddComponent<dae::TextComponent>(
        m_LivesText.get(),
        "Lives: " + std::to_string(m_SharedLives),
        dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36)
    );
    m_LivesText->SetLocalPosition({ 200.f, 10.f, 0.f });
    m_Scene->Add(m_LivesText);

    for (auto& eGO : m_EnemyGOs) {
        if (auto eComp = eGO->GetComponent<dae::EnemyAIComponent>())
        {
            eComp->AttachObserver(m_ScoreObserver);
            eComp->AttachObserver(soundObserver);
        }

    }
    for (auto& p : m_PlayerGOs)
    {
    if (auto playerComp = p->GetComponent<dae::CollisionComponent>())
        playerComp->AttachObserver(soundObserver);
    }

}

void CoOpState::InitInput()
{
    UnbindKeys();

    auto& in = ServiceLocator::GetInputManager();
    auto& xi = ServiceLocator::GetXInputManager();
    const float speed = 100.f;

    if (m_Player1GO) {
        static const std::vector<std::pair<SDL_KeyCode, glm::vec3>> keys = {
            {SDLK_UP, {0,-1,0}},
            {SDLK_DOWN, {0, 1,0}},
            {SDLK_LEFT, {-1,0,0}},
            {SDLK_RIGHT, {1, 0,0}}
        };
        for (auto& [k, dir] : keys) {
            auto cmd = std::make_shared<MoveCommand>(m_Player1GO.get(), speed);
            cmd->SetDirection(dir);
            in.BindCommand(k, dae::InputManager::KeyState::Held, cmd);
        }
    }

    if (m_Player2GO) {
        static const std::vector<std::pair<dae::XInputManager::ButtonID, glm::vec3>> pads = {
            {XINPUT_GAMEPAD_DPAD_UP,    {0,-1,0}},
            {XINPUT_GAMEPAD_DPAD_DOWN,  {0, 1,0}},
            {XINPUT_GAMEPAD_DPAD_LEFT,  {-1,0,0}},
            {XINPUT_GAMEPAD_DPAD_RIGHT, {1, 0,0}}
        };
        for (auto& [b, dir] : pads) {
            auto cmd = std::make_shared<MoveCommand>(m_Player2GO.get(), speed);
            cmd->SetDirection(dir);
            xi.BindCommand(b, dae::XInputManager::ButtonState::Held, cmd);
        }
    }

    if (m_Player1GO) {
        auto mute = std::make_shared<SoundCommand>(m_Player1GO.get(), 0, 0.f);
        in.BindCommand(SDLK_F2, dae::InputManager::KeyState::Pressed, mute);
    }
}

void CoOpState::UnbindKeys()
{
    auto& in = ServiceLocator::GetInputManager();
    auto& xi = ServiceLocator::GetXInputManager();

    in.UnbindCommand(SDLK_w);
    in.UnbindCommand(SDLK_s);
    in.UnbindCommand(SDLK_a);
    in.UnbindCommand(SDLK_d);
    in.UnbindCommand(SDLK_F2);

    xi.UnbindCommand(XINPUT_GAMEPAD_DPAD_UP);
    xi.UnbindCommand(XINPUT_GAMEPAD_DPAD_DOWN);
    xi.UnbindCommand(XINPUT_GAMEPAD_DPAD_LEFT);
    xi.UnbindCommand(XINPUT_GAMEPAD_DPAD_RIGHT);
}

void CoOpState::OnExit()
{
    ServiceLocator::GetSoundSystem().StopMusic();
    UnbindKeys();
    if (m_LivesText) m_LivesText->MarkForDestroy();
    m_Scene->RemoveAll();

    m_PlayerGOs.clear();
    m_EnemyGOs.clear();
    m_LivesComps.clear();
    m_GridView = nullptr;
    m_LevelMgr->Reset();
}

void CoOpState::Update(float)
{
    auto gmComp = m_GameManager->GetComponent<dae::GameManager>();

    for (size_t i = 0; i < m_PlayerGOs.size(); ++i) {
        if (auto pc = m_PlayerGOs[i]->GetComponent<dae::PlayerComponent>()) {
            if (!pc->IsAlive()) {
                OnPlayerDead();
                gmComp->ResetRound();
                InitInput();
                return;
            }
        }
    }

    if (m_ScoreComp) {
        m_ScoreText->GetComponent<dae::TextComponent>()
            ->SetText("Score: " + std::to_string(m_ScoreComp->GetScore()));
    }

    if (m_TimerRunning) {
        m_LevelTimer += dae::GameTime::GetInstance().GetDeltaTime();
    }

    if (!m_GridView->HasEnemiesRemaining()) {
        OnLevelComplete();
    }

    ServiceLocator::GetCollisionSystem().CheckAll();
}

void CoOpState::OnPlayerDead()
{
    m_SharedLives--;
    if (m_LivesText)
        m_LivesText->GetComponent<dae::TextComponent>()->SetText("Lives: " + std::to_string(m_SharedLives));

    if (m_SharedLives <= 0) {
        m_TimerRunning = false;
        if (m_ScoreComp)
            HighscoreManager::SetPendingScore(m_ScoreComp->GetScore());
        m_RequestedTransition = StateTransition::ToHighScore;
    }
    else {
        m_TimerRunning = false;
    }
}

void CoOpState::OnLevelComplete()
{
    if (m_LevelTimer < 60.f && m_ScoreComp) {
        m_ScoreComp->AddPoints(int((60.f - m_LevelTimer) * 10.f));
    }

    if (m_LevelMgr->LoadNextLevel(m_GridView)) {
        InitGridAndLevel();

        auto gmComp = m_GameManager->GetComponent<dae::GameManager>();
        for (auto& pGO : m_PlayerGOs) {
            if (auto pc = pGO->GetComponent<dae::PlayerComponent>())
                gmComp->UnregisterPlayer();
        }
        gmComp->UnregisterEnemies();

        m_LevelTimer = 0.f;
        m_TimerRunning = true;

        InitPlayerComponents();
        InitInput();

        if (m_ScoreText && m_ScoreComp)
            m_ScoreText->GetComponent<dae::TextComponent>()->SetText("Score: " + std::to_string(m_ScoreComp->GetScore()));
        if (m_LivesText)
            m_LivesText->GetComponent<dae::TextComponent>()->SetText("Lives: " + std::to_string(m_SharedLives));
    }
    else {
        m_TimerRunning = false;

        if (m_ScoreComp)
            HighscoreManager::SetPendingScore(m_ScoreComp->GetScore());
        m_RequestedTransition = StateTransition::ToHighScore;
    }
}

void CoOpState::ToggleMute()
{
    static bool muted = false;
    muted = !muted;
    ServiceLocator::GetSoundSystem().SetMasterVolume(muted ? 0.f : 1.f);
}