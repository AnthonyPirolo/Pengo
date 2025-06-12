#pragma once

#include "BaseComponent.h"
#include "GridLogic.h"
#include "GridModel.h"
#include "LevelLoader.h"
#include <memory>
#include <vector>
#include <glm.hpp>
#include "ScoreComponent.h"

namespace dae {
	class GameObject;

	class GridViewComponent final : public BaseComponent {
	public:
		GridViewComponent(GameObject* owner, int tileSize, const glm::vec3& offset);

		void LoadFromLevelData(const LevelData& level);
		void ClearLevel();

		std::shared_ptr<GameObject> GetPlayerGameObject() const { return m_SpawnedPlayer; }
		std::vector<std::shared_ptr<GameObject>> GetSpawnedEnemies() const;
		std::vector<std::shared_ptr<GameObject>> GetSpawnedWalls() const;

		TileType GetTileType(int x, int y) const;
		std::shared_ptr<GameObject> GetEnemyAt(int x, int y) const;

		void OnWallPushed(int oldX, int oldY, int newX, int newY);
		void OnWallBroken(int x, int y);

		void HatchInitialEggs(int count);
		void HatchNextEgg();

		bool HasEnemiesRemaining() const;

		void FixedUpdate(float) override {}
		void Update() override {}
		void LateUpdate() override {}
		void Render() const override {}

		GridLogic m_Logic;
		GridModel m_Model;

		GridModel& GetModel() { return m_Model; }

		ScoreComponent* GetScoreComponent() const { return m_ScoreComp; }

	private:
		std::shared_ptr<GameObject> CreateWallAt(int x, int y);
		std::shared_ptr<GameObject> CreatePlayerAt(int x, int y);
		std::shared_ptr<GameObject> CreateEnemyAt(int x, int y);

		int m_TileSize;
		glm::vec3 m_GridOffset;

		std::shared_ptr<GameObject> m_SpawnedPlayer;
		std::vector<std::shared_ptr<GameObject>> m_SpawnedEnemies;
		std::vector<std::shared_ptr<GameObject>> m_SpawnedWalls;
		std::vector<glm::ivec2> m_EggPositions;
		std::vector<bool> m_EggHatched;

		ScoreComponent* m_ScoreComp;
	};
}
