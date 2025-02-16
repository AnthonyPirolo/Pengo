#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

dae::TextureComponent::TextureComponent(const std::string& path)
{
	SetTexture(path);
}

void dae::TextureComponent::FixedUpdate(float DeltaTime) 
{
	(void)DeltaTime; // Suppress unused parameter warning
}

void dae::TextureComponent::Update()
{

}

void dae::TextureComponent::LateUpdate()
{

}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::TextureComponent::Render() const
{
	const auto& pos = m_pOwner->GetPosition().GetPosition();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}
