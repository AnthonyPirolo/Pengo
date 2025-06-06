#pragma once

#include "BaseComponent.h"
#include "GridLogic.h"
#include "GridModel.h"
#include <vector>
#include <glm.hpp>
#include <memory>

namespace dae
{
    class GameObject;

    class GridViewComponent : public BaseComponent
    {
    public:
        GridViewComponent(GameObject* owner, int tileSize, const glm::vec3& offset);

        void FixedUpdate(float) override {}
        void Update() override {}
        void LateUpdate() override {}
        void Render() const override {}

        void LoadFromLevelData(const LevelData& level);
        void ClearLevel();

        std::shared_ptr<GameObject> CreateWallAt(int x, int y);
        std::shared_ptr<GameObject> CreatePlayerAt(int x, int y);
        std::shared_ptr<GameObject> CreateEnemyAt(int x, int y);

        void OnWallPushed(int oldX, int oldY, int newX, int newY);
        void OnWallBroken(int x, int y);

        void HatchInitialEggs(int count);
        void HatchNextEgg();

        const std::vector<std::shared_ptr<GameObject>>& GetSpawnedEnemies() const { return m_SpawnedEnemies; }
        const std::shared_ptr<GameObject>& GetPlayerGameObject() const { return m_SpawnedPlayer; }

        TileType GetTileType(int x, int y) const;

        GameObject* GetEnemyAt(int x, int y) const;

    private:
        int m_TileSize;
        glm::vec3 m_GridOffset;

        GridModel m_Model;
        GridLogic m_Logic;

        std::vector<std::vector<GameObject*>> m_WallGameObjects;
        std::shared_ptr<GameObject> m_SpawnedPlayer;
        std::vector<std::shared_ptr<GameObject>> m_SpawnedEnemies;

        std::vector<std::pair<int, int>> m_EggPositions;
        std::vector<bool> m_EggHatched;
    };
}
