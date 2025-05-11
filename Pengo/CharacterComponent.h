#pragma once
#include "BaseComponent.h"
#include "CharacterState.h"
#include "GameTime.h"
#include "IdleState.h"

namespace dae
{
    class CharacterState;

    class CharacterComponent : public BaseComponent
    {
    public:
        CharacterComponent(GameObject* owner)
            : BaseComponent(owner), m_CurrentState(new IdleState()), m_IsMoving(false)
        {
        }

        ~CharacterComponent() override
        {
            delete m_CurrentState;
        }

        void SetMoving(bool moving) { m_IsMoving = moving; }
        bool IsMoving() const { return m_IsMoving; }

        void Update() override
        {
            if (m_CurrentState)
                m_CurrentState->Update(this, dae::GameTime::GetInstance().GetDeltaTime());

            // Reset movement flag so it's only true for the frame movement happens
            m_IsMoving = false;
        }

        void SetState(CharacterState* newState)
        {
            if (m_CurrentState)
                m_CurrentState->OnExit(this);

            delete m_CurrentState;
            m_CurrentState = newState;

            if (m_CurrentState)
                m_CurrentState->OnEnter(this);
        }

		void FixedUpdate(float) override {}
		void LateUpdate() override {}
		void Render() const override
		{
			// Render logic for the character can be added here
		}

    private:
        CharacterState* m_CurrentState;
        bool m_IsMoving;
    };

}
