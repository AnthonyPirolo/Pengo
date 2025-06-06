#include "PushingState.h"
#include "CharacterComponent.h"
#include "IdleState.h"
#include "DeadState.h"
#include <iostream>

namespace dae
{
    void PushingState::OnEnter(CharacterComponent* character)
    {
        (void)character;
        std::cout << "Entering Pushing State" << std::endl;
    }

    void PushingState::Update(CharacterComponent* character, float deltaTime)
    {
        (void)deltaTime;
		(void)character;
    }

    void PushingState::OnExit(CharacterComponent* character)
    {
        (void)character;
        std::cout << "Exiting Pushing State" << std::endl;
    }
}
