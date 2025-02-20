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
		//---------------------------------
		//Game related functions
		//---------------------------------
		void Update();
		void FixedUpdate(float deltaTime);
		void LateUpdate();
		void Render() const;

		//---------------------------------
		//Position related functons
		//---------------------------------
		void SetLocalPosition(glm::vec3 location);
		const glm::vec3& GetWorldPosition();
		void UpdateWorldPosition();
		Transform GetPosition() const { return m_transform; }
		void SetPositionDirty();

		//---------------------------------
		//Scale related functions
		//---------------------------------
		void SetScale(float scale) { m_Scale = scale; }
		float GetScale() const { return m_Scale; }

		//---------------------------------
		//Templated component functions
		//---------------------------------
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

		//---------------------------------
		//Parent related functions
		//---------------------------------
		GameObject* GetParent() const { return m_pParent; };
		void SetParent(GameObject* parent, bool keepWorldPosition);

		//---------------------------------
		//Child related functions
		//---------------------------------
		int GetChildCount() const { return int(m_pChildren.size()); }
		GameObject* GetChild(int index) const { return m_pChildren.at(index); }
		void AddChild(GameObject* child) { m_pChildren.push_back(child); }
		void RemoveChild(GameObject* child);
		bool IsChild(GameObject* parent) const;

		//---------------------------------
		//Constructor & Destructor
		//---------------------------------
		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		//---------------------------------
		//Private Member Variables
		//---------------------------------

		//---------------------------------
		//Location related variables
		//---------------------------------
		Transform m_transform{};
		Transform m_WorldLocation{};
		glm::vec3 m_WorldPosition{};
		glm::vec3 m_LocalPosition{};
		bool m_PositionIsDirty{};

		//---------------------------------
		//Component related variables
		//---------------------------------
		std::vector<std::shared_ptr<BaseComponent>> m_pComponents;

		//---------------------------------
		//Parent related variables
		//---------------------------------
		GameObject* m_pParent{};

		//---------------------------------
		//Child related variables
		//---------------------------------
		std::vector<GameObject*> m_pChildren;

		//---------------------------------
		//Scale related variables
		//---------------------------------
		float m_Scale = 1.0f;

	};
};
