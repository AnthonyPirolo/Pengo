#pragma once
#include "BaseState.h"
#include "HighscoreManager.h"
#include "Scene.h"
#include <array>
#include <memory>
#include <vector>
#include <functional>

namespace dae {
    class GameObject;
}

class HighScoreState : public BaseState
{
public:
    HighScoreState(int score, std::shared_ptr<HighscoreManager> highscoreMgr, bool viewOnly = false);

    void OnEnter() override;
    void Update(float) override;
    void Render() override;
    void OnExit() override;

private:
    enum class EntryPhase { EnterInitials, ShowTopScores };
    EntryPhase m_Phase{ EntryPhase::EnterInitials };

    int m_Score;
    std::shared_ptr<HighscoreManager> m_HighscoreMgr;
    std::array<char, 3> m_Initials{ 'A', 'A', 'A' };
    int m_CurrentLetter{ 0 };
    bool m_EntrySubmitted{ false };
    std::vector<HighscoreEntry> m_TopScores;

    // UI GameObjects
    std::shared_ptr<dae::GameObject> m_ScoreText;
    std::shared_ptr<dae::GameObject> m_InitialsText;
    std::shared_ptr<dae::GameObject> m_PromptText;
    std::vector<std::shared_ptr<dae::GameObject>> m_HighscoreTextGOs;

    void NextLetter();
    void PrevLetter();
    void NextChar();
    void PrevChar();
    void SubmitScore();
    void BackToMenu();

    // UI update helpers
    void UpdateInitialsText();
    void ShowTopScoresUI();

    bool m_ViewOnly = false;
};