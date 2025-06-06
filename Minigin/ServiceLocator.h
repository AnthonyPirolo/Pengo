#pragma once
#include <memory>
#include "ISoundSystem.h"
#include <iostream>
#include "ICollisionSystem.h"

class ServiceLocator final
{
class ICollisionSystem;
public:
	static ISoundSystem& GetSoundSystem() { return *m_SSInstance; }

	static dae::ICollisionSystem& GetCollisionSystem()
	{
		return *m_CollisionInstance;
	}

	static void RegisterCollisionSystem(std::unique_ptr<dae::ICollisionSystem>&& cs)
	{
		if (m_CollisionInstance)
		{
			std::cerr << "[ServiceLocator] ERROR: CollisionSystem already registered!\n";
			throw std::runtime_error("CollisionSystem already registered");
		}
		m_CollisionInstance = std::move(cs);
	}

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
	static std::unique_ptr<dae::ICollisionSystem> m_CollisionInstance;
};