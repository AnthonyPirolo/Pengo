#pragma once
#include <memory>
#include <vector>
#include "Transform.h"
#include "BaseComponent.h"
#include "Font.h"

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:
		virtual void Update(float deltaTime);
		virtual void Render() const;

		void SetPosition(float x, float y);

		void AddTextComponent(const std::string& text, std::shared_ptr<Font> font);
		void AddTextureComponent(const std::string& texture);

		void RemoveAllComponents() { m_pComponents.clear(); }

		void RemoveTextComponent();
		void RemoveTextureComponent();

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		std::vector<std::unique_ptr<BaseComponent>> m_pComponents;
	};
}
