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
            soundSystem.LoadSound("PlayerDie.wav", 2);
            soundSystem.LoadSound("EnemyDie.wav", 3);
        }

        void OnNotify(BaseComponent* entity, Event event) override
        {
            auto& soundSystem = ServiceLocator::GetSoundSystem();

            if (event == Event::subjectAttached)
            {
                AddSubject(entity);
            }
            if (event == Event::playerDied)
            {
                soundSystem.Play(2, 0.5f);
            }
            if (event == Event::EnemyDied)
            {
                soundSystem.Play(3, 0.5f);
            }
            if (event == Event::play)
            {
                soundSystem.PlayMusic("PengoMain.ogg", 0.5f, true);
            }
        }

    private:

    };
}