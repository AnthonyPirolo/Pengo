#pragma once
#include "BaseComponent.h"

namespace dae
{
    class ScoreComponent final : public BaseComponent
    {
    public:
        explicit ScoreComponent(GameObject* owner);
        ~ScoreComponent() override = default;

        void AddPoints(int amount);
        void Reset();

        int GetScore() const { return m_Score; }

        void FixedUpdate(float) override {}
        void Update() override {}
        void LateUpdate() override {}
        void Render() const override {}

    private:
        int m_Score{ 0 };
    };
}
