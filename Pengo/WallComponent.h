#pragma once
#include "BaseComponent.h"
#include <memory>

namespace dae
{
	class GridViewComponent;
	class GameObject;

	class WallComponent : public BaseComponent
	{
	public:
		enum class State
		{
			Idle,
			BeingBroken,
			Broken
		};

		WallComponent(GameObject* owner);
		void Update() override;

		void Initialize(GridViewComponent* view, int gridX, int gridY);
		void SetHasEgg(bool hasEgg);
		bool HasEgg() const;

		void BeginBreaking(GameObject* breaker);
		void CancelBreaking(GameObject* breaker);

	private:
		State m_State;
		bool m_HasEgg;

		GameObject* m_CurrentBreaker;
		float m_BreakTimer;
		const float m_BreakDuration = 3.0f;

		GridViewComponent* m_pGridView;
		int m_GridX, m_GridY;
	};
}
