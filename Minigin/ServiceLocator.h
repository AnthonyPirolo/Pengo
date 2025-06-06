#pragma once
#include <memory>
#include "ISoundSystem.h"
#include <iostream>

class ServiceLocator final
{
public:
	static ISoundSystem& GetSoundSystem() { return *m_SSInstance; }

	static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& ss)
	{
		if (m_SSInstance)
		{
			std::cerr << "[ServiceLocator] ERROR: SoundSystem is already registered!\n";
			throw std::runtime_error("SoundSystem already registered");
		}
		m_SSInstance = std::move(ss);
	}



private:
	static std::unique_ptr<ISoundSystem> m_SSInstance;
};