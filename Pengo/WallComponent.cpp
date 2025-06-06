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

    void WallComponent::FixedUpdate(float deltaTime)
    {
        switch (m_State) {
        case State::BeingBroken:
            if (!m_CurrentBreaker) return;
            m_BreakTimer += deltaTime;
            if (m_BreakTimer >= m_BreakDuration) {
                m_DestroyAfterSlide = true;
                m_State = State::Broken;
            }
            break;

        case State::Sliding: {
            if (!m_pGridView) return;
            auto* moveComponent = GetOwner()->GetComponent<MoveComponent>();
            if (moveComponent && !moveComponent->IsMovingToTarget()) {
                m_State = State::Idle;
                if (m_DestroyAfterSlide && m_pGridView) {
                    m_pGridView->OnWallBroken(m_GridX, m_GridY);
                    m_DestroyAfterSlide = false;
                }
                return;
            }
            auto* wallRigidbody = GetOwner()->GetComponent<RigidbodyComponent>();
            if (!wallRigidbody) return;

            glm::vec3 direction = glm::normalize(glm::vec3(m_PushDirection, 0.0f));
            float pushDistance = 300.0f * deltaTime;
            AABB wallAABB = wallRigidbody->GetAABB();
            wallAABB.center += glm::vec2(direction.x, direction.y) * pushDistance;

            EnemyPushSystem pushSystem(m_pGridView);
            pushSystem.HandleEnemyPush(direction, pushDistance, wallAABB);

            break;
        }
        case State::Broken:
        case State::Idle:
        default:
            break;
        }
    }
}

