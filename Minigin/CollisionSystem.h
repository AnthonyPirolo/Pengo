#pragma once

#include "BaseComponent.h"
#include <glm.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>

namespace dae
{
    class CollisionComponent;
    class GameObject;

    struct AABB
    {
        glm::vec2 center;
        glm::vec2 halfExtents;

        bool Intersects(const AABB& other) const
        {
            return std::abs(center.x - other.center.x) < (halfExtents.x + other.halfExtents.x) &&
                std::abs(center.y - other.center.y) < (halfExtents.y + other.halfExtents.y);
        }
    };

    class CollisionComponent : public BaseComponent
    {
    public:
        explicit CollisionComponent(GameObject* owner);

        void FixedUpdate(float deltaTime) override;
        void Update() override {}
        void LateUpdate() override {}
        void Render() const override {}

        AABB GetAABB() const;

    private:
        bool m_Resolved = false;
    };

    class CollisionSystem
    {
    public:
        void Register(CollisionComponent* comp);
        void Unregister(CollisionComponent* comp);
        void CheckAll();

    private:
        std::vector<CollisionComponent*> m_Components;
    };
}