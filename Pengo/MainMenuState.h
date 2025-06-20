#pragma once
#include "BaseState.h"
#include <vector>
#include <string>
#include <memory>
#include "HighscoreManager.h"

namespace dae {
    class Scene;
    class GameObject;
}

class MainMenuState final : public BaseState
{
public:
    explicit MainMenuState(dae::Scene* scene, std::shared_ptr<HighscoreManager> highscoreMgr);

    void OnEnter() override;
    void OnExit() override;
    void Update(float deltaTime) override;
    void Render() override;

    void NextMode();
    void PrevMode();
    void Confirm();

    StateTransition GetRequestedTransition() const override { return m_RequestedTransition; }
    void ClearTransitionRequest() override { m_RequestedTransition = StateTransition::None; }

private:
    void UpdateTexts();

    dae::Scene* m_Scene;
    std::vector<std::string> m_GameModes{ "Single Player", "Co-Op", "Versus", "High Score" };
    int m_SelectedMode{ 0 };

    std::shared_ptr<HighscoreManager> m_HighscoreManager;

    std::shared_ptr<dae::GameObject> m_ModeText;
    std::shared_ptr<dae::GameObject> m_PromptText;

    StateTransition m_RequestedTransition{ StateTransition::None };
};