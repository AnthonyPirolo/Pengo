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

		//---------------------------------
		//Scale related functions
		//---------------------------------
		void SetScale(float scale) { m_Scale = scale; }
		float GetScale() const { return m_Scale; }

		//---------------------------------
		//Templated component functions
		//---------------------------------
		template <typename T>
		T* GetComponent()
		{
			for (const auto& component : m_pComponents)
			{
				if (dynamic_cast<T*>(component.get()))
				{
					return static_cast<T*>(component.get());
				}
			}
			return nullptr;
		}

		template <typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of<BaseComponent, T>::value, "T must derive from BaseComponent");
			auto newComponent = std::make_unique<T>(std::forward<Args>(args)...);
			newComponent->SetOwner(this);
			T* newComponentPtr = newComponent.get();
			m_pComponents.push_back(std::move(newComponent));
			return newComponentPtr;
		}

		template <typename T>
		void RemoveComponent()
		{
			for (auto it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
			{
				if (dynamic_cast<T*>(it->get()))
				{
					m_pDeleteComponents.push_back(std::move(*it));
					break;
				}
			}
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
		//Private Member fUnctions
		//---------------------------------
		void AddChild(GameObject* child) { m_pChildren.push_back(child); }
		void RemoveChild(GameObject* child);
		bool IsChild(GameObject* parent) const;

		void SetPositionDirty();



		//---------------------------------
		//Private Member Variables
		//---------------------------------

		//---------------------------------
		//Location related variables
		//---------------------------------
		glm::vec3 m_WorldPosition{};
		glm::vec3 m_LocalPosition{};
		bool m_PositionIsDirty{ true };

		//---------------------------------
		//Component related variables
		//---------------------------------
		std::vector<std::unique_ptr<BaseComponent>> m_pComponents;
		std::vector<std::unique_ptr<BaseComponent>> m_pDeleteComponents;

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