#pragma once
#include "IdleState.h"
#include "CharacterComponent.h"
#include "RunningState.h"
#include "PushingState.h"
#include "DeadState.h"
#include <iostream>

namespace dae
{
    void IdleState::OnEnter(CharacterComponent* character)
    {
		(void)character;
        std::cout << "Entering Idle State" << std::endl;
		std::cout << "Idle" << std::endl;
		//Should change sprites in here for example
    }

    void IdleState::Update(CharacterComponent* character, float deltaTime)
    {
		(void)deltaTime;
        if (character->IsMoving())
        {
            character->SetState(new RunningState());
        }
        else if (false)
        {
            character->SetState(new PushingState());
        }
        else if (false)
        {
            character->SetState(new DeadState());
        }
    }

    void IdleState::OnExit(CharacterComponent* character)
    {
		(void)character;
        std::cout << "Exiting Idle State" << std::endl;
    }
}
