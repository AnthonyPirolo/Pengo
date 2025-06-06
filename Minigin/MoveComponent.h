#pragma once

#include "BaseComponent.h"
#include <glm.hpp>

namespace dae
{
    class MoveComponent : public BaseComponent
    {
    public:
        explicit MoveComponent(GameObject* owner, float maxMovementSpeed);

        void Update() override;
        void FixedUpdate(float) override {}
        void LateUpdate() override {}
        void Render() const override {}

        void UpdateDirection(const glm::vec3& direction);
        void SetTarget(const glm::vec3& targetPos);

        bool IsMovingToTarget() const { return m_IsMovingToTarget; }
        glm::vec3 GetDirection() const { return m_Direction; }

    private:
        float m_MaxMovementSpeed;
        glm::vec3 m_Direction;

        bool m_IsFreeform;
        bool m_IsMovingToTarget;

        glm::vec3 m_TargetPosition;
    };
}