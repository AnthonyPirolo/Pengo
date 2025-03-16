#pragma once
#include "BaseComponent.h"


namespace dae
{
	class GameObject;

	class HealthComponent final : public BaseComponent
	{
	public:

		//---------------------------------
		//Game Related Functions
		//---------------------------------
		void FixedUpdate(float deltaTime) override;
		void Update() override;
		void LateUpdate() override;
		void Render() const override;

		//---------------------------------
		//Constructor & Destructor
		//---------------------------------
		HealthComponent(GameObject* owner);
		virtual ~HealthComponent() override = default;

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		//---------------------------------
		//Health Related Functions
		//---------------------------------
		void DecreaseHealth(float amount);
		void IncreaseHealth(float amount);
		void SetStartHealth(float amount);
		void SetHealth(float amount);
		float GetHealth() const;

		

	private:
		//---------------------------------
		//Health Related Variables
		//---------------------------------
		float m_Lives{ 3.0f };
		float m_MaxLives{ 3.0f };
		float m_StartLives{ 3.0f };

		virtual void Notify(Observer::Event event) const;

	};
}


