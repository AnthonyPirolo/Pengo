#include "EnemyAIComponent.h"
#include "GridLogic.h"
#include "GridViewComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "CharacterComponent.h"
#include <chrono>
#include <iostream>
#include "GameManager.h"

namespace dae
{
	EnemyAIComponent::EnemyAIComponent(GameObject* owner, GridLogic* logic, GridViewComponent* view, float tileSize, float moveSpeed)
		: BaseComponent(owner)
		, m_pGridLogic(logic)
		, m_pGridView(view)
		, m_TileSize(tileSize)
		, m_MoveSpeed(moveSpeed)
		, m_IsMoving(false)
		, m_TargetPosition(0.0f)
		, m_MoveDirection(0.0f)
		, m_CurrentDirection(Direction::None)
		, m_RNG(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()))
	{
		PickNewDirection();

		m_SpawnPos = GetOwner()->GetWorldPosition();

	}

	void EnemyAIComponent::ResetToSpawn()
	{
		if (auto* ai = GetOwner()->GetComponent<EnemyAIComponent>())
		{
			ai->FinishMove();
			ai->PickNewDirection();
		}
		GetOwner()->SetLocalPosition(m_SpawnPos);
	}

	void EnemyAIComponent::FixedUpdate(float)
	{
		switch (m_State)
		{
		case EnemyState::Roaming:
		if (m_IsMoving)
		{
			float dt = GameTime::GetInstance().GetDeltaTime();
			glm::vec3 pos = GetOwner()->GetWorldPosition();
			glm::vec3 newPos = pos + m_MoveDirection * m_MoveSpeed * dt;

			glm::vec3 prevDelta = m_TargetPosition - pos;
			glm::vec3 currDelta = m_TargetPosition - newPos;

			if (glm::dot(prevDelta, currDelta) <= 0.0f)
			{
				GetOwner()->SetLocalPosition(m_TargetPosition);
				FinishMove();
			}
			else
			{
				GetOwner()->SetLocalPosition(newPos);
			}
			return;
		}
		else { AttemptStep(); }
		break;
		case EnemyState::BeingPushed:
			HandlePushedMovement();
			break;

		case EnemyState::Dead:
			break;
		}
	}

	void EnemyAIComponent::HandlePushedMovement()
	{
		float dt = GameTime::GetInstance().GetDeltaTime();
		glm::vec3 pos = GetOwner()->GetWorldPosition();
		glm::vec3 newPos = pos + m_PushDirection * m_PushSpeed * dt;

		GetOwner()->SetLocalPosition(newPos);

		// Check if we've reached destination
		int x, y;
		m_pGridLogic->WorldToGrid(newPos, x, y);
		if (m_pGridLogic->GetModel()->HasWall(x, y))
		{
			Die(); // Kill enemy
		}
	}

	void EnemyAIComponent::Die()
	{
		if (m_State == EnemyState::Dead)
			return;

		m_State = EnemyState::Dead;
		GetOwner()->MarkForDestroy(); // Or play animation and delay
		// You can also notify observers here
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

		int gridW = m_pGridLogic->GetModel()->GetWidth();
		int gridH = m_pGridLogic->GetModel()->GetHeight();

		if (nextX < 0 || nextX >= gridW || nextY < 0 || nextY >= gridH)
			return;

		TileType nextTile = m_pGridView->GetTileType(nextX, nextY);
		if (nextTile == TileType::Wall)
		{
			HandleWallPush(nextX, nextY);
			return;
		}
		if (nextTile == TileType::Enemy)
		{
			return;
		}
		StartMoveTo(nextX, nextY);
	}

	void EnemyAIComponent::HandleWallPush(int wallX, int wallY)
	{
		glm::vec3 wallTopLeft = m_pGridLogic->GridToWorld(wallX, wallY);
		int newX = -1, newY = -1;

		bool pushed = m_pGridLogic->SlideOrBreakAt(wallTopLeft, DirectionToOffset(m_CurrentDirection).x, DirectionToOffset(m_CurrentDirection).y, newX, newY);
		if (pushed)
		{
			m_pGridView->OnWallBroken(wallX, wallY);
			StartMoveTo(wallX, wallY);
		}
	}

	void EnemyAIComponent::StartMoveTo(int gx, int gy)
	{
		glm::vec3 center = m_pGridLogic->GridToWorld(gx, gy) + glm::vec3(m_TileSize / 2.0f, m_TileSize / 2.0f, 0.0f);
		glm::vec3 from = GetOwner()->GetWorldPosition();
		glm::vec3 dir = glm::normalize(center - from);

		m_TargetPosition = center;
		m_MoveDirection = dir;
		m_IsMoving = true;

		if (auto* character = GetOwner()->GetComponent<CharacterComponent>())
			character->SetMoving(true);
	}

	void EnemyAIComponent::FinishMove()
	{
		m_IsMoving = false;
		m_MoveDirection = glm::vec3(0.0f);

		if (auto* character = GetOwner()->GetComponent<CharacterComponent>())
			character->SetMoving(false);
	}

	void EnemyAIComponent::PickNewDirection()
	{
		static const std::vector<Direction> dirs = { Direction::Up, Direction::Down, Direction::Left, Direction::Right };
		std::vector<Direction> shuffled = dirs;
		std::shuffle(shuffled.begin(), shuffled.end(), m_RNG);
		m_CurrentDirection = shuffled.front();
	}

	glm::ivec2 EnemyAIComponent::DirectionToOffset(Direction dir) const
	{
		switch (dir)
		{
		case Direction::Up: return { 0, -1 };
		case Direction::Down: return { 0, 1 };
		case Direction::Left: return { -1, 0 };
		case Direction::Right: return { 1, 0 };
		default: return { 0, 0 };
		}
	}
}