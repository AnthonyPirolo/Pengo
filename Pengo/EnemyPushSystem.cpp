#include "EnemyPushSystem.h"
#include "RigidbodyComponent.h"
#include "EnemyAIComponent.h"

namespace dae {

    EnemyPushSystem::EnemyPushSystem(GridViewComponent* gridView)
        : m_pGridView(gridView) {
    }

    void EnemyPushSystem::HandleEnemyPush(
        const glm::vec3& direction,
        float pushDistance,
        const AABB& wallAABB)
    {
        auto enemies = m_pGridView->GetSpawnedEnemies();
        auto walls = m_pGridView->GetSpawnedWalls();

        for (const auto& enemy : enemies) {
            if (!enemy || enemy->IsMarkedForDestroy()) continue;
            auto* enemyRb = enemy->GetComponent<RigidbodyComponent>();
            if (!enemyRb) continue;

            AABB enemyAABB = enemyRb->GetAABB();

            // Only affect enemies that will be overlapped by the wall after the move
            if (wallAABB.Intersects(enemyAABB)) {
                glm::vec3 enemyNewPosition = enemy->GetWorldPosition() + direction * pushDistance;

                // Out-of-bounds check
                int gridX, gridY;
                m_pGridView->m_Logic.WorldToGrid(enemyNewPosition, gridX, gridY);
                bool outOfBounds = gridX < 0 || gridY < 0 ||
                    gridX >= m_pGridView->m_Model.GetWidth() ||
                    gridY >= m_pGridView->m_Model.GetHeight();

                if (outOfBounds || WillCollideWithWallAABB(enemyAABB, direction, pushDistance, walls)) {
                    if (auto* ai = enemy->GetComponent<EnemyAIComponent>()) {
						std::cout << "[EnemyPushSystem] Enemy at pushed out of bounds or colliding with wall, calling Die()\n";
                        ai->Die();
                    }
                    continue;
                }

                // Only move if in bounds and not colliding with a wall
                enemy->SetLocalPosition(enemyNewPosition);
                if (auto* ai = enemy->GetComponent<EnemyAIComponent>()) {
                    ai->SetPushed(direction, 300.0f);
                }
            }
        }
    }

    bool EnemyPushSystem::WillCollideWithWallAABB(
        const AABB& enemyAABB,
        const glm::vec3& direction,
        float distance,
        const std::vector<std::shared_ptr<GameObject>>& walls) const
    {
        AABB movedAABB = enemyAABB;
        movedAABB.center += glm::vec2(direction.x, direction.y) * distance;

        for (const auto& wall : walls) {
            if (!wall || wall->IsMarkedForDestroy()) continue;
            auto* wallRb = wall->GetComponent<RigidbodyComponent>();
            if (!wallRb) continue;
            if (movedAABB.Intersects(wallRb->GetAABB())) {
                return true;
            }
        }
        return false;
    }

}
