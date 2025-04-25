#pragma once
#include <memory>
#include "ISoundSystem.h"

class ServiceLocator final
{
public:
	static ISoundSystem& GetSoundSystem() { return *m_SSInstance; }

	static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& ss)
	{
		m_SSInstance = std::move(ss);
	}

private:
	static std::unique_ptr<ISoundSystem> m_SSInstance;
};