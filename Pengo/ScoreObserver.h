#pragma once

#include "Observer.h"
#include "ScoreComponent.h"
#include "GameObject.h"

class ScoreObserver final : public dae::Observer
{
public:

	explicit ScoreObserver(dae::ScoreComponent* scoreComponent)
		: m_ScoreComponent(scoreComponent)
	{
		if (m_ScoreComponent)
			AddSubject(m_ScoreComponent);
	}
	void OnNotify(dae::BaseComponent* entity, Event event) override
	{
		if (event == Event::EnemyDied)
		{
			if (auto* scoreComp = static_cast<dae::ScoreComponent*>(entity))
			{
				m_ScoreComponent->AddPoints(400);
			}
		}
	}
private:
	dae::ScoreComponent* m_ScoreComponent{ nullptr };
};
