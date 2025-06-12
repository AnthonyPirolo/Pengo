// EggComponent.h
#pragma once
#include "BaseComponent.h"

namespace dae {
    class EggComponent final : public BaseComponent {
    public:
        EggComponent(GameObject* owner, int gridX, int gridY)
            : BaseComponent(owner), m_GridX(gridX), m_GridY(gridY)
        {
        }

		~EggComponent() override = default;
        virtual void FixedUpdate(float deltaTime) override
        {
            deltaTime;
		}
        virtual void Render() const override
        {
            // Egg-specific rendering logic can go here
		}
        virtual void Update()
        {
        }
        virtual void LateUpdate()
        {
		}


        int GetGridX() const { return m_GridX; }
        int GetGridY() const { return m_GridY; }

    private:
        int m_GridX, m_GridY;
    };
}
