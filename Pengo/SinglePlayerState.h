#pragma once

#include "BaseState.h"
#include <memory>
#include <vector>
#include <string>
#include "GameManager.h"

namespace dae {
    class Scene;
    class GridViewComponent;
    class GameObject;
    class LivesComponent;
    class ScoreComponent;
}

class LevelManager;
class HighscoreManager;

class SinglePlayerState final : public BaseState 
{
public:
    explicit SinglePlayerState(dae::Scene* scene);

    void OnEnter() override;
    void OnExit() override;
    void Update(float deltaTime) override;
    void Render() override;

    void ToggleMute();

private:

    void InitHUD();
    void InitGridAndLevel();
    void InitPlayerComponents();
    void InitInput();

    void OnPlayerDead();
    void OnLevelComplete();

    dae::Scene* m_Scene;
    dae::GridViewComponent* m_GridView;
    std::shared_ptr<dae::GameObject> m_PlayerGO;
    std::vector<std::shared_ptr<dae::GameObject>> m_EnemyGOs;

    dae::LivesComponent* m_LivesComp;
    dae::ScoreComponent* m_ScoreComp;

    std::unique_ptr<LevelManager> m_LevelMgr;
    std::unique_ptr<HighscoreManager> m_HighscoreMgr;

    float m_LevelTimer;
    bool m_TimerRunning;

	float m_ResetTimer{ 0.0f };

    dae::GameManager m_GameManager;
};
