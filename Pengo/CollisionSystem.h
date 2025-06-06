#pragma once
#include "BaseComponent.h"

namespace dae
{
    class CollisionComponent : public BaseComponent
    {
    public:
        CollisionComponent(GameObject* owner)
            : BaseComponent(owner)
        {
        }

        virtual void FixedUpdate(float) override {}
        virtual void Update() override {}
        virtual void LateUpdate() override {}
        virtual void Render() const override {}
    };
}
