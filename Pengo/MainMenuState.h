#pragma once
#include "BaseState.h"
#include <vector>
#include <memory>
#include "TextComponent.h"
#include "GridViewComponent.h"
#include "Scene.h"

class MenuUpCommand;
class MenuDownCommand;
class MenuSelectCommand;

class MainMenuState : public BaseState {
public:
    MainMenuState(dae::Scene* scene);
    virtual void OnEnter() override;
    virtual void OnExit()  override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;

    void MoveSelectionUp();
    void MoveSelectionDown();
    void ConfirmSelection();

private:
    dae::Scene* m_Scene;
    std::vector<std::shared_ptr<dae::GameObject>> m_MenuItems;
    int m_SelectedIndex;

    std::shared_ptr<MenuUpCommand> m_CmdUp;
    std::shared_ptr<MenuDownCommand> m_CmdDown;
    std::shared_ptr<MenuSelectCommand> m_CmdSelect;

    //HighscoreManager m_HighscoreMgr; // for “view highscores”
};
