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
            m_Player = player;
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
			m_Player = nullptr;
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
