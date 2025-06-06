#pragma once
#include "ICollisionSystem.h"
#include "CollisionComponent.h"
#include "Observer.h"
#include <algorithm>
#include <iostream>

namespace dae
{
    class CollisionSystem : public ICollisionSystem
    {
    public:
        void Register(CollisionComponent* comp) override
        {
            m_Components.push_back(comp);

        }

        void Unregister(CollisionComponent* comp) override
        {
            m_Components.erase(
                std::remove(m_Components.begin(), m_Components.end(), comp),
                m_Components.end()
            );
        }

        void CheckAll() override
        {
            for (size_t i = 0; i < m_Components.size(); ++i)
            {
                for (size_t j = i + 1; j < m_Components.size(); ++j)
                {
                    auto* a = m_Components[i];
                    auto* b = m_Components[j];

                    if (a->GetAABB().Intersects(b->GetAABB()))
                    {
                        const auto tagA = a->m_Tag;
                        const auto tagB = b->m_Tag;

                        if ((tagA == CollisionTag::Player && tagB == CollisionTag::Enemy) ||
                            (tagA == CollisionTag::Enemy && tagB == CollisionTag::Player))
                        {
                            CollisionComponent* playerCol =
                                (tagA == CollisionTag::Player) ? a : b;
                            playerCol->Notify(Observer::Event::playerDied);

                            return;
                        }
                    }
                }
            }
        }

    private:
        std::vector<CollisionComponent*> m_Components;
    };
}
