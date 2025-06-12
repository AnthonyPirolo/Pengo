#include "WallComponent.h"
#include "GameTime.h"
#include "GridViewComponent.h"
#include "GameObject.h"
#include "EnemyAIComponent.h"
#include "iostream"
#include <MoveComponent.h>
#include <CollisionSystem.h>
#include "RigidbodyComponent.h"
#include "EnemyPushSystem.h"

namespace dae {

    WallComponent::WallComponent(GameObject* owner, GridViewComponent* view, int gridX, int gridY)
        : BaseComponent(owner), m_pGridView(view), m_GridX(gridX), m_GridY(gridY), m_State(State::Idle)
    {
    }

    void WallComponent::SetHasEgg(bool hasEgg) {
        m_HasEgg = hasEgg;
    }

    bool WallComponent::HasEgg() const {
        return m_HasEgg;
    }

    void WallComponent::SetGridPosition(int x, int y) {
        m_GridX = x;
        m_GridY = y;
    }

    void WallComponent::SetBreaker(GameObject* breaker) {
        m_CurrentBreaker = breaker;
        m_State = State::BeingBroken;
        m_BreakTimer = 0.0f;
    }

    void WallComponent::FixedUpdate(float deltaTime) {
        switch (m_State) {
        case State::BeingBroken:
            if (!m_CurrentBreaker) return;
            m_BreakTimer += deltaTime;
            if (m_BreakTimer >= m_BreakDuration) {
                m_DestroyAfterSlide = true;
                m_State = State::Broken;
            }
            break;

        case State::Sliding:
        {
            if (!m_pGridView) return;

            glm::vec3 currentPos = GetOwner()->GetWorldPosition();
            glm::vec3 targetPos = m_pGridView->m_Logic.GridToWorld(m_GridX, m_GridY) + glm::vec3(m_pGridView->m_TileSize / 2.0f);
            glm::vec3 direction = glm::normalize(targetPos - currentPos);
            float moveSpeed = 300.0f * deltaTime;

            auto* wallRigidbody = GetOwner()->GetComponent<RigidbodyComponent>();
            if (!wallRigidbody) return;

            float pushDistance = 300.0f * deltaTime;
            AABB wallAABB = wallRigidbody->GetAABB();
            wallAABB.center += glm::vec2(direction.x, direction.y) * pushDistance;

            EnemyPushSystem pushSystem(m_pGridView);
            pushSystem.HandleEnemyPush(direction, pushDistance, wallAABB);

            if (glm::length(targetPos - currentPos) <= moveSpeed) {
                GetOwner()->SetLocalPosition(targetPos);
                m_State = State::Idle;

                if (m_pGridView) {
                    m_pGridView->m_Model.SetWall(m_GridX, m_GridY);
                }

                if (m_DestroyAfterSlide && m_pGridView) {
                    m_pGridView->OnWallBroken(m_GridX, m_GridY);
                    m_DestroyAfterSlide = false;
                }
            }
            else {
                GetOwner()->SetLocalPosition(currentPos + direction * moveSpeed);
            }
            break;

        }
        case State::Broken:
			return;
            break;
        case State::Idle:
            return;
            break;
        default:
            return;
            break;
        }
    }
}
