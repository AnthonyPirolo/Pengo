#include "GridViewComponent.h"
#include "SceneManager.h"
#include "CreateEntity.h"
#include "TextureComponent.h"
#include "RigidbodyComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "WallComponent.h"
#include "EggComponent.h"
#include "PlayerComponent.h"
#include "EnemyAIComponent.h"
#include "CharacterComponent.h"
#include "SoundPlayer.h"
#include <algorithm>
#include <random>
#include "Scene.h"
#include "PlayerCollisionListener.h"

namespace dae {

    GridViewComponent::GridViewComponent(GameObject* owner, int tileSize, const glm::vec3& offset)
        : BaseComponent(owner)
        , m_TileSize(tileSize)
        , m_GridOffset(offset)
        , m_Model()
        , m_Logic(&m_Model, tileSize, offset)
    {
        owner->AddComponent<dae::ScoreComponent>(owner);
        m_ScoreComp = owner->GetComponent<dae::ScoreComponent>();
    }

    void GridViewComponent::ClearLevel()
    {
        for (auto& wall : m_SpawnedWalls)
            if (wall) wall->MarkForDestroy();
        m_SpawnedWalls.clear();

        for (auto& enemy : m_SpawnedEnemies)
            if (enemy) enemy->MarkForDestroy();
        m_SpawnedEnemies.clear();

        for (auto& player : m_SpawnedPlayers) {
            if (player) {
                ServiceLocator::GetCollisionSystem().Unregister(
                    player->GetComponent<CollisionComponent>()
                );
                player->MarkForDestroy();
            }
        }
        m_SpawnedPlayers.clear();

        m_Model.Initialize(600, 800);
    }

    void GridViewComponent::LoadFromLevelData(const LevelData& level)
    {
        ClearLevel();
        m_Model.InitializeFromLevelData(level);

        int w = m_Model.GetWidth(), h = m_Model.GetHeight();
        for (int x = 0; x < w; ++x)
            for (int y = 0; y < h; ++y)
                if (m_Model.IsWall(x, y)) {
                    bool hasEgg = std::any_of(level.tiles.begin(), level.tiles.end(),
                        [&](auto& t) { return t.x == x && t.y == y && t.type == TileType::Egg; });
                    CreateWallAt(x, y, hasEgg);
                }

        m_SpawnedPlayers.clear();
        m_SpawnedEnemies.clear();
        for (auto& tile : level.tiles) {
            if (tile.type == TileType::Player)
                m_SpawnedPlayers.push_back(CreatePlayerAt(tile.x, tile.y));
            else if (tile.type == TileType::Enemy)
                m_SpawnedEnemies.push_back(CreateEnemyAt(tile.x, tile.y));
        }

        HatchInitialEggs(4);
    }

    std::shared_ptr<GameObject> GridViewComponent::CreateWallAt(int x, int y, bool hasEgg)
    {
        auto worldPos = m_Logic.GridToWorld(x, y) + glm::vec3(m_TileSize / 2.f);
        auto wall = CreateWall(worldPos);
        wall->SetTag("wall");
        wall->AddComponent<RigidbodyComponent>(wall.get(), m_TileSize, m_TileSize);
        wall->AddComponent<TextureComponent>(wall.get(), "Wall.png");
        wall->AddComponent<MoveComponent>(wall.get(), 300.f);
        wall->AddComponent<CollisionComponent>(wall.get());
        wall->AddComponent<WallComponent>(wall.get(), this, x, y);

        if (hasEgg) {
            auto egg = CreateEgg(worldPos);
            egg->SetTag("egg");
            egg->SetParent(wall.get(), false);
            egg->AddComponent<EggComponent>(egg.get(), x, y);
            SceneManager::GetInstance().GetActiveScene().Add(egg);
        }

        m_SpawnedWalls.push_back(wall);
        SceneManager::GetInstance().GetActiveScene().Add(wall);
        return wall;
    }

    std::shared_ptr<GameObject> GridViewComponent::CreatePlayerAt(int x, int y)
    {
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

        SceneManager::GetInstance().GetActiveScene().Add(player);
        return player;
    }

    std::shared_ptr<GameObject> GridViewComponent::CreateEnemyAt(int x, int y)
    {
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
        m_SpawnedEnemies.push_back(enemy);
        SceneManager::GetInstance().GetActiveScene().Add(enemy);
        return enemy;
    }

