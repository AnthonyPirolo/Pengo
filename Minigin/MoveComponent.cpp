#include "GameTime.h"
#include "GameObject.h"
#include "MoveComponent.h"

dae::MoveComponent::MoveComponent(GameObject* owner, float maxMovementSpeed) :
	BaseComponent(owner),
	m_MaxMovementSpeed(maxMovementSpeed),
	m_Direction(0.0f, 0.0f, 0.0f)
{

}

void dae::MoveComponent::Update()
{
	glm::vec3 newPosition = GetOwner()->GetWorldPosition() + m_Direction * GameTime::GetInstance().GetDeltaTime();
	GetOwner()->SetLocalPosition(newPosition);
}

void dae::MoveComponent::UpdateDirection(glm::vec3 dir)
{
	m_Direction = dir * m_MaxMovementSpeed;
}