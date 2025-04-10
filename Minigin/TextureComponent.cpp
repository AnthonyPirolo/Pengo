#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Texture2D.h"

dae::TextureComponent::TextureComponent(GameObject* owner, const std::string& path):
	BaseComponent(owner)
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
	m_scaledSize = m_texture->GetSize();
}

void dae::TextureComponent::Render() const
{
	if (!m_texture) return;
	const auto& pos = BaseComponent::GetOwner()->GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y, m_scaledSize.x, m_scaledSize.y);
}

void dae::TextureComponent::SetScale(const float xScale, const float yScale)
{
	m_scale.x = xScale;
	m_scale.y = yScale;

	m_scaledSize.x *= m_scale.x;
	m_scaledSize.y *= m_scale.y;
}
