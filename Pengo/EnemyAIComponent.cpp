#include "EnemyAIComponent.h"
#include "GridLogic.h"
#include "GridViewComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "CharacterComponent.h"
#include <chrono>
#include <CollisionSystem.h>

namespace dae {

	EnemyAIComponent::EnemyAIComponent(GameObject* owner, GridLogic* logic, GridViewComponent* view, float tileSize, float moveSpeed)
		: BaseComponent(owner), m_pGridLogic(logic), m_pGridView(view), m_TileSize(tileSize),
		m_MoveSpeed(moveSpeed), m_IsMoving(false), m_TargetPosition(0.0f), m_MoveDirection(0.0f),
		m_PushDirection(0.0f), m_PushSpeed(0.0f), m_CurrentDirection(Direction::None)
	{
		m_RNG.seed(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));
		PickNewDirection();
		m_SpawnPos = GetOwner()->GetWorldPosition();
	}

	void EnemyAIComponent::ResetToSpawn()
	{
		GetOwner()->SetLocalPosition(m_SpawnPos);
		FinishMove();
		PickNewDirection();
		m_State = EnemyState::Roaming;
	}

	void EnemyAIComponent::FixedUpdate(float)
	{
		switch (m_State) {
		case EnemyState::Roaming:
			if (m_IsMoving) {
				float dt = GameTime::GetInstance().GetDeltaTime();
				glm::vec3 pos = GetOwner()->GetWorldPosition();
				glm::vec3 newPos = pos + m_MoveDirection * m_MoveSpeed * dt;

				glm::vec3 prevDelta = m_TargetPosition - pos;
				glm::vec3 currDelta = m_TargetPosition - newPos;

				if (glm::dot(prevDelta, currDelta) <= 0.0f) {
					GetOwner()->SetLocalPosition(m_TargetPosition);
					FinishMove();
				}
				else {
					GetOwner()->SetLocalPosition(newPos);
				}
			}
			else {
				AttemptStep();
			}
			break;

		case EnemyState::BeingPushed:
			HandlePushedMovement();
			break;

		case EnemyState::Dead:
			break;
		}
	}

	void EnemyAIComponent::AttemptStep()
	{
		glm::vec3 pos = GetOwner()->GetWorldPosition();
		glm::vec3 topLeft = pos - glm::vec3(m_TileSize / 2.0f);
		int currX, currY;
		m_pGridLogic->WorldToGrid(topLeft, currX, currY);

		PickNewDirection();
		glm::ivec2 offset = DirectionToOffset(m_CurrentDirection);
		int nextX = currX + offset.x;
		int nextY = currY + offset.y;

		if (!m_pGridLogic->GetModel()->IsInBounds(nextX, nextY))
			return;

		TileType tile = m_pGridView->GetTileType(nextX, nextY);
		if (tile == TileType::Wall) {
			HandleWallPush(nextX, nextY);
			return;
		}
		if (tile == TileType::Enemy)
			return;

		StartMoveTo(nextX, nextY);
	}

	void EnemyAIComponent::HandleWallPush(int wallX, int wallY)
	{
		glm::vec3 wallTopLeft = m_pGridLogic->GridToWorld(wallX, wallY);
		int newX = -1, newY = -1;

		if (m_pGridLogic->SlideOrBreakAt(wallTopLeft, DirectionToOffset(m_CurrentDirection).x, DirectionToOffset(m_CurrentDirection).y, newX, newY)) {
			m_pGridView->OnWallBroken(wallX, wallY);
			StartMoveTo(wallX, wallY);
		}
	}

	void EnemyAIComponent::HandlePushedMovement()
	{
		float dt = GameTime::GetInstance().GetDeltaTime();
		glm::vec3 pos = GetOwner()->GetWorldPosition();
		glm::vec3 newPos = pos + m_PushDirection * m_PushSpeed * dt;

		GetOwner()->SetLocalPosition(newPos);

		int x, y;
		m_pGridLogic->WorldToGrid(newPos, x, y);
		if (m_pGridLogic->GetModel()->HasWall(x, y) || IsOutOfBounds())
		{
			Die();
		}
	}

	void EnemyAIComponent::StartMoveTo(int gx, int gy)
	{
		glm::vec3 center = m_pGridLogic->GridToWorld(gx, gy) + glm::vec3(m_TileSize / 2.0f);
		glm::vec3 from = GetOwner()->GetWorldPosition();
		glm::vec3 dir = glm::normalize(center - from);

		m_TargetPosition = center;
		m_MoveDirection = dir;
		m_IsMoving = true;

		if (auto* charComp = GetOwner()->GetComponent<CharacterComponent>())
			charComp->SetMoving(true);
	}

	void EnemyAIComponent::FinishMove()
	{
		m_IsMoving = false;
		m_MoveDirection = glm::vec3(0.0f);

		if (auto* charComp = GetOwner()->GetComponent<CharacterComponent>())
			charComp->SetMoving(false);
	}

	void EnemyAIComponent::PickNewDirection()
	{
		static const std::vector<Direction> dirs = { Direction::Up, Direction::Down, Direction::Left, Direction::Right };
		std::vector<Direction> shuffled = dirs;
		std::shuffle(shuffled.begin(), shuffled.end(), m_RNG);
		m_CurrentDirection = shuffled.front();
	}

	bool EnemyAIComponent::IsOutOfBounds() const
	{
		int x, y;
		m_pGridLogic->WorldToGrid(GetOwner()->GetWorldPosition(), x, y);
		auto* model = m_pGridLogic->GetModel();
		return !model->IsInBounds(x, y);
	}

	void EnemyAIComponent::Die()
	{
		if (m_State == EnemyState::Dead)
			return;

		m_State = EnemyState::Dead;
		Notify(Observer::Event::EnemyDied);
		auto* colComp = GetOwner()->GetComponent<CollisionComponent>();
		ServiceLocator::GetCollisionSystem().Unregister(colComp);
		GetOwner()->MarkForDestroy();
		if (m_pGridView) {
			m_pGridView->HatchNextEgg();
		}
	}

	void EnemyAIComponent::SetPushed(const glm::vec3& dir, float speed)
	{
		m_State = EnemyState::BeingPushed;
		m_PushDirection = dir;
		m_PushSpeed = speed;
	}

	glm::ivec2 EnemyAIComponent::DirectionToOffset(Direction dir) const
	{
		switch (dir) {
		case Direction::Up: return { 0, -1 };
		case Direction::Down: return { 0, 1 };
		case Direction::Left: return { -1, 0 };
		case Direction::Right: return { 1, 0 };
		default: return { 0, 0 };
		}
	}
}
