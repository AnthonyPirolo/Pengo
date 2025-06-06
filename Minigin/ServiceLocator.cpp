#include "ServiceLocator.h"
#include "ICollisionSystem.h"

// Define the static member

	std::unique_ptr<dae::ICollisionSystem> ServiceLocator::m_CollisionInstance = nullptr;
	std::unique_ptr<ISoundSystem> ServiceLocator::m_SSInstance{ nullptr };
