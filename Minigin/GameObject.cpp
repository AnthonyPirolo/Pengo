#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include <algorithm>

dae::GameObject::~GameObject() = default;

void dae::GameObject::FixedUpdate(float deltaTime) 
{
	for (const auto& component : m_pComponents)
	{
		component->FixedUpdate(deltaTime);
	}
}

void dae::GameObject::Update()
{
	for (const auto& component : m_pComponents)
	{
		component->Update();
	}
}

void dae::GameObject::LateUpdate()
{
	for (const auto& component : m_pDeleteComponents)
	{
		m_pComponents.erase(std::remove(m_pComponents.begin(), m_pComponents.end(), component), m_pComponents.end());
	}
	m_pDeleteComponents.clear();

	for (const auto& component : m_pComponents)
	{
		component->LateUpdate();
	}
}

void dae::GameObject::Render() const
{
	for (const auto& component : m_pComponents)
	{
		component->Render();
	}
}

void dae::GameObject::SetLocalPosition(glm::vec3 location)
{
	m_LocalPosition = location;
	SetPositionDirty();
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_PositionIsDirty)
		UpdateWorldPosition();
	return m_WorldPosition;
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		if (m_pParent == nullptr)
			m_WorldPosition = m_LocalPosition;
		else
			m_WorldPosition = m_pParent->GetWorldPosition() + m_LocalPosition;
	}
	m_PositionIsDirty = false;
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsChild(parent) || parent == this || m_pParent == parent)
		return;
	if (parent == nullptr)
		SetLocalPosition(GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
		SetPositionDirty();
	}
	if (m_pParent) m_pParent->RemoveChild(this);
	m_pParent = parent;
	if (m_pParent) m_pParent->AddChild(this);
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	if (child)
	{
		auto it = std::find(m_pChildren.begin(), m_pChildren.end(), child);
		if (it != m_pChildren.end())
			m_pChildren.erase(it);
	}
}

bool dae::GameObject::IsChild(GameObject* parent) const
{
	return std::find(m_pChildren.begin(), m_pChildren.end(), parent) != m_pChildren.end();
}

void dae::GameObject::SetPositionDirty()
{
	m_PositionIsDirty = true;
	for (auto child : m_pChildren)
		child->SetPositionDirty();
}


