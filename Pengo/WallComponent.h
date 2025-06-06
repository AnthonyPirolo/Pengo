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
			Sliding,
			Broken
		};

		WallComponent(GameObject* owner, GridViewComponent* view, int gridX, int gridY);
		void Update() override {};
		void FixedUpdate(float deltaTime) override;
		void LateUpdate() override{}
		void Render() const override {}

		void SetHasEgg(bool hasEgg);
		bool HasEgg() const;

		void SetGridPosition(int x, int y)
		{
			m_GridX = x;
			m_GridY = y;
		}


		State m_State;
	private:
		
		bool m_HasEgg;

		GameObject* m_CurrentBreaker;
		float m_BreakTimer;
		const float m_BreakDuration = 3.0f;

		GridViewComponent* m_pGridView;
		int m_GridX;
		int m_GridY;
	};
}
