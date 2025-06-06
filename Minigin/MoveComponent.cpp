#include "MoveComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include <glm.hpp>
#include <iostream>

namespace dae
{
    MoveComponent::MoveComponent(GameObject* owner, float maxMovementSpeed)
        : BaseComponent(owner)
        , m_MaxMovementSpeed(maxMovementSpeed)
        , m_Direction(0.0f)
        , m_IsFreeform(false)
        , m_IsMovingToTarget(false)
        , m_TargetPosition(0.0f)
    {
    }

    void MoveComponent::UpdateDirection(const glm::vec3& direction)
    {
        if (glm::length(direction) < 0.1f)
        {
            m_Direction = glm::vec3(0.0f);
            m_IsFreeform = false;
            return;
        }

        m_Direction = glm::normalize(direction) * m_MaxMovementSpeed;
        m_IsFreeform = true;
        m_IsMovingToTarget = false;
    }

    void MoveComponent::SetTarget(const glm::vec3& targetPos)
    {
        m_TargetPosition = targetPos;
        m_IsMovingToTarget = true;
        m_IsFreeform = false;

        const glm::vec3 currentPos = GetOwner()->GetWorldPosition();
        const glm::vec3 delta = m_TargetPosition - currentPos;

        if (glm::length(delta) < 0.1f)
        {
            m_IsMovingToTarget = false;
            m_Direction = glm::vec3(0.0f);
            return;
        }

        m_Direction = glm::normalize(glm::vec3(delta.x, delta.y, 0.0f)) * m_MaxMovementSpeed;
    }

    void MoveComponent::Update()
    {
        float deltaTime = GameTime::GetInstance().GetDeltaTime();
        auto* owner = GetOwner();
        glm::vec3 currentPos = owner->GetWorldPosition();

        if (m_IsMovingToTarget)
        {
            glm::vec3 newPos = currentPos + m_Direction * deltaTime;
            glm::vec3 toTargetPrev = m_TargetPosition - currentPos;
            glm::vec3 toTargetNow = m_TargetPosition - newPos;

            if (glm::dot(toTargetPrev, toTargetNow) <= 0.0f)
            {
                owner->SetLocalPosition(m_TargetPosition);
                m_IsMovingToTarget = false;
                m_Direction = glm::vec3(0.0f);
            }
            else
            {
                owner->SetLocalPosition(newPos);
            }
            return;
        }

        if (m_IsFreeform)
        {
            glm::vec3 newPos = currentPos + m_Direction * deltaTime;
            owner->SetLocalPosition(newPos);
        }
    }
}
