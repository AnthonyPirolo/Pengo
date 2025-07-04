#pragma once
#include <string>
#include <memory>
#include "BaseComponent.h"
#include "Transform.h"
#include "GameObject.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public BaseComponent
	{
	public:
		//---------------------------------
		//Game Related Functions
		//---------------------------------
		void FixedUpdate(float DeltaTime) override;
		void Update() override;
		void LateUpdate() override;
		void Render() const override;

		//---------------------------------
		//Text Related Functions
		//---------------------------------
		void SetText(const std::string& text);

		//---------------------------------
		//Constructor & Destructor
		//---------------------------------
		TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font);
		virtual ~TextComponent() override = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		//---------------------------------
		//Text Related Variables
		//---------------------------------
		bool m_needsUpdate;
		std::string m_text;
		Transform m_transform{};
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};
}
