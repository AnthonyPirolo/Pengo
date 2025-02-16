#pragma once
#include "Singleton.h"

namespace dae
{
	class Time final : public Singleton<Time>
	{
	public:
		void SetDeltaTime(float deltaTime) { m_DeltaTime = deltaTime; }
		float GetDeltaTime() const { return m_DeltaTime; }

	private:
		friend class Singleton<Time>;
		Time() = default;
		float m_DeltaTime{ 0.0f };
	};
}

