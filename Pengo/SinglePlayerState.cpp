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
#include "Components.h"
#include "EnemyAIComponent.h"
#include "PlayerComponent.h"
#include "GameCommands.h"
#include "CollisionSystem.h"
#include "Observer.h"

#include <SDL.h>
#include <Xinput.h>
#include <iostream>
#include <numeric>
#include <random>

SinglePlayerState::SinglePlayerState(dae::Scene* scene)
	: m_Scene(scene) {
	m_LevelMgr = std::make_unique<LevelManager>(std::vector<std::string>{
		"Level1.json", "Level2.json", "Level3.json"
	});
	m_HighscoreMgr = std::make_unique<HighscoreManager>();
}

void SinglePlayerState::OnEnter() {
	m_HighscoreMgr->Load();

	auto& soundSystem = ServiceLocator::GetSoundSystem();
	soundSystem.PlayMusic("PengoMain.ogg", 0.5f, true);

	InitHUD();
	InitGridAndLevel();
	InitInput();

	m_LevelTimer = 0.0f;
	m_TimerRunning = true;
}

void SinglePlayerState::InitHUD() {
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto background = std::make_shared<dae::GameObject>();
	background->AddComponent<dae::TextureComponent>(background.get(), "LevelBG.png");
	background->GetComponent<dae::TextureComponent>()->SetScale(2.f, 2.f);
	background->SetLocalPosition({ 0.f, 47.f, 0.f });
	m_Scene->Add(background);
}

