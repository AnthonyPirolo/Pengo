#include "CollisionSystem.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include <iostream>

namespace dae
{
    CollisionComponent::CollisionComponent(GameObject* owner)
        : BaseComponent(owner)
    {
    }

    void CollisionComponent::FixedUpdate(float)
    {

    }

    AABB CollisionComponent::GetAABB() const
    {
        auto* rb = GetOwner()->GetComponent<RigidbodyComponent>();
        if (!rb)
        {
            std::cerr << "[CollisionComponent] No Rigidbody found\n";
            return {};
        }

        glm::vec3 center3D = GetOwner()->GetWorldPosition();
        glm::vec2 center = { center3D.x, center3D.y };
        glm::vec2 halfExtents = { rb->m_Width / 2.0f, rb->m_Height / 2.0f };
        return { center, halfExtents };
    }

    void CollisionSystem::Register(CollisionComponent* comp)
    {
        m_Components.push_back(comp);
    }

    void CollisionSystem::Unregister(CollisionComponent* comp)
    {
        m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), comp), m_Components.end());
    }

    void CollisionSystem::CheckAll()
    {
        for (size_t i = 0; i < m_Components.size(); ++i)
        {
            for (size_t j = i + 1; j < m_Components.size(); ++j)
            {
                auto a = m_Components[i]->GetAABB();
                auto b = m_Components[j]->GetAABB();
                if (a.Intersects(b))
                {
                    std::cout << "[CollisionSystem] Collision detected between objects\n";
                }
            }
        }
    }
}