#pragma once
#include "BaseState.h"
#include "HighscoreManager.h"
#include "Scene.h"

class GameOverState : public BaseState 
{
public:
    GameOverState(dae::Scene* scene, int finalScore, HighscoreManager* hsMgr);
    virtual void OnEnter() override;
    virtual void OnExit()  override {}
    virtual void Update(float deltaTime) override;
    virtual void Render() override;

private:
    dae::Scene* m_Scene;
    int m_FinalScore;
    HighscoreManager* m_HSMgr;

    int m_CurrentLetterIndex;
    std::string m_Initials;
    bool  m_WaitingForHighscore;
    bool  m_HighscoreSaved;
};
