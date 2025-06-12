#include "GameObject.h"
#include <algorithm>
#include <glm.hpp>
#include "BaseComponent.h"

namespace dae
{
    GameObject::~GameObject()
    {
        m_pChildren.clear();

        m_pDeleteComponents.clear();
        m_pComponents.clear();
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
        for (auto* child : m_pChildren)
            child->SetPositionDirty();
    }

    void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
    {
        if (parent == this || m_pParent == parent)
            return;
        if (parent && parent->IsChild(this))
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
            m_pParent->AddChild(this);
    }

    void GameObject::AddChild(GameObject* child)
    {
        if (!child || child == this)
            return;
        if (std::find(m_pChildren.begin(), m_pChildren.end(), child) == m_pChildren.end())
            m_pChildren.push_back(child);
    }

    void GameObject::RemoveChild(GameObject* child)
    {
        auto it = std::remove(m_pChildren.begin(), m_pChildren.end(), child);
        m_pChildren.erase(it, m_pChildren.end());
    }

    bool GameObject::IsChild(GameObject* child) const
    {
        return std::find(m_pChildren.begin(), m_pChildren.end(), child) != m_pChildren.end();
    }

    GameObject* GameObject::GetChild(int index) const
    {
        if (index < 0 || index >= static_cast<int>(m_pChildren.size()))
            return nullptr;
        return m_pChildren[index];
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