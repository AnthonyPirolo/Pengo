#pragma once

#include "BaseComponent.h"

namespace dae {
	class GameObject;
	class GridViewComponent;

	class WallComponent : public BaseComponent {
	public:
		enum class State {
			Idle,
			BeingBroken,
			Sliding,
			Broken
		};

		WallComponent(GameObject* owner, GridViewComponent* view, int gridX, int gridY);

		void FixedUpdate(float deltaTime) override;
		void Update() override {}	
		void LateUpdate() override {}
		void Render() const override {}

		void SetHasEgg(bool hasEgg);
		bool HasEgg() const;

		void SetGridPosition(int x, int y);
		void SetBreaker(GameObject* breaker);

		State m_State;

		glm::ivec2 m_PushDirection{};

	private:
		State m_PrevState = State::Idle;
		bool m_HasEgg = false;
		GameObject* m_CurrentBreaker = nullptr;

		float m_BreakTimer = 0.0f;
		const float m_BreakDuration = 3.0f;

		GridViewComponent* m_pGridView = nullptr;
		int m_GridX = -1;
		int m_GridY = -1;

		bool m_DestroyAfterSlide;
	};
}
