#pragma once

#include "BaseComponent.h"
#include <glm.hpp>
#include <random>
#include "GridViewComponent.h"

namespace dae {
	class GridLogic;
	class GridViewComponent;

	class EnemyAIComponent : public BaseComponent {
	public:
		enum class Direction {
			Up,
			Down,
			Left,
			Right,
			None
		};

		enum class EnemyState {
			Roaming,
			BeingPushed,
			Dead
		};

		EnemyAIComponent(GameObject* owner, GridLogic* logic, GridViewComponent* view, float tileSize, float moveSpeed);

		void FixedUpdate(float deltaTime) override;
		void Update() override {}
		void LateUpdate() override {}
		void Render() const override {}

		void ResetToSpawn();
		void Die();
		void SetPushed(const glm::vec3& dir, float speed);

		bool IsOutOfBounds() const;

	private:
		void AttemptStep();
		void HandleWallPush(int wallX, int wallY);
		void HandlePushedMovement();

		void StartMoveTo(int gx, int gy);
		void FinishMove();
		void PickNewDirection();

		glm::ivec2 DirectionToOffset(Direction dir) const;

		GridLogic* m_pGridLogic;
		GridViewComponent* m_pGridView;

		glm::vec3 m_MoveDirection;
		glm::vec3 m_TargetPosition;

		glm::vec3 m_PushDirection;
		float m_PushSpeed;

		float m_TileSize;
		float m_MoveSpeed;
		bool m_IsMoving;

		glm::vec3 m_SpawnPos;

		std::default_random_engine m_RNG;
		Direction m_CurrentDirection;
		EnemyState m_State = EnemyState::Roaming;
	};
}
