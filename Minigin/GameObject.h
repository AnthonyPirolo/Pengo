#pragma once
#include <memory>
#include <vector>
#include "Transform.h"
#include "Font.h"
#include "BaseComponent.h"

namespace dae

{
	class Texture2D;
	class BaseComponent;
	class GameObject final
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
            newComponent->SetOwner(this);
            m_pComponents.push_back(newComponent);
            return newComponent;
        }

		template <typename T>
		void RemoveComponent()
		{
			//Remark: Mark component for deletion, then delete it in the next update loop 
			//Put it in another list than erase list and comp in other list
			m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(),
				[](const std::shared_ptr<BaseComponent>& component)
				{
					return dynamic_cast<T*>(component.get());
				}), m_pComponents.end());
		}

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		std::vector<std::shared_ptr<BaseComponent>> m_pComponents;
	};
}
