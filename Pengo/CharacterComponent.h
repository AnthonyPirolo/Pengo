#pragma once

#include "BaseComponent.h"
#include "CharacterState.h"
#include "GameTime.h"
#include "IdleState.h"

namespace dae
{
    class CharacterComponent : public BaseComponent
    {
    public:
        explicit CharacterComponent(GameObject* owner)
            : BaseComponent(owner)
            , m_CurrentState(new IdleState())
        {
            if (m_CurrentState)
                m_CurrentState->OnEnter(this);
        }

        ~CharacterComponent() override
        {
            delete m_CurrentState;
            m_CurrentState = nullptr;
        }

        void SetMoving(bool moving) { m_IsMoving = moving; }
        bool IsMoving() const { return m_IsMoving; }

        void Update() override
        {
            if (m_CurrentState)
            {
                const float dt = GameTime::GetInstance().GetDeltaTime();
                m_CurrentState->Update(this, dt);
            }

            m_IsMoving = false;
        }

        void SetState(CharacterState* newState)
        {
            if (m_CurrentState)
            {
                m_CurrentState->OnExit(this);
                delete m_CurrentState;
            }

            m_CurrentState = newState;

            if (m_CurrentState)
                m_CurrentState->OnEnter(this);
        }

        void FixedUpdate(float) override {}
        void LateUpdate() override {}
        void Render() const override {}

    private:
        CharacterState* m_CurrentState{ nullptr };
        bool m_IsMoving{ false };
    };
}
