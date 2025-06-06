#pragma once
#include <memory>
#include <vector>
#include <string>
#include <glm.hpp>
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
		// Game Related Functions
		//---------------------------------
		void Update();
		void FixedUpdate(float deltaTime);
		void LateUpdate();
		void Render() const;

		//---------------------------------
		// Transform Related Functions
		//---------------------------------
		void SetLocalPosition(glm::vec3 location);
		const glm::vec3& GetWorldPosition();
		void UpdateWorldPosition();

		void SetScale(float scale) { m_Scale = scale; }
		float GetScale() const { return m_Scale; }

		//---------------------------------
		// Component Management
		//---------------------------------
		template <typename T>
		T* GetComponent()
		{
			for (const auto& component : m_pComponents)
			{
				if (dynamic_cast<T*>(component.get()))
					return static_cast<T*>(component.get());
			}
			return nullptr;
		}

		template <typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of<BaseComponent, T>::value, "T must derive from BaseComponent");
			auto newComponent = std::make_unique<T>(std::forward<Args>(args)...);
			newComponent->SetOwner(this);
			T* rawPtr = newComponent.get();
			m_pComponents.push_back(std::move(newComponent));
			return rawPtr;
		}

		void RemoveComponent(BaseComponent* pComponent);

		//---------------------------------
		// Parenting
		//---------------------------------
		GameObject* GetParent() const { return m_pParent; }
		void SetParent(GameObject* parent, bool keepWorldPosition);

		int GetChildCount() const { return int(m_pChildren.size()); }
		GameObject* GetChild(int index) const { return m_pChildren.at(index).get(); }

		//---------------------------------
		// Destruction/Tagging
		//---------------------------------
		void MarkForDestroy() { m_MarkedForDestroy = true; }
		bool IsMarkedForDestroy() const { return m_MarkedForDestroy; }

		void SetTag(const std::string& tag) { m_Tag = tag; }
		const std::string& GetTag() const { return m_Tag; }

		//---------------------------------
		// Construction
		//---------------------------------
		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void AddChild(std::unique_ptr<GameObject> child);
		void RemoveChild(GameObject* child);
		bool IsChild(GameObject* parent) const;

		void SetPositionDirty();

		//---------------------------------
		// Members
		//---------------------------------
		bool m_MarkedForDestroy = false;
		std::string m_Tag;

		glm::vec3 m_WorldPosition{};
		glm::vec3 m_LocalPosition{};
		bool m_PositionIsDirty{ true };
		float m_Scale = 1.0f;

		std::vector<std::unique_ptr<BaseComponent>> m_pComponents;
		std::vector<std::unique_ptr<BaseComponent>> m_pDeleteComponents;

		GameObject* m_pParent = nullptr;
		std::vector<std::unique_ptr<GameObject>> m_pChildren;
	};
}
