#pragma once
#include "Singleton.h"

namespace dae
{
	class GameTime final : public Singleton<GameTime>
	{
	public:
		void SetDeltaTime(float deltaTime) { m_DeltaTime = deltaTime; }
		float GetDeltaTime() const { return m_DeltaTime; }

	private:
		friend class Singleton<GameTime>;
		GameTime() = default;
		float m_DeltaTime{ 0.0f };
	};
}

