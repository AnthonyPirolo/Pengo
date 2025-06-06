#include "GridViewComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CreateEntity.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "RigidbodyComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "WallComponent.h"
#include "PlayerComponent.h"
#include "EnemyAIComponent.h"
#include "CharacterComponent.h"
#include "SoundPlayer.h"
#include "PlayerCollisionListener.h"
#include "ServiceLocator.h"
#include <algorithm>
#include <numeric>
#include <random>
#include "ScoreComponent.h"

namespace dae {

	GridViewComponent::GridViewComponent(GameObject* owner, int tileSize, const glm::vec3& offset)
		: BaseComponent(owner), m_TileSize(tileSize), m_GridOffset(offset),
		m_Model(), m_Logic(&m_Model, tileSize, offset),
		m_WallGrid(1, 1), m_EnemyGrid(1, 1), m_PlayerGrid(1, 1)
	{
	}

	void GridViewComponent::ClearLevel() {
		m_WallGrid.Clear();
		m_EnemyGrid.Clear();
		m_PlayerGrid.Clear();

		if (m_SpawnedPlayer)
			m_SpawnedPlayer->MarkForDestroy();

		m_SpawnedPlayer = nullptr;
		m_EggPositions.clear();
		m_EggHatched.clear();
		m_Model.Initialize(600, 800);
	}

	void GridViewComponent::LoadFromLevelData(const LevelData& level) {
		ClearLevel();
		m_Model.InitializeFromLevelData(level);

		int w = m_Model.GetWidth();
		int h = m_Model.GetHeight();
		m_WallGrid = SpatialPartitionGrid(w, h);
		m_EnemyGrid = SpatialPartitionGrid(w, h);
		m_PlayerGrid = SpatialPartitionGrid(w, h);

		for (int x = 0; x < w; ++x)
			for (int y = 0; y < h; ++y)
				if (m_Model.IsWall(x, y))
					CreateWallAt(x, y);

		for (const auto& tile : level.tiles) {
			switch (tile.type) {
			case TileType::Player: m_SpawnedPlayer = CreatePlayerAt(tile.x, tile.y); break;
			case TileType::Enemy: CreateEnemyAt(tile.x, tile.y); break;
			case TileType::Egg: m_EggPositions.emplace_back(tile.x, tile.y); break;
			default: break;
			}
		}

		m_EggHatched.assign(m_EggPositions.size(), false);
	}

	std::shared_ptr<GameObject> GridViewComponent::CreateWallAt(int x, int y) {
		auto wall = CreateWall(m_Logic.GridToWorld(x, y) + glm::vec3(m_TileSize / 2.f));
		wall->SetTag("wall");
		wall->AddComponent<RigidbodyComponent>(wall.get(), m_TileSize, m_TileSize);
		wall->AddComponent<TextureComponent>(wall.get(), "Wall.png");
		wall->AddComponent<MoveComponent>(wall.get(), 300.f);
		wall->AddComponent<CollisionComponent>(wall.get());
		wall->AddComponent<WallComponent>(wall.get(), this, x, y);

		m_WallGrid.Register(wall, x, y);
		SceneManager::GetInstance().GetActiveScene().Add(wall);
		return wall;
	}

	std::shared_ptr<GameObject> GridViewComponent::CreatePlayerAt(int x, int y) {
		auto player = CreatePlayer(m_Logic.GridToWorld(x, y) + glm::vec3(m_TileSize / 2.f));
		player->SetTag("player");
		player->AddComponent<RigidbodyComponent>(player.get(), m_TileSize, m_TileSize);
		player->AddComponent<TextureComponent>(player.get(), "SinglePenguin.png");

		auto* colComp = player->AddComponent<CollisionComponent>(player.get());
		colComp->m_Tag = CollisionTag::Player;

		auto* playerComp = player->AddComponent<PlayerComponent>(player.get(), &m_Logic, this, float(m_TileSize), 150.0f);
		ServiceLocator::GetCollisionSystem().Register(colComp);

		player->AddComponent<CharacterComponent>(player.get());
		player->GetComponent<CharacterComponent>()->AttachObserver(std::make_shared<SoundPlayer>());

		auto listener = std::make_shared<PlayerCollisionListener>(playerComp);
		colComp->AttachObserver(listener);

		m_SpawnedPlayer = player;
		m_PlayerGrid.Register(player, x, y);
		SceneManager::GetInstance().GetActiveScene().Add(player);
		return player;
	}

	std::shared_ptr<GameObject> GridViewComponent::CreateEnemyAt(int x, int y) {
		auto enemy = CreateEnemy(m_Logic.GridToWorld(x, y) + glm::vec3(m_TileSize / 2.f));
		enemy->SetTag("enemy");
		enemy->AddComponent<RigidbodyComponent>(enemy.get(), m_TileSize, m_TileSize);
		enemy->AddComponent<TextureComponent>(enemy.get(), "SinglePenguin2.png");
		enemy->AddComponent<CharacterComponent>(enemy.get());
		enemy->AddComponent<MoveComponent>(enemy.get(), 150.0f);

		auto* colComp = enemy->AddComponent<CollisionComponent>(enemy.get());
		colComp->m_Tag = CollisionTag::Enemy;
		ServiceLocator::GetCollisionSystem().Register(colComp);

		enemy->AddComponent<EnemyAIComponent>(enemy.get(), &m_Logic, this, float(m_TileSize), 75.f);
		m_EnemyGrid.Register(enemy, x, y);

		SceneManager::GetInstance().GetActiveScene().Add(enemy);
		return enemy;
	}

