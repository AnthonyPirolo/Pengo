#pragma once

#include "BaseComponent.h"
#include "LevelLoader.h"
#include "ScoreComponent.h"
#include <vector>
#include <memory>
#include <glm.hpp>
#include "GridModel.h"
#include "GridLogic.h"

namespace dae {
    class GameObject;
    class CollisionComponent;
    class EnemyAIComponent;
    class EggComponent;

    class GridViewComponent : public BaseComponent {
    public:
        GridViewComponent(GameObject* owner, int tileSize, const glm::vec3& offset);
        ~GridViewComponent() override = default;

        virtual void FixedUpdate(float deltaTime) override { deltaTime; }
        virtual void Update() override {}
        virtual void Render() const override {}
        virtual void LateUpdate() override {}

        void ClearLevel();
        void LoadFromLevelData(const LevelData& level);

        std::vector<std::shared_ptr<GameObject>> GetSpawnedPlayers() const;
        std::vector<std::shared_ptr<GameObject>> GetSpawnedEnemies() const;
        std::vector<std::shared_ptr<GameObject>> GetSpawnedWalls() const;
        bool HasEnemiesRemaining() const;

		ScoreComponent* GetScoreComponent() const { return m_ScoreComp; }

        void HatchEgg(std::shared_ptr<GameObject> eggGO);
        void HatchInitialEggs(int count);
        void HatchNextEgg();


        void OnWallPushed(int oldX, int oldY, int newX, int newY);
        void OnWallBroken(int x, int y);

		TileType GetTileTypeAt(int x, int y) const;

        GridModel m_Model;
        GridLogic m_Logic;
        int m_TileSize;

    private:
        std::shared_ptr<GameObject> CreateWallAt(int x, int y, bool hasEgg);
        std::shared_ptr<GameObject> CreatePlayerAt(int x, int y);
        std::shared_ptr<GameObject> CreateEnemyAt(int x, int y);

        glm::vec3 m_GridOffset;
        ScoreComponent* m_ScoreComp;

        std::vector<std::shared_ptr<GameObject>> m_SpawnedWalls;
        std::vector<std::shared_ptr<GameObject>> m_SpawnedPlayers;
        std::vector<std::shared_ptr<GameObject>> m_SpawnedEnemies;
    };
}
