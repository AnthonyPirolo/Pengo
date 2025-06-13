#pragma once
#include <vector>
#include "EnemyAIComponent.h"
#include "PlayerComponent.h"
#include "BaseComponent.h"

namespace dae
{
    class PlayerComponent;
    class EnemyAIComponent;

    class GameManager : public BaseComponent
    {
    public:
		GameManager(GameObject* owner)
			: BaseComponent(owner)
		{
		}

        void FixedUpdate(float deltaTime) override
        {
            deltaTime;
        }
		void Update() override
		{
		}
		void LateUpdate() override
		{
		}
        void Render() const override
        { 
        }

        void RegisterPlayer(PlayerComponent* player)
        {
			m_Players.push_back(player);
        }

        void RegisterEnemy(EnemyAIComponent* enemy)
        {
            m_Enemies.push_back(enemy);
        }

		void UnregisterEnemies()
		{
			for (auto& e : m_Enemies)
			{
				if (e)
				{
                    e = nullptr;
				}
			}
			m_Enemies.clear();
		}

		void UnregisterPlayer()
		{
			m_Players.clear();   
		}

        void ResetRound()
        {
            for (auto p : m_Players)
                if (p)
                    p->ResetToStart();

            for (auto e : m_Enemies)
                if (e)
                    e->ResetToSpawn();
        }

    private:
        std::vector<PlayerComponent*> m_Players;
        std::vector<EnemyAIComponent*> m_Enemies;
    };
}
