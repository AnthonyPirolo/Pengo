#pragma once
#include <memory>
#include "BaseState.h"
#include "MainMenuState.h"
#include "SinglePlayerState.h"
#include "CoOpState.h"
#include "HighScoreState.h"
#include "HighscoreManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GridViewComponent.h"

class GameStateManager final
{

public:

    static GameStateManager& GetInstance()
    {
        static GameStateManager instance;
        return instance;
    }

    void Init(dae::Scene* initialScene)
    {
        m_HighscoreManager = std::make_shared<HighscoreManager>();
        m_CurrentState = std::make_shared<MainMenuState>(initialScene, m_HighscoreManager);
        m_CurrentState->OnEnter();
    }

    void Update(float dt)
    {
        if (m_CurrentState)
        {
            m_CurrentState->Update(dt);
            auto transition = m_CurrentState->GetRequestedTransition();
            if (transition != StateTransition::None)
            {
                m_CurrentState->OnExit();
                m_CurrentState = CreateStateForTransition(transition);
                if (m_CurrentState)
                    m_CurrentState->OnEnter();
            }
        }
    }

    void Render()
    {
        if (m_CurrentState)
            m_CurrentState->Render();
    }

    BaseState* GetCurrentState() { return m_CurrentState.get(); }

private:
private:
    std::shared_ptr<BaseState> m_CurrentState;
    std::shared_ptr<HighscoreManager> m_HighscoreManager;

    std::shared_ptr<BaseState> CreateStateForTransition(StateTransition transition)
    {
        switch (transition)
        {
        case StateTransition::ToMainMenu:
        {
            auto& sceneMgr = dae::SceneManager::GetInstance();
            auto& newScene = sceneMgr.CreateScene("MainMenu");
            return std::make_shared<MainMenuState>(&newScene, m_HighscoreManager);
        }
        case StateTransition::ToSinglePlayer:
        {
            auto& sceneMgr = dae::SceneManager::GetInstance();
            auto& newScene = sceneMgr.CreateScene("SinglePlayer");
            auto grid = std::make_shared<dae::GameObject>();
            grid->AddComponent<dae::GridViewComponent>(grid.get(), 32, glm::vec3{ -16.0f, 30.0f, 0.0f });
            newScene.Add(grid);
            return std::make_shared<SinglePlayerState>(&newScene, grid, m_HighscoreManager);
        }
        case StateTransition::ToCoOp:
        {
            auto& sceneMgr = dae::SceneManager::GetInstance();
            auto& newScene = sceneMgr.CreateScene("CoOp");
            auto grid = std::make_shared<dae::GameObject>();
            grid->AddComponent<dae::GridViewComponent>(grid.get(), 32, glm::vec3{ -16.0f, 30.0f, 0.0f });
            newScene.Add(grid);
            return std::make_shared<CoOpState>(&newScene, grid, m_HighscoreManager);
        }
        case StateTransition::ToHighScore:
        {
            bool viewOnly = false;
            if (auto prev = m_CurrentState.get())
            {
                if (dynamic_cast<MainMenuState*>(prev))
                    viewOnly = true;
            }
            int score = HighscoreManager::GetPendingScore();
            auto& sceneMgr = dae::SceneManager::GetInstance();
            sceneMgr.CreateScene("HighScore");
            return std::make_shared<HighScoreState>(score, m_HighscoreManager, viewOnly);
        }
        default:
            return nullptr;
        }
    }
};