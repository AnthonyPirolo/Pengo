#pragma once
#include <vector>
#include "EnemyAIComponent.h"
#include "PlayerComponent.h"

namespace dae
{
    class PlayerComponent;
    class EnemyAIComponent;

    class GameManager
    {
    public:
        GameManager() = default;
        ~GameManager() = default;

        GameManager(const GameManager&) = delete;
        GameManager& operator=(const GameManager&) = delete;

        void RegisterPlayer(PlayerComponent* player)
        {
            m_Player = player;
        }

        void RegisterEnemy(EnemyAIComponent* enemy)
        {
            m_Enemies.push_back(enemy);
        }

        void ResetRound()
        {
            if (m_Player)
                m_Player->ResetToStart();

            for (auto e : m_Enemies)
                if (e)
                    e->ResetToSpawn();
        }

    private:
        PlayerComponent* m_Player{ nullptr };
        std::vector<EnemyAIComponent*> m_Enemies;
    };
}
