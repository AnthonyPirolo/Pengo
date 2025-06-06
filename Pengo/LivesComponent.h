#pragma once
#include "BaseComponent.h"
#include <functional>
#include <vector>
#include <algorithm>

namespace dae
{
    class LivesComponent : public BaseComponent
    {
    public:
        explicit LivesComponent(GameObject* owner, int startingLives = 4);

        void LoseLife();

        int GetLives() const { return m_Lives; }

        void AddObserver(std::function<void(int)> onLivesChanged);
        void AddGameOverObserver(std::function<void()> onGameOver);

        void ResetLives(int newLives = 4);

        void FixedUpdate(float) override {}
        void Update() override {}
        void LateUpdate() override {}
        void Render() const override {}

    private:
        int m_Lives{ 4 };
        bool m_GameOverTriggered{ false };

        std::vector<std::function<void(int)>> m_LivesObservers;
        std::vector<std::function<void()>> m_GameOverObservers;
    };
}
