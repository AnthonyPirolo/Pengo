#pragma once
#include <memory>
#include "GameObject.h"

namespace dae
{
    class GameObject;
    class BaseComponent
    {
        GameObject* m_pOwner{};
    public:
        virtual void FixedUpdate(float deltaTime) = 0;
        virtual void Update() = 0;
        virtual void LateUpdate() = 0;
        virtual void Render() const = 0;

        //Should change ownership + "tell" owner component is removed 
        void SetOwner(GameObject* owner) { m_pOwner = owner; }
        GameObject* GetOwner() const { return m_pOwner; }

        BaseComponent(GameObject* owner) : m_pOwner(owner) {};
        virtual ~BaseComponent() = default;
        BaseComponent(const BaseComponent& other) = delete;
        BaseComponent(BaseComponent&& other) = delete;
        BaseComponent& operator=(const BaseComponent& other) = delete;
        BaseComponent& operator=(BaseComponent&& other) = delete;
    };
}
