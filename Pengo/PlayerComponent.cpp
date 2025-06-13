#include "PlayerComponent.h"
#include "GameObject.h"
#include "GridLogic.h"
#include "GridViewComponent.h"
#include "CollisionComponent.h"
#include "GameTime.h"
#include "CharacterComponent.h"
#include "PlayerCollisionListener.h"
#include <iostream>

namespace dae {

	PlayerComponent::PlayerComponent(GameObject* owner, GridLogic* logic, GridViewComponent* view, float tileSize, float moveSpeed)
		: BaseComponent(owner), m_pLogic(logic), m_pView(view), m_TileSize(tileSize), m_MoveSpeed(moveSpeed),
		m_IsMoving(false), m_TargetPosition(0.0f), m_MoveDirection(0.0f),
		m_PendingDX(0), m_PendingDY(0), m_IsAlive(true), m_DeathTimer(0.0f)
	{
		glm::vec3 worldPos = owner->GetWorldPosition();
		glm::vec3 topLeft = worldPos - glm::vec3(tileSize / 2.0f);
		int gx, gy;
		m_pLogic->WorldToGrid(topLeft, gx, gy);
		glm::vec3 center = m_pLogic->GridToWorld(gx, gy) + glm::vec3(tileSize / 2.0f);
		owner->SetLocalPosition(center);
		m_SpawnPosition = center;

		m_Listener = std::make_shared<PlayerCollisionListener>(this);

		if (auto col = owner->GetComponent<CollisionComponent>()) {
			col->m_Tag = CollisionTag::Player;
		}
	}

	void PlayerComponent::SetDesiredDirection(int dx, int dy) {
		if (!m_IsMoving && (dx != 0 || dy != 0)) {
			m_PendingDX = dx;
			m_PendingDY = dy;
		}
	}

	void PlayerComponent::Update() {
		if (m_IsMoving) {
			MoveTowardsTarget();
			return;
		}

		if (m_PendingDX == 0 && m_PendingDY == 0)
			return;

		AttemptStep();
	}

	void PlayerComponent::Die(float delay) {
		if (!m_IsAlive) return;
		m_IsAlive = false;
		m_DeathTimer = delay;

	}

	void PlayerComponent::ResetToStart() {
		m_IsAlive = true;
		m_DeathTimer = 0.0f;
		m_IsMoving = false;
		m_PendingDX = m_PendingDY = 0;
		m_MoveDirection = glm::vec3(0.0f);
		if (GetOwner())
			GetOwner()->SetLocalPosition(m_SpawnPosition);
	}

	void PlayerComponent::MoveTowardsTarget() {
		float dt = GameTime::GetInstance().GetDeltaTime();
		glm::vec3 pos = GetOwner()->GetWorldPosition();
		glm::vec3 nextPos = pos + m_MoveDirection * m_MoveSpeed * dt;

		glm::vec3 prevDelta = m_TargetPosition - pos;
		glm::vec3 currDelta = m_TargetPosition - nextPos;

		if (glm::dot(prevDelta, currDelta) <= 0.0f) {
			GetOwner()->SetLocalPosition(m_TargetPosition);
			FinishMove();
		}
		else {
			GetOwner()->SetLocalPosition(nextPos);
		}
	}

	void PlayerComponent::AttemptStep() {
		glm::vec3 pos = GetOwner()->GetWorldPosition();
		glm::vec3 topLeft = pos - glm::vec3(m_TileSize / 2.0f);
		int currX, currY;
		m_pLogic->WorldToGrid(topLeft, currX, currY);

		int nextX = currX + m_PendingDX;
		int nextY = currY + m_PendingDY;

		int gridW = m_pLogic->GetModel()->GetWidth();
		int gridH = m_pLogic->GetModel()->GetHeight();

		if (nextX < 0 || nextX >= gridW || nextY < 0 || nextY >= gridH) {
			std::cout << "[Player] blocked: outside grid\n";
			m_PendingDX = m_PendingDY = 0;
			return;
		}

		if (m_pView->GetTileTypeAt(nextX, nextY) == TileType::Wall) {
			HandleWallPush(nextX, nextY);
			return;
		}

		StartMoveTo(nextX, nextY);
	}

	void PlayerComponent::HandleWallPush(int wallX, int wallY) {
		glm::vec3 wallTopLeft = m_pLogic->GridToWorld(wallX, wallY);
		int newX = -1, newY = -1;

		bool pushed = m_pLogic->SlideOrBreakAt(wallTopLeft, m_PendingDX, m_PendingDY, newX, newY);
		if (pushed) {
			if (newX < 0 || newY < 0) {
				m_pView->OnWallBroken(wallX, wallY);
			}
			else {
				m_pView->OnWallPushed(wallX, wallY, newX, newY);
			}

			StartMoveTo(wallX, wallY);
		}
		else {
			m_PendingDX = m_PendingDY = 0;
		}
	}

	void PlayerComponent::StartMoveTo(int gx, int gy) {
		glm::vec3 center = m_pLogic->GridToWorld(gx, gy) + glm::vec3(m_TileSize / 2.0f);
		glm::vec3 from = GetOwner()->GetWorldPosition();
		glm::vec3 dir = glm::normalize(center - from);

		m_TargetPosition = center;
		m_MoveDirection = dir;
		m_IsMoving = true;

		if (auto* character = GetOwner()->GetComponent<CharacterComponent>()) {
			character->SetMoving(true);
		}

		m_PendingDX = m_PendingDY = 0;
	}

	void PlayerComponent::FinishMove() {
		m_IsMoving = false;
		m_MoveDirection = glm::vec3(0.0f);

		if (auto* character = GetOwner()->GetComponent<CharacterComponent>()) {
			character->SetMoving(false);
		}
	}
}
