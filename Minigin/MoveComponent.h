#pragma once
#include "BaseComponent.h"

namespace dae
{
	class MoveComponent final : public BaseComponent
	{
	public:
		//---------------------------------
		//Constructor & Destructor
		//---------------------------------
		MoveComponent(GameObject* owner, float movementSpeed = 50.0f);
		virtual ~MoveComponent() override = default;

		void Update() override;
		void UpdateDirection(glm::vec3 dir);

		MoveComponent(const MoveComponent& other) = delete;
		MoveComponent(MoveComponent&& other) = delete;
		MoveComponent& operator=(const MoveComponent& other) = delete;
		MoveComponent& operator=(MoveComponent&& other) = delete;

	private:
		float m_MaxMovementSpeed;
		glm::vec3 m_Direction;
	};
}
