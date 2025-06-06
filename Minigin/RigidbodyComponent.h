#pragma once

#include "BaseComponent.h"
#include "ICollisionSystem.h"


namespace dae
{
    class RigidbodyComponent final : public BaseComponent
    {
    public:
        RigidbodyComponent(GameObject* owner, int width, int height);

        virtual void Update() override {}
        virtual void FixedUpdate(float) override {}
        virtual void LateUpdate() override {}
        virtual void Render() const override {}

        const int m_Width;
        const int m_Height;

        AABB GetAABB() const
        {
            glm::vec3 center3D = GetOwner()->GetWorldPosition();
            glm::vec2 center{ center3D.x, center3D.y };
            glm::vec2 halfExtents{ m_Width * 0.5f, m_Height * 0.5f };
            return { center, halfExtents };
        }

    };
}
