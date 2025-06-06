#pragma once
#include <vector>
#include <glm.hpp>

namespace dae
{
    class CollisionComponent;

    struct AABB
    {
        glm::vec2 center;
        glm::vec2 halfExtents;

        bool Intersects(const AABB& other) const
        {
            return std::abs(center.x - other.center.x) < (halfExtents.x + other.halfExtents.x)
                && std::abs(center.y - other.center.y) < (halfExtents.y + other.halfExtents.y);
        }
    };

    enum class CollisionTag
    {
        None,
        Player,
        Enemy,
        Wall
    };

    class ICollisionSystem
    {
    public:
        virtual ~ICollisionSystem() = default;

        virtual void Register(CollisionComponent* comp) = 0;

        virtual void Unregister(CollisionComponent* comp) = 0;

        virtual void CheckAll() = 0;
    };
}