	void GridViewComponent::OnWallPushed(int oldX, int oldY, int newX, int newY) {
		auto walls = m_WallGrid.GetObjectsAt(oldX, oldY);
		if (walls.empty()) return;

		auto wall = walls.front();
		m_WallGrid.Move(wall, oldX, oldY, newX, newY);

		auto center = m_Logic.GridToWorld(newX, newY) + glm::vec3(m_TileSize / 2.0f);
		if (auto* move = wall->GetComponent<MoveComponent>()) move->SetTarget(center);
		else wall->SetLocalPosition(center);

		if (auto* wc = wall->GetComponent<WallComponent>()) {
			wc->SetGridPosition(newX, newY);
			wc->m_State = WallComponent::State::Sliding;
			wc->m_PushDirection = glm::ivec2(newX - oldX, newY - oldY);
		}

		auto enemy = GetEnemyAt(newX, newY);
		if (enemy)
		{
			if (auto* ai = enemy->GetComponent<EnemyAIComponent>()) ai->Die();
			m_EnemyGrid.Unregister(enemy, newX, newY);
		}
	}

	void GridViewComponent::OnWallBroken(int x, int y) {
		auto walls = m_WallGrid.GetObjectsAt(x, y);
		if (!walls.empty()) {
			auto wall = walls.front();
			if (wall) {
				wall->MarkForDestroy();
				m_WallGrid.Unregister(wall, x, y);
			}
		}
	}

	std::shared_ptr<GameObject> GridViewComponent::GetEnemyAt(int x, int y) const {
		if (!m_EnemyGrid.IsInBounds(x, y)) return nullptr;

		auto occupants = m_EnemyGrid.GetObjectsAt(x, y);
		for (const auto& obj : occupants) {
			if (obj && !obj->IsMarkedForDestroy() && obj->GetTag() == "enemy") {
				return obj;
			}
		}
		return nullptr;
	}


	TileType GridViewComponent::GetTileType(int x, int y) const {
		if (!m_Model.IsInBounds(x, y)) return TileType::Empty;
		if (m_Model.HasWall(x, y)) return TileType::Wall;
		if (!m_EnemyGrid.GetObjectsAt(x, y).empty()) return TileType::Enemy;
		if (!m_PlayerGrid.GetObjectsAt(x, y).empty()) return TileType::Player;
		return TileType::Empty;
	}

	std::vector<std::shared_ptr<GameObject>> GridViewComponent::GetSpawnedEnemies() const {
		std::vector<std::shared_ptr<GameObject>> enemies;
		for (int x = 0; x < m_Model.GetWidth(); ++x)
			for (int y = 0; y < m_Model.GetHeight(); ++y)
				for (const auto& e : m_EnemyGrid.GetObjectsAt(x, y))
					if (e && !e->IsMarkedForDestroy())
						enemies.push_back(e);
		return enemies;
	}

	void GridViewComponent::HatchInitialEggs(int count) {
		std::vector<int> indices(m_EggPositions.size());
		std::iota(indices.begin(), indices.end(), 0);
		std::shuffle(indices.begin(), indices.end(), std::mt19937{ std::random_device{}() });

		for (int i = 0; i < std::min(count, int(indices.size())); ++i) {
			int idx = indices[i];
			m_EggHatched[idx] = true;
			CreateEnemyAt(m_EggPositions[idx].x, m_EggPositions[idx].y);
		}
	}

	void GridViewComponent::HatchNextEgg() {
		for (size_t i = 0; i < m_EggPositions.size(); ++i) {
			if (!m_EggHatched[i]) {
				m_EggHatched[i] = true;
				CreateEnemyAt(m_EggPositions[i].x, m_EggPositions[i].y);
				return;
			}
		}
	}

	bool GridViewComponent::HasEnemiesRemaining() const {
		for (int x = 0; x < m_Model.GetWidth(); ++x)
			for (int y = 0; y < m_Model.GetHeight(); ++y)
				for (const auto& e : m_EnemyGrid.GetObjectsAt(x, y))
					if (e && !e->IsMarkedForDestroy()) return true;
		return false;
	}

	std::vector<std::shared_ptr<GameObject>> GridViewComponent::GetSpawnedWalls() const {
		std::vector<std::shared_ptr<GameObject>> walls;
		for (int x = 0; x < m_Model.GetWidth(); ++x)
			for (int y = 0; y < m_Model.GetHeight(); ++y)
				for (const auto& w : m_WallGrid.GetObjectsAt(x, y))
					if (w && !w->IsMarkedForDestroy())
						walls.push_back(w);
		return walls;
	}
}