void SinglePlayerState::InitGridAndLevel() {
	auto rootGO = std::make_shared<dae::GameObject>();
	m_GridView = rootGO->AddComponent<dae::GridViewComponent>(rootGO.get(), 32, glm::vec3{ -16.f, 30.f, 0.f });
	m_Scene->Add(rootGO);

	if (!m_LevelMgr->LoadNextLevel(m_GridView)) {
		std::cerr << "[SinglePlayerState] Failed to load first level!\n";
		return;
	}

	m_PlayerGO = m_GridView->GetPlayerGameObject();

	InitPlayerComponents();

	for (auto& enemyGO : m_GridView->GetSpawnedEnemies()) {
		if (enemyGO) {
			auto eComp = enemyGO->GetComponent<dae::EnemyAIComponent>();
			if (eComp) 
			{
				if (m_PlayerGO) {
					if (auto scoreComp = m_PlayerGO->GetComponent<dae::ScoreComponent>()) {
						eComp->AttachObserver(std::shared_ptr<dae::Observer>(scoreComp));
					}
				}
				m_GameManager.RegisterEnemy(eComp);
			}
		}
	}

	m_ScoreText = std::make_shared<dae::GameObject>();
	m_ScoreText->AddComponent<dae::TextComponent>(m_ScoreText.get(), "Score: 0", dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
	m_ScoreText->SetLocalPosition({ 10.f, 10.f, 0.f });
	m_Scene->Add(m_ScoreText);
}

void SinglePlayerState::InitPlayerComponents() {
	if (!m_PlayerGO) return;

	m_LivesComp = m_PlayerGO->GetComponent<dae::LivesComponent>();
	if (m_LivesComp) {
		m_LivesComp->AddGameOverObserver([this]() { OnPlayerDead(); });
	}

	m_ScoreComp = m_PlayerGO->AddComponent<dae::ScoreComponent>(m_PlayerGO.get());

	if (auto playerComp = m_PlayerGO->GetComponent<dae::PlayerComponent>()) {
		m_GameManager.RegisterPlayer(playerComp);
	}
}

void SinglePlayerState::InitInput() {
	auto& inputMgr = dae::InputManager::GetInstance();
	auto& xi = dae::XInputManager::GetInstance();
	constexpr float speed = 100.f;

	const std::vector<std::pair<SDL_Keycode, glm::vec3>> keyDirs = {
		{ SDLK_UP,    {  0.f, -1.f, 0.f } },
		{ SDLK_DOWN,  {  0.f,  1.f, 0.f } },
		{ SDLK_LEFT,  { -1.f,  0.f, 0.f } },
		{ SDLK_RIGHT, {  1.f,  0.f, 0.f } },
	};

	for (const auto& [key, dir] : keyDirs) {
		auto move = std::make_shared<MoveCommand>(m_PlayerGO.get(), speed);
		move->SetDirection(dir);
		inputMgr.BindCommand(static_cast<SDL_KeyCode>(key), dae::InputManager::KeyState::Held, move);
	}

	const std::vector<std::pair<dae::XInputManager::ButtonID, glm::vec3>> gpDirs = {
		{ XINPUT_GAMEPAD_DPAD_UP,    {  0.f, -1.f, 0.f } },
		{ XINPUT_GAMEPAD_DPAD_DOWN,  {  0.f,  1.f, 0.f } },
		{ XINPUT_GAMEPAD_DPAD_LEFT,  { -1.f,  0.f, 0.f } },
		{ XINPUT_GAMEPAD_DPAD_RIGHT, {  1.f,  0.f, 0.f } },
	};

	for (const auto& [btn, dir] : gpDirs) {
		auto move = std::make_shared<MoveCommand>(m_PlayerGO.get(), speed);
		move->SetDirection(dir);
		xi.BindCommand(btn, dae::XInputManager::ButtonState::Held, move);
	}

	auto mute = std::make_shared<SoundCommand>(m_PlayerGO.get(), 0, 0.f);
	inputMgr.BindCommand(SDLK_F2, dae::InputManager::KeyState::Pressed, mute);
}

void SinglePlayerState::OnExit() {
	m_Scene->RemoveAll();
	m_PlayerGO.reset();
	m_LivesComp = nullptr;
	m_ScoreComp = nullptr;
	m_GridView = nullptr;
	m_LevelMgr->Reset();
}

void SinglePlayerState::Update(float) {
	if (auto player = m_PlayerGO->GetComponent<dae::PlayerComponent>()) {
		if (!player->IsAlive()) {
			OnPlayerDead();
			m_ResetTimer = 0.0f;
			m_GameManager.ResetRound();
			return;
		}
	}

	if (m_TimerRunning) {
		m_LevelTimer += dae::GameTime::GetInstance().GetDeltaTime();
	}

	if (!m_GridView->HasEnemiesRemaining()) {
		OnLevelComplete();
	}

	m_ScoreText->GetComponent<dae::TextComponent>()->SetText(
		"Score: " + std::to_string(m_ScoreComp ? m_ScoreComp->GetScore() : 0)
	);

	ServiceLocator::GetCollisionSystem().CheckAll();
}

void SinglePlayerState::Render() 
{

}

void SinglePlayerState::OnPlayerDead() {
	if (m_LivesComp) {
		m_LivesComp->LoseLife();
	}
	m_TimerRunning = false;
}

void SinglePlayerState::OnLevelComplete() {
	if (m_LevelTimer < 60.f && m_ScoreComp) {
		int bonus = static_cast<int>((60.f - m_LevelTimer) * 10.f);
		m_ScoreComp->AddPoints(bonus);
	}

	if (m_LevelMgr->LoadNextLevel(m_GridView)) {
		m_LevelTimer = 0.0f;
		m_TimerRunning = true;

		m_PlayerGO = m_GridView->GetPlayerGameObject();

		for (auto& enemyGO : m_GridView->GetSpawnedEnemies()) {
			if (enemyGO) {
				auto eComp = enemyGO->GetComponent<dae::EnemyAIComponent>();
				if (eComp) m_GameManager.RegisterEnemy(eComp);
			}
		}

		if (m_LivesComp) {
			m_LivesComp->ResetLives(4);
		}
	}
	else {
		m_TimerRunning = false;
		int finalScore = (m_ScoreComp ? m_ScoreComp->GetScore() : 0);
		m_HighscoreMgr->AddEntry({ "AAA", finalScore });

		dae::GameStateManager::GetInstance().ChangeState(
			new GameOverState(m_Scene, finalScore, m_HighscoreMgr.get())
		);
	}
}

void SinglePlayerState::ToggleMute() {
	static bool muted = false;
	muted = !muted;
	ServiceLocator::GetSoundSystem().SetMasterVolume(muted ? 0.f : 1.f);
}
