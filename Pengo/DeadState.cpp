#include "DeadState.h"
#include "CharacterComponent.h"
#include <iostream>

namespace dae
{
    void DeadState::OnEnter(CharacterComponent* character)
    {
        (void)character;
        std::cout << "Entering Dead State" << std::endl;
    }

    void DeadState::Update(CharacterComponent* character, float deltaTime)
    {
        (void)deltaTime;
        (void)character;
        // No transitions from DeadState
    }

    void DeadState::OnExit(CharacterComponent* character)
    {
        (void)character;
        std::cout << "Exiting Dead State" << std::endl;
    }
}
