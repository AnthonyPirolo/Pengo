#pragma once
#include "RunningState.h"
#include "CharacterComponent.h"
#include "IdleState.h"
#include "DeadState.h"
#include "PushingState.h"
#include <iostream>

namespace dae
{
    void RunningState::OnEnter(CharacterComponent* character)
    {
		(void)character;
        std::cout << "Entering Running State" << std::endl;
		std::cout << "Running" << std::endl;
		//Should change sprites in here for example
    }

    void RunningState::Update(CharacterComponent* character, float deltaTime)
    {
        (void)deltaTime;
        if (!character->IsMoving())
        {
            character->SetState(new IdleState());
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

    void RunningState::OnExit(CharacterComponent* character)
    {
		(void)character;
        std::cout << "Exiting Running State" << std::endl;
    }
}
