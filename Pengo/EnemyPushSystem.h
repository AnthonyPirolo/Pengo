#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"
#include "GridViewComponent.h"
#include <CollisionSystem.h>

namespace dae {
    class EnemyPushSystem {
    public:
        EnemyPushSystem(GridViewComponent* gridView);

        void HandleEnemyPush(
            const glm::vec3& direction,
            float pushDistance,
            const AABB& wallAABB);

    private:
        GridViewComponent* m_pGridView;

        bool WillCollideWithWallAABB(
            const AABB& enemyAABB,
            const glm::vec3& direction,
            float distance,
            const std::vector<std::shared_ptr<GameObject>>& walls) const;

        bool IsOutOfBounds(const glm::vec3& position) const;
    };
}
