#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"

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

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