    void GridViewComponent::OnWallPushed(int oldX, int oldY, int newX, int newY)
    {
        m_Model.ClearWall(oldX, oldY);
        m_Model.SetWall(newX, newY);
        for (auto& w : m_SpawnedWalls) {
            if (!w) continue;
            auto* wc = w->GetComponent<WallComponent>();
            if (wc && wc->m_GridX == oldX && wc->m_GridY == oldY) {
                wc->SetGridPosition(newX, newY);
                wc->m_State = WallComponent::State::Sliding;
                wc->m_PushDirection = { newX - oldX, newY - oldY };
                return;
            }
        }
    }

    void GridViewComponent::OnWallBroken(int x, int y)
    {
        if (!m_Model.IsInBounds(x, y)) return;
        m_Model.ClearWall(x, y);
        for (auto it = m_SpawnedWalls.begin(); it != m_SpawnedWalls.end(); ++it) {
            auto& w = *it;
            if (!w) continue;
            auto* wc = w->GetComponent<WallComponent>();
            if (wc && wc->m_GridX == x && wc->m_GridY == y) {
                w->MarkForDestroy();
                m_SpawnedWalls.erase(it);
                return;
            }
        }
    }

    std::vector<std::shared_ptr<GameObject>> GridViewComponent::GetSpawnedPlayers() const
    {
        std::vector<std::shared_ptr<GameObject>> result;
        for (auto& p : m_SpawnedPlayers)
            if (p && !p->IsMarkedForDestroy())
                result.push_back(p);
        return result;
    }

    std::vector<std::shared_ptr<GameObject>> GridViewComponent::GetSpawnedEnemies() const
    {
        std::vector<std::shared_ptr<GameObject>> result;
        for (auto& e : m_SpawnedEnemies)
            if (e && !e->IsMarkedForDestroy())
                result.push_back(e);
        return result;
    }

    std::vector<std::shared_ptr<GameObject>> GridViewComponent::GetSpawnedWalls() const
    {
        std::vector<std::shared_ptr<GameObject>> result;
        for (auto& w : m_SpawnedWalls)
            if (w && !w->IsMarkedForDestroy())
                result.push_back(w);
        return result;
    }

    void GridViewComponent::HatchEgg(std::shared_ptr<GameObject> eggGO)
    {
        auto* ec = eggGO->GetComponent<EggComponent>();
        int x = ec->GetGridX(), y = ec->GetGridY();
        eggGO->MarkForDestroy();
        m_SpawnedEnemies.push_back(CreateEnemyAt(x, y));
    }

    void GridViewComponent::HatchInitialEggs(int count)
    {
        auto& scene = SceneManager::GetInstance().GetActiveScene();
        auto eggs = scene.GetObjectsWithTag("egg");
        std::shuffle(eggs.begin(), eggs.end(), std::mt19937{ std::random_device{}() });
        for (int i = 0; i < count && i < int(eggs.size()); ++i)
            HatchEgg(eggs[i]);
    }

    void GridViewComponent::HatchNextEgg()
    {
        auto eggs = SceneManager::GetInstance().GetActiveScene().GetObjectsWithTag("egg");
        if (!eggs.empty())
            HatchEgg(eggs.front());
    }

    bool GridViewComponent::HasEnemiesRemaining() const
    {
        for (auto& e : m_SpawnedEnemies)
            if (e && !e->IsMarkedForDestroy())
                return true;
        return false;
    }

    TileType GridViewComponent::GetTileTypeAt(int x, int y) const {
        if (!m_Model.IsInBounds(x, y)) return TileType::Empty;
        if (m_Model.HasWall(x, y)) return TileType::Wall;
        for (const auto& enemy : m_SpawnedEnemies) {
            if (enemy && !enemy->IsMarkedForDestroy()) {
                glm::vec3 pos = enemy->GetWorldPosition();
                glm::vec3 topLeft = pos - glm::vec3(m_TileSize / 2.0f);
                int ex, ey;
                m_Logic.WorldToGrid(topLeft, ex, ey);
                if (ex == x && ey == y)
                    return TileType::Enemy;
            }
        }
        if (!m_SpawnedPlayers.empty() && !m_SpawnedPlayers[0]->IsMarkedForDestroy() && !m_SpawnedPlayers[1]->IsMarkedForDestroy()) 
        {
            for (auto player : m_SpawnedPlayers)
            {
                glm::vec3 pos = player->GetWorldPosition();
                glm::vec3 topLeft = pos - glm::vec3(m_TileSize / 2.0f);
                int px, py;
                m_Logic.WorldToGrid(topLeft, px, py);
                if (px == x && py == y)
                    return TileType::Player;
            }
        }
        return TileType::Empty;
    }

}
