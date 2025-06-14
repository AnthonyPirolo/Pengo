#pragma once

#include "BaseState.h"
#include <memory>
#include <vector>
#include <string>
#include "GameManager.h"
#include "ScoreObserver.h"
#include "HighscoreManager.h"
#include "LevelManager.h"

namespace dae {
    class Scene;
    class GridViewComponent;
    class GameObject;
    class LivesComponent;
    class ScoreComponent;
}

class LevelManager;

class SinglePlayerState final : public BaseState
{
public:
    explicit SinglePlayerState(dae::Scene* scene, std::shared_ptr<dae::GameObject> grid, std::shared_ptr<HighscoreManager> highscoreMgr);

    void OnEnter() override;
    void OnExit() override;
    void Update(float deltaTime) override;
    void Render() override;

    void ToggleMute();

    StateTransition GetRequestedTransition() const override { return m_RequestedTransition; }
    void ClearTransitionRequest() override { m_RequestedTransition = StateTransition::None; }

    dae::ScoreComponent* m_ScoreComp;

private:
    void InitHUD();
    void InitGridAndLevel();
    void InitPlayerComponents();
    void InitInput();
    void UnbindKeys();

    void OnPlayerDead();
    void OnLevelComplete();

    dae::Scene* m_Scene;
    std::shared_ptr<dae::GameObject> m_PlayerGO;
    std::vector<std::shared_ptr<dae::GameObject>> m_EnemyGOs;

    dae::LivesComponent* m_LivesComp;
    std::shared_ptr<dae::GameObject> m_LivesText;
    int m_Lives{ 4 };

    std::shared_ptr<ScoreObserver> m_ScoreObserver;

    std::unique_ptr<LevelManager> m_LevelMgr;
    std::shared_ptr<HighscoreManager> m_HighscoreMgr;

    float m_LevelTimer;
    bool m_TimerRunning;
    float m_ResetTimer{ 0.0f };

    std::shared_ptr<dae::GameObject> m_GameManager;
    std::shared_ptr<dae::GameObject> m_ScoreText;

    std::shared_ptr<dae::GameObject> m_Grid;
    dae::GridViewComponent* m_GridView;

    StateTransition m_RequestedTransition{ StateTransition::None };
};