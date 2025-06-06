#include "LivesComponent.h"
#include <iostream>

namespace dae
{
    LivesComponent::LivesComponent(GameObject* owner, int startingLives)
        : BaseComponent(owner)
        , m_Lives(startingLives)
    {
    }

    void LivesComponent::LoseLife()
    {
        if (m_GameOverTriggered)
            return;

        m_Lives = std::max(0, m_Lives - 1);

        for (const auto& observer : m_LivesObservers)
            observer(m_Lives);

        if (m_Lives == 0)
        {
            m_GameOverTriggered = true;
            for (const auto& observer : m_GameOverObservers)
                observer();
        }
    }

    void LivesComponent::AddObserver(std::function<void(int)> onLivesChanged)
    {
        m_LivesObservers.emplace_back(std::move(onLivesChanged));
    }

    void LivesComponent::AddGameOverObserver(std::function<void()> onGameOver)
    {
        m_GameOverObservers.emplace_back(std::move(onGameOver));
    }

    void LivesComponent::ResetLives(int newLives)
    {
        m_Lives = newLives;
        m_GameOverTriggered = false;
    }
}
