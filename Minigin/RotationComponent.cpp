#include "RotationComponent.h"
#include "GameObject.h"
#include "Time.h"
#include <iostream>

namespace dae
{
	void RotationComponent::FixedUpdate(float deltaTime)
	{
		(void)deltaTime;
	}

	void RotationComponent::Update()
	{
		m_RotationCenter = (m_pTarget != nullptr) ? m_pTarget->GetWorldPosition() : m_RotationCenter;

		auto owner = BaseComponent::GetOwner();
		auto currWorldPos = owner->GetWorldPosition();

		glm::vec3 offset = currWorldPos - m_RotationCenter;

		float angleInRadians = glm::radians(m_RotationSpeed * Time::GetInstance().GetDeltaTime());

		float cosAngle = cos(angleInRadians);
		float sinAngle = sin(angleInRadians);

		glm::vec3 newOffset{};
		newOffset.x = offset.x * cosAngle - offset.y * sinAngle;
		newOffset.y = offset.x * sinAngle + offset.y * cosAngle;
		newOffset.z = offset.z;

		glm::vec3 newPos = m_RotationCenter + newOffset;

		if (owner->GetParent() != nullptr)
		{
			newPos -= owner->GetParent()->GetWorldPosition();
		}

		owner->SetLocalPosition(newPos); 
	}

	void RotationComponent::LateUpdate()
	{

	}

	void RotationComponent::Render() const
	{

	}

	RotationComponent::RotationComponent(GameObject* owner, float rotationSpeed, GameObject* target) :
		BaseComponent(owner), m_RotationSpeed(rotationSpeed), m_pTarget(target)
	{
		m_RotationCenter = m_pTarget->GetWorldPosition();
	};

	RotationComponent::RotationComponent(GameObject* owner, float rotationSpeed, const glm::vec3& rotationCenter) :
		BaseComponent(owner), m_RotationSpeed(rotationSpeed), m_pTarget(nullptr), m_RotationCenter(rotationCenter)
	{

	};


}
