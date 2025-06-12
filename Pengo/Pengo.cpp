//﻿#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameStateManager.h"
#include "SinglePlayerState.h"
#include "StateComponent.h"
#include "MainMenuState.h"

void load()
{
    auto& sceneMgr = dae::SceneManager::GetInstance();
    auto& myScene = sceneMgr.CreateScene("MainMenu");

    auto* menuState = new MainMenuState(&myScene);
    dae::GameStateManager::GetInstance().ChangeState(menuState);

    auto stateGO = std::make_shared<dae::GameObject>();
    stateGO->AddComponent<dae::StateComponent>(stateGO.get(), menuState);
    myScene.Add(stateGO);
}

int main(int, char* [])
{
    dae::Minigin engine("../Data/");
    engine.Run(load);
    return 0;
}




