#pragma once

#include "BaseComponent.h"
#include <memory>
#include <glm.hpp>

namespace dae {
	class GridLogic;
	class GridViewComponent;
	class PlayerCollisionListener;

	class PlayerComponent : public BaseComponent {
	public:
		PlayerComponent(GameObject* owner, GridLogic* logic, GridViewComponent* view, float tileSize, float moveSpeed);

		void Update() override;
		void SetDesiredDirection(int dx, int dy);

		void ResetToStart();
		bool IsAlive() const { return m_IsAlive; }

		void FixedUpdate(float deltaTime) override { deltaTime; }
		void Render() const override {}
		void LateUpdate() override {}
	private:
		void AttemptStep();
		void MoveTowardsTarget();
		void HandleWallPush(int wallX, int wallY);
		void StartMoveTo(int gx, int gy);
		void FinishMove();

		GridLogic* m_pLogic = nullptr;
		GridViewComponent* m_pView = nullptr;

		float m_TileSize;
		float m_MoveSpeed;
		glm::vec3 m_TargetPosition;
		glm::vec3 m_MoveDirection;

		int m_PendingDX;
		int m_PendingDY;

		bool m_IsMoving;
		bool m_IsAlive;
		float m_DeathTimer;
		glm::vec3 m_SpawnPosition;

		std::shared_ptr<PlayerCollisionListener> m_Listener;
	};
}
