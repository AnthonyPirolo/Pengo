#pragma once

#include "BaseComponent.h"

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

    };
}
