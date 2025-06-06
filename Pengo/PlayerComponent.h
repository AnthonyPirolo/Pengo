#pragma once

#include "BaseComponent.h"
#include "GridLogic.h"
#include <glm.hpp>

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

        void FixedUpdate(float) override {}
        void Update() override;
        void LateUpdate() override {}
        void Render() const override {}

        void SetDesiredDirection(int dx, int dy);

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
    };
}
