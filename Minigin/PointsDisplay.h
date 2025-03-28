#pragma once
#pragma once
#include "Observer.h"
#include "TextComponent.h"
#include "PointsComponent.h"

namespace dae
{
    class PointsDisplay : public Observer
    {
    public:
        PointsDisplay(TextComponent* textComponent)
            : m_TextComponent(textComponent) {
        }

        void OnNotify(BaseComponent* entity, Event event) override
        {
            if (event == Event::subjectAttached)
            {
                AddSubject(entity);
            }
            if (event == Event::addPoints)
            {
                UpdateDisplay(entity);
            }
        }

    private:
        void UpdateDisplay(BaseComponent* entity)
        {
            m_TextComponent->SetText(std::string("Points: " + std::to_string(static_cast<PointsComponent*>(entity)->GetPoints())));
        }

        TextComponent* m_TextComponent;
        int m_Points;
    };
}
