#include "GridViewComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "CreateEntity.h"
#include "CollisionSystem.h"
#include "RigidbodyComponent.h"
#include "MoveComponent.h"
#include "TextureComponent.h"
#include "PlayerComponent.h"
#include "Scene.h"
#include "CharacterComponent.h"
#include "SoundPlayer.h"
#include <random>
#include <iostream>
#include <json.hpp>
#include "EnemyAIComponent.h"

namespace dae
{
    GridViewComponent::GridViewComponent(GameObject* owner, int tileSize, const glm::vec3& offset)
        : BaseComponent(owner), m_TileSize(tileSize), m_GridOffset(offset),
        m_Model(), m_Logic(&m_Model, tileSize, offset)
    {
    }

    void GridViewComponent::ClearLevel()
    {
        for (auto& column : m_WallGameObjects)
            for (auto& wallPtr : column)
                if (wallPtr) wallPtr->MarkForDestroy();

        m_WallGameObjects.clear();

        for (auto& enemy : m_SpawnedEnemies)
            if (enemy) enemy->MarkForDestroy();
        m_SpawnedEnemies.clear();

        if (m_SpawnedPlayer)
        {
            m_SpawnedPlayer->MarkForDestroy();
            m_SpawnedPlayer = nullptr;
        }

        m_EggPositions.clear();
        m_EggHatched.clear();
        m_Model.Initialize(600, 800);
    }

    void GridViewComponent::LoadFromLevelData(const LevelData& level)
    {
        ClearLevel();
        m_Model.InitializeFromLevelData(level);
        m_Logic = GridLogic(&m_Model, m_TileSize, m_GridOffset);

        int w = m_Model.GetWidth(), h = m_Model.GetHeight();
        m_WallGameObjects.assign(w, std::vector<GameObject*>(h, nullptr));

        for (int x = 0; x < w; ++x)
            for (int y = 0; y < h; ++y)
                if (m_Model.IsWall(x, y)) CreateWallAt(x, y);

        for (const auto& rec : level.tiles)
        {
            switch (rec.type)
            {
            case TileType::Player:
                m_SpawnedPlayer = CreatePlayerAt(rec.x, rec.y);
                break;
            case TileType::Enemy:
                m_SpawnedEnemies.push_back(CreateEnemyAt(rec.x, rec.y));
                break;
            case TileType::Egg:
                m_EggPositions.emplace_back(rec.x, rec.y);
                break;
            default: break;
            }
        }

        m_EggHatched.assign(m_EggPositions.size(), false);
    }

    std::shared_ptr<GameObject> GridViewComponent::CreateWallAt(int x, int y)
    {
        glm::vec3 center = m_Logic.GridToWorld(x, y) + glm::vec3(m_TileSize / 2.0f);
        auto wall = CreateWall(center);
        wall->SetTag("wall");
        wall->AddComponent<RigidbodyComponent>(wall.get(), m_TileSize, m_TileSize);
        wall->AddComponent<TextureComponent>(wall.get(), "Wall.png");
        wall->AddComponent<MoveComponent>(wall.get(), 300.0f);
        wall->AddComponent<CollisionComponent>(wall.get());

        m_WallGameObjects[x][y] = wall.get();
        SceneManager::GetInstance().GetActiveScene().Add(wall);
        return wall;
    }

    std::shared_ptr<GameObject> GridViewComponent::CreatePlayerAt(int x, int y)
    {
        glm::vec3 center = m_Logic.GridToWorld(x, y) + glm::vec3(m_TileSize / 2.0f);
        auto player = CreatePlayer(center);
        player->SetTag("player");
        player->AddComponent<RigidbodyComponent>(player.get(), m_TileSize, m_TileSize);
        player->AddComponent<TextureComponent>(player.get(), "SinglePenguin.png");
        player->AddComponent<PlayerComponent>(player.get(), &m_Logic, this, float(m_TileSize), 150.0f);
        player->AddComponent<CollisionComponent>(player.get());
        player->AddComponent<CharacterComponent>(player.get());

        auto soundObs = std::make_shared<SoundPlayer>();
        player->GetComponent<CharacterComponent>()->AttachObserver(soundObs);

        m_SpawnedPlayer = player;
        SceneManager::GetInstance().GetActiveScene().Add(player);
        return player;
    }

    std::shared_ptr<GameObject> GridViewComponent::CreateEnemyAt(int x, int y)
    {
        glm::vec3 center = m_Logic.GridToWorld(x, y) + glm::vec3(m_TileSize / 2.0f);
        auto enemy = CreateEnemy(center);
        enemy->SetTag("enemy");
        enemy->AddComponent<RigidbodyComponent>(enemy.get(), m_TileSize, m_TileSize);
        enemy->AddComponent<TextureComponent>(enemy.get(), "SinglePenguin2.png");
        enemy->AddComponent<CharacterComponent>(enemy.get());
        enemy->AddComponent<MoveComponent>(enemy.get(), 150.0f);
        enemy->AddComponent<CollisionComponent>(enemy.get());
        enemy->AddComponent<EnemyAIComponent>(enemy.get(), &m_Logic, this, float(m_TileSize), 75.f);

        m_SpawnedEnemies.push_back(enemy);
        SceneManager::GetInstance().GetActiveScene().Add(enemy);
        return enemy;
    }

    void GridViewComponent::OnWallPushed(int oldX, int oldY, int newX, int newY)
    {
        if (oldX < 0 || oldX >= m_Model.GetWidth() || oldY < 0 || oldY >= m_Model.GetHeight()) return;
        GameObject* wall = m_WallGameObjects[oldX][oldY];
        if (!wall) return;
        m_WallGameObjects[oldX][oldY] = nullptr;

        if (newX < 0 || newX >= m_Model.GetWidth() || newY < 0 || newY >= m_Model.GetHeight())
        {
            wall->MarkForDestroy();
            return;
        }

        m_WallGameObjects[newX][newY] = wall;
        glm::vec3 center = m_Logic.GridToWorld(newX, newY) + glm::vec3(m_TileSize / 2.0f);

        if (auto* mc = wall->GetComponent<MoveComponent>())
            mc->SetTarget(center);
        else
            wall->SetLocalPosition(center);
    }

    void GridViewComponent::OnWallBroken(int x, int y)
    {
        if (x < 0 || x >= m_Model.GetWidth() || y < 0 || y >= m_Model.GetHeight()) return;
        if (auto* wall = m_WallGameObjects[x][y])
        {
            wall->MarkForDestroy();
            m_WallGameObjects[x][y] = nullptr;
        }
    }

    void GridViewComponent::HatchInitialEggs(int count)
    {
        if (m_EggPositions.empty()) return;

        std::vector<int> indices(m_EggPositions.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::shuffle(indices.begin(), indices.end(), std::mt19937{ std::random_device{}() });

        for (int i = 0; i < std::min(count, int(indices.size())); ++i)
        {
            int idx = indices[i];
            m_EggHatched[idx] = true;
            auto [x, y] = m_EggPositions[idx];
            m_SpawnedEnemies.push_back(CreateEnemyAt(x, y));
        }
    }

    void GridViewComponent::HatchNextEgg()
    {
        for (size_t i = 0; i < m_EggPositions.size(); ++i)
        {
            if (!m_EggHatched[i])
            {
                m_EggHatched[i] = true;
                auto [x, y] = m_EggPositions[i];
                m_SpawnedEnemies.push_back(CreateEnemyAt(x, y));
                return;
            }
        }
    }
}
