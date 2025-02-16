#pragma once
#include <memory>
#include <vector>
#include "Transform.h"
#include "BaseComponent.h"
#include "Font.h"

namespace dae
{
	class Texture2D;
	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:

		void Update();
		void FixedUpdate(float deltaTime);
		void LateUpdate();
		void Render() const;

		void SetPosition(float x, float y);
		Transform GetPosition() const { return m_transform; }

		template <typename T>
		std::shared_ptr<T> GetComponent()
		{
			for (const auto& component : m_pComponents)
			{
				if (dynamic_cast<T*>(component.get()))
				{
					return std::static_pointer_cast<T>(component);
				}
			}
			return nullptr;
		}

        template <typename T, typename... Args>
        std::shared_ptr<T> AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of<BaseComponent, T>::value, "T must derive from BaseComponent");
            const auto newComponent = std::make_shared<T>(std::forward<Args>(args)...);
            newComponent->SetOwner(shared_from_this());
            m_pComponents.push_back(newComponent);
            return newComponent;
        }

		template <typename T>
		void RemoveComponent()
		{
			m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(),
				[](const std::unique_ptr<BaseComponent>& component)
				{
					return dynamic_cast<T*>(component.get());
				}), m_pComponents.end());
		}

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		std::vector<std::shared_ptr<BaseComponent>> m_pComponents;
	};
}
