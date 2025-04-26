#pragma once
#include "Observer.h"
#include "SDLSoundSystem.h"
#include "ServiceLocator.h"

namespace dae
{
    class SoundPlayer : public Observer
    {
    public:
        SoundPlayer()
        {
            auto& soundSystem = ServiceLocator::GetSoundSystem();
            std::cout << "Registered Sound System at: " << &ServiceLocator::GetSoundSystem() << std::endl;
            soundSystem.LoadSound("TestSound.wav", 1);
            soundSystem.LoadSound("TestSound2.wav", 2);
        }

        void OnNotify(BaseComponent* entity, Event event) override
        {
            if (event == Event::subjectAttached)
            {
                AddSubject(entity);
            }
            if (event == Event::pointsPickup)
            {
                auto& soundSystem = ServiceLocator::GetSoundSystem();
                soundSystem.Play(1, 1.0f);
            }
            if (event == Event::playerDied)
            {
                auto& soundSystem = ServiceLocator::GetSoundSystem();
                soundSystem.Play(1, 1.0f);
            }
        }

    private:

    };
}
