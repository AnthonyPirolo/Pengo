#include "ServiceLocator.h"

// Define the static member
std::unique_ptr<ISoundSystem> ServiceLocator::m_SSInstance{ nullptr };
