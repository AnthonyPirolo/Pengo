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

void load()
{
    auto& sceneMgr = dae::SceneManager::GetInstance();
    auto& myScene = sceneMgr.CreateScene("SinglePlayerTest");
    auto* spState = new SinglePlayerState(&myScene);

    dae::GameStateManager::GetInstance().ChangeState(
		spState
    );

    auto stateGO = std::make_shared<dae::GameObject>();

    stateGO->AddComponent<dae::StateComponent>(stateGO.get(), spState);

    sceneMgr.GetActiveScene().Add(stateGO);
}

int main(int, char* [])
{
    dae::Minigin engine("../Data/");
    engine.Run(load);
    return 0;
}




