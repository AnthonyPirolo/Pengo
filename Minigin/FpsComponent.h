#pragma once
#include "BaseComponent.h"

namespace dae
{
	class FpsComponent final : public BaseComponent
	{
	public:
		FpsComponent(GameObject* owner);
		virtual ~FpsComponent() override = default;

		FpsComponent(const FpsComponent& other) = delete;
		FpsComponent(FpsComponent&& other) = delete;
		FpsComponent& operator=(const FpsComponent& other) = delete;
		FpsComponent& operator=(FpsComponent&& other) = delete;

		void FixedUpdate(float deltaTime) override;
		void Update() override;
		void LateUpdate() override;
		void Render() const override;

		void CalculateFPS(float deltaTime);

	private:
		float m_FPS{ 0.0f };
		int m_FrameCount{ 0 };
		float m_ElapsedTime{ 0.0f };

	};
}
