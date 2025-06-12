#include "ServiceLocator.h"

std::unique_ptr<ISoundSystem> ServiceLocator::m_SSInstance = nullptr;
std::unique_ptr<dae::ICollisionSystem> ServiceLocator::m_CollisionInstance = nullptr;
std::unique_ptr<dae::InputManager> ServiceLocator::m_InputInstance = nullptr;
std::unique_ptr<dae::XInputManager> ServiceLocator::m_XInputInstance = nullptr;
std::unique_ptr<dae::IFileService> ServiceLocator::m_FileServiceInstance = nullptr;

dae::NullSoundSystem ServiceLocator::s_NullSoundSystem;
dae::NullCollisionSystem ServiceLocator::s_NullCollisionSystem;
dae::NullInputManager ServiceLocator::s_NullInputManager;
dae::NullXInputManager ServiceLocator::s_NullXInputManager;
dae::NullFileService ServiceLocator::s_NullFileService;