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
            soundSystem.LoadSound("TestSound.wav", 2);
            soundSystem.LoadSound("TestSound2.wav", 3);
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
                soundSystem.Play(2, 1.0f);
            }
            if (event == Event::playerDied)
            {
                auto& soundSystem = ServiceLocator::GetSoundSystem();
				soundSystem.StopMusic();
                soundSystem.Play(3, 1.0f);
            }
			if (event == Event::play)
			{
				auto& soundSystem = ServiceLocator::GetSoundSystem();
				soundSystem.PlayMusic("PengoMain.ogg", 0.5f, true);
			}
        }

    private:

    };
}
