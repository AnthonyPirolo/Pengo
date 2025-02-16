#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TextComponent.h"
#include "TextureComponent.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime) 
{
    (void)deltaTime; // Suppress the unused parameter warning
	for (const auto& component : m_pComponents)
	{
		component->Update(deltaTime);
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
	for (const auto& component : m_pComponents)
	{
		component->SetPosition(x, y);
	}
}

void dae::GameObject::AddTextComponent(const std::string& text, std::shared_ptr<Font> font)
{
	m_pComponents.push_back(std::make_unique<TextComponent>(text, font));
}

void dae::GameObject::AddTextureComponent(const std::string& texture)
{
	m_pComponents.push_back(std::make_unique<TextureComponent>(texture));
}

