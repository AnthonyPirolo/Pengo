#include <string>
#include <memory>
#include <algorithm>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	GameObject::~GameObject()
	{
		for (auto& component : m_pDeleteComponents)
			component.reset();
		m_pDeleteComponents.clear();

		m_pComponents.clear();

		m_pChildren.clear();
	}

	void GameObject::FixedUpdate(float deltaTime)
	{
		for (const auto& component : m_pComponents)
			component->FixedUpdate(deltaTime);
	}

	void GameObject::Update()
	{
		for (const auto& component : m_pComponents)
			component->Update();
	}

	void GameObject::LateUpdate()
	{
		for (const auto& deleted : m_pDeleteComponents)
		{
			auto it = std::remove_if(m_pComponents.begin(), m_pComponents.end(),
				[&](const std::unique_ptr<BaseComponent>& comp) {
					return comp.get() == deleted.get();
				});
			m_pComponents.erase(it, m_pComponents.end());
		}
		m_pDeleteComponents.clear();

		for (const auto& component : m_pComponents)
			component->LateUpdate();
	}

	void GameObject::Render() const
	{
		for (const auto& component : m_pComponents)
			component->Render();
	}

	void GameObject::SetLocalPosition(glm::vec3 location)
	{
		m_LocalPosition = location;
		SetPositionDirty();
	}

	const glm::vec3& GameObject::GetWorldPosition()
	{
		if (m_PositionIsDirty)
			UpdateWorldPosition();
		return m_WorldPosition;
	}

	void GameObject::UpdateWorldPosition()
	{
		if (m_pParent == nullptr)
			m_WorldPosition = m_LocalPosition;
		else
			m_WorldPosition = m_pParent->GetWorldPosition() + m_LocalPosition;

		m_PositionIsDirty = false;
	}

	void GameObject::SetPositionDirty()
	{
		m_PositionIsDirty = true;
		for (auto& child : m_pChildren)
			child->SetPositionDirty();
	}

	void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
	{
		if (IsChild(parent) || parent == this || m_pParent == parent)
			return;

		if (parent == nullptr)
			SetLocalPosition(GetWorldPosition());
		else if (keepWorldPosition)
			SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());

		SetPositionDirty();

		if (m_pParent)
			m_pParent->RemoveChild(this);
		m_pParent = parent;
		if (m_pParent)
			m_pParent->AddChild(std::unique_ptr<GameObject>(this));
	}

	void GameObject::AddChild(std::unique_ptr<GameObject> child)
	{
		m_pChildren.push_back(std::move(child));
	}

	void GameObject::RemoveChild(GameObject* child)
	{
		auto it = std::remove_if(m_pChildren.begin(), m_pChildren.end(),
			[&](const std::unique_ptr<GameObject>& c) { return c.get() == child; });
		m_pChildren.erase(it, m_pChildren.end());
	}

	bool GameObject::IsChild(GameObject* parent) const
	{
		return std::any_of(m_pChildren.begin(), m_pChildren.end(),
			[&](const std::unique_ptr<GameObject>& c) { return c.get() == parent; });
	}

	void GameObject::RemoveComponent(BaseComponent* pComponent)
	{
		auto it = std::find_if(m_pComponents.begin(), m_pComponents.end(),
			[pComponent](const std::unique_ptr<BaseComponent>& comp) {
				return comp.get() == pComponent;
			});
		if (it != m_pComponents.end())
			m_pDeleteComponents.push_back(std::move(*it));
	}
}