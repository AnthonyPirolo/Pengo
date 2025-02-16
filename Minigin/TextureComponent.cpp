#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::TextureComponent::TextureComponent(const std::string& path)
{
	SetTexture(path);
}

void dae::TextureComponent::Update(float DeltaTime) 
{
	(void)DeltaTime; // Suppress unused parameter warning
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::TextureComponent::Render() const
{
	const auto& pos = m_transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

void dae::TextureComponent::SetPosition(float x, float y) 
{
	m_transform.SetPosition(x, y, 0.0f);
}