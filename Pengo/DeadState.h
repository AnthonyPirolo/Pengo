#pragma once
#include "CharacterState.h"

namespace dae
{
    class DeadState : public CharacterState
    {
    public:
        void OnEnter(CharacterComponent* character) override;
        void Update(CharacterComponent* character, float deltaTime) override;
        void OnExit(CharacterComponent* character) override;
    };
}
