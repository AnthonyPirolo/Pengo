#include "ScoreComponent.h"

namespace dae
{
    ScoreComponent::ScoreComponent(GameObject* owner)
        : BaseComponent(owner)
    {
    }

    void ScoreComponent::AddPoints(int amount)
    {
        if (amount > 0)
            m_Score += amount;
    }

    void ScoreComponent::Reset()
    {
        m_Score = 0;
    }
}
