#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"

dae::TextComponent::TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font)
	: m_needsUpdate(true), m_text(text), m_font(std::move(font)), m_textTexture(nullptr), BaseComponent(owner)
{ }

void dae::TextComponent::FixedUpdate(float DeltaTime)
{
	(void)DeltaTime; // Suppress unused parameter warning
}

void dae::TextComponent::Update()
{

	if (m_needsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_textTexture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextComponent::LateUpdate()
{

}

void dae::TextComponent::Render() const
{
	if (m_textTexture != nullptr)
	{
		const auto& pos = BaseComponent::GetOwner()->GetWorldPosition();
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}



