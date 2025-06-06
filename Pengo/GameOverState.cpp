// GameOverState.cpp
#include "GameOverState.h"
#include "Renderer.h"
#include "TextComponent.h"
#include "InputManager.h"

GameOverState::GameOverState(dae::Scene* scene, int finalScore, HighscoreManager* hsMgr)
    : m_Scene(scene)
    , m_FinalScore(finalScore)
    , m_HSMgr(hsMgr)
    , m_CurrentLetterIndex(0)
    , m_Initials("___")
    , m_WaitingForHighscore(true)
    , m_HighscoreSaved(false)
{
}

void GameOverState::OnEnter()
{
    m_Scene->RemoveAll();
    auto go = std::make_shared<dae::GameObject>();
    //auto txt = go->AddComponent<dae::TextComponent>(go.get(), "GAME OVER - SCORE: " + std::to_string(m_FinalScore), );
    m_Scene->Add(go);

    auto go2 = std::make_shared<dae::GameObject>();
    //auto txt2 = go2->AddComponent<dae::TextComponent>(go2.get(), "INIT: " + m_Initials, );
    m_Scene->Add(go2);
}

void GameOverState::Update(float deltaTime)
{
    if (!m_WaitingForHighscore) {
        //if (dae::InputManager::GetInstance().IsPressed(SDLK_RETURN) ||
        //    dae::XInputManager::GetInstance().IsPressed(XINPUT_GAMEPAD_START))
        //{
        //    // Transition back to main menu or restart
        //    dae::SceneManager::GetInstance().ChangeState(/* new MainMenuState() or SinglePlayerState( ) */);
        //}
        return;
    }

    //static const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    //if (dae::InputManager::GetInstance().IsPressedOnce(SDLK_UP)) {
    //    char& c = m_Initials[m_CurrentLetterIndex];
    //    if (c == '_') c = 'A';
    //    else {
    //        size_t idx = alphabet.find(c);
    //        if (idx == std::string::npos || idx + 1 >= alphabet.size()) c = 'A';
    //        else c = alphabet[idx + 1];
    //    }
    //}
    //if (dae::InputManager::GetInstance().IsPressedOnce(SDLK_DOWN)) {
    //    char& c = m_Initials[m_CurrentLetterIndex];
    //    if (c == '_') c = 'Z';
    //    else {
    //        size_t idx = alphabet.find(c);
    //        if (idx == std::string::npos || idx == 0) c = 'Z';
    //        else c = alphabet[idx - 1];
    //    }
    //}
    //if (dae::InputManager::GetInstance().IsPressedOnce(SDLK_RIGHT)) {
    //    if (m_CurrentLetterIndex < 2) {
    //        ++m_CurrentLetterIndex;
    //    }
    //    else {
    //        HighscoreEntry entry{ m_Initials, m_FinalScore };
    //        m_HSMgr->AddEntry(entry);
    //        m_WaitingForHighscore = false;
    //        m_HighscoreSaved = true;

    //        auto topList = m_HSMgr->GetTopN(10);
    //        int y = 3;
    //        for (auto& he : topList) {
    //            auto ent = std::make_shared<dae::GameObject>();
    //            auto t = ent->AddComponent<dae::TextComponent>(ent.get(),
    //                he.initials + " - " + std::to_string(he.score), );
    //            ent->GetParent()->SetLocalPosition(glm::vec3(50, 50 + 20 * y, 0));
    //            m_Scene->Add(ent);
    //            y++;
    //        }
    //    }
    //}

    deltaTime;
}

void GameOverState::Render()
{

    m_Scene->Render();
}
