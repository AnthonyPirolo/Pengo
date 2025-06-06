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

void load()
{
    auto& sceneMgr = dae::SceneManager::GetInstance();
    auto& myScene = sceneMgr.CreateScene("SinglePlayerTest");

    dae::GameStateManager::GetInstance().ChangeState(
        new SinglePlayerState(&myScene)
    );
}

int main(int, char* [])
{
    dae::Minigin engine("../Data/");
    engine.Run(load);
    return 0;
}




