#pragma once
#include "BaseComponent.h"
#include "BaseState.h"
#include "GameTime.h"

namespace dae
{
    class StateComponent final : public BaseComponent
    {
    public:
        StateComponent(GameObject* owner, BaseState* state)
            : BaseComponent{ owner }
            , m_State{ state }
        {
        }

        void Update() override
        {
            if (m_State)
                m_State->Update(dae::GameTime::GetInstance().GetDeltaTime());
        }

        void Render() const override
        {
            if (m_State)
                m_State->Render();
        }

        void FixedUpdate(float deltaTime) override
        {
            (void)deltaTime;
        }

        void LateUpdate() override
        {
        }

    private:
        BaseState* m_State;
    };
}