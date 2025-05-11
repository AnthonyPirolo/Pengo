#pragma once

namespace dae
{
    class CharacterComponent;

    class CharacterState
    {
    public:
        virtual ~CharacterState() = default;
        virtual void OnEnter(CharacterComponent* character) = 0;
        virtual void Update(CharacterComponent* character, float deltaTime) = 0;
        virtual void OnExit(CharacterComponent* character) = 0;
    };
}
