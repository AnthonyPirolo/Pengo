#pragma once
#include "Observer.h"
#include "TextComponent.h"
#include "HealthComponent.h"

namespace dae
{
    class LivesDisplay : public Observer
    {
    public:
        LivesDisplay(TextComponent* textComponent)
            : m_TextComponent(textComponent) {}

        void OnNotify(BaseComponent* entity, Event event) override
        {
            if (event == Event::subjectAttached)
            {
                AddSubject(entity);
            }
            if (event == Event::playerDied)
            {
                UpdateDisplay(entity);
            }
			if (event == Event::playerHit)
			{
				UpdateDisplay(entity);
			}
        }

    private:
        void UpdateDisplay(BaseComponent* entity)
        {

            auto healthComponent = dynamic_cast<HealthComponent*>(entity);
            if (healthComponent)
            {
                int remainingLives = static_cast<int>(healthComponent->GetHealth());
				if (remainingLives == 0) m_TextComponent->SetText("Player died!");
                else m_TextComponent->SetText("Lives: " + std::to_string(remainingLives));
            }
        }

        TextComponent* m_TextComponent;
    };
}
