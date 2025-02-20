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
        //---------------------------------
		//Game Related Functions
        //---------------------------------
        virtual void FixedUpdate(float deltaTime) = 0;
        virtual void Update() = 0;
        virtual void LateUpdate() = 0;
        virtual void Render() const = 0;

		//---------------------------------
		//Owner related functions
		//---------------------------------
        //Should change ownership + "tell" owner component is removed 
        void SetOwner(GameObject* owner);
        GameObject* GetOwner() const;

		//---------------------------------
		//Constructor & Destructor
		//---------------------------------
        BaseComponent(GameObject* owner) : m_pOwner(owner) {};
        virtual ~BaseComponent() = default;
        BaseComponent(const BaseComponent& other) = delete;
        BaseComponent(BaseComponent&& other) = delete;
        BaseComponent& operator=(const BaseComponent& other) = delete;
        BaseComponent& operator=(BaseComponent&& other) = delete;
    };
}
