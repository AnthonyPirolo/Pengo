#pragma once

#include "BaseComponent.h"
#include "GridLogic.h"
#include <glm.hpp>
#include <memory>
#include <iostream>

namespace dae
{
    class GridViewComponent;

	class PlayerComponent : public BaseComponent
    {
    public:
        PlayerComponent(GameObject* owner,
            GridLogic* logic,
            GridViewComponent* view,
            float tileSize,
            float moveSpeed = 150.0f);

        void FixedUpdate(float deltaTime) override { deltaTime; }
        void Update() override;
        void LateUpdate() override {}
        void Render() const override {}

        void SetDesiredDirection(int dx, int dy);

        bool IsAlive() const { return m_IsAlive; };

        void HandleDeath()
        {
            m_IsAlive = false;
            m_DeathTimer = 3.0f;

            std::cout << "[PlayerComponent] Player died! Starting 1s cooldown...\n";
        }

        void ResetToStart();

        void SetSpawnPosition(const glm::vec3& pos)
        {
            m_SpawnPosition = pos;
        }


    private:
        void MoveTowardsTarget();
        void AttemptStep();
        void HandleWallPush(int wallX, int wallY);
        void StartMoveTo(int gx, int gy);
        void FinishMove();

        GridLogic* m_pLogic;
        GridViewComponent* m_pView;
        float m_TileSize;
        float m_MoveSpeed;

        bool m_IsMoving;
        glm::vec3 m_TargetPosition;
        glm::vec3 m_MoveDirection;
        int m_PendingDX;
        int m_PendingDY;

        bool m_IsAlive;
        float m_DeathTimer;
        glm::vec3 m_SpawnPosition;

        std::shared_ptr<Observer> m_Listener;
    };
}
