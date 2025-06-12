#pragma once
#include "Observer.h"
#include "PlayerComponent.h"
#include "CollisionComponent.h"

namespace dae
{
    class PlayerCollisionListener final : public Observer
    {
    public:
        PlayerCollisionListener(PlayerComponent* player)
            : m_Player(player)
        {
            if (!player) return;
        }

        ~PlayerCollisionListener() override = default;

        void OnNotify(BaseComponent* entity, Event event) override
        {
            if (event == Event::subjectAttached)
            {
                AddSubject(entity);
            }
            else if (event == Event::playerDied)
            {
				m_Player->Die(3.0f);
            }
        }

    private:
        PlayerComponent* m_Player;
    };
}
