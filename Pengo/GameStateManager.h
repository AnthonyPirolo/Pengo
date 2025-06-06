#pragma once
#include "BaseState.h"
#include <memory>

namespace dae {
    class GameStateManager {
    public:
        static GameStateManager& GetInstance();

        void ChangeState(BaseState* newState);

        void Update(float deltaTime);
        void Render();

    private:
        GameStateManager() = default;
        ~GameStateManager();
        std::unique_ptr<BaseState> m_CurrentState;
    };
}
