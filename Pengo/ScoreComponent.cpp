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

    void ScoreComponent::OnNotify(BaseComponent* entity, Event event)
    {
        if (event == Observer::Event::EnemyDied) {
            AddPoints(100);
		}
        else if (event == Observer::Event::subjectAttached) 
        {
            AddSubject(entity);
        }
    }
}
