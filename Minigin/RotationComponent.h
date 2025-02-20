#pragma once
#include "BaseComponent.h"
#include "GameObject.h"

namespace dae
{

	class RotationComponent final : public BaseComponent
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
		RotationComponent(GameObject* owner, float rotationSpeed, GameObject* target);
		RotationComponent(GameObject* owner, float rotationSpeed, const glm::vec3& rotationCenter);
		virtual ~RotationComponent() override = default;
		RotationComponent(const RotationComponent& other) = delete;
		RotationComponent(RotationComponent&& other) = delete;
		RotationComponent& operator=(const RotationComponent& other) = delete;
		RotationComponent& operator=(RotationComponent&& other) = delete;
		
		//---------------------------------
		//Rotation related functions
		//---------------------------------
		
	private:
		float m_RotationSpeed{ 0.0f };
		glm::vec3 m_RotationCenter{ 0.0f, 0.0f, 0.0f };
		GameObject* m_pTarget;
	};
}

