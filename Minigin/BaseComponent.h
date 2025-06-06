#pragma once
#include <memory>
#include <vector>
#include "GameObject.h"
#include "Observer.h" // Include Observer.h to resolve the undefined type error

namespace dae
{
    class GameObject;
    class Observer; // Forward declaration of Observer

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

        //---------------------------------
        //Observer related functions
        //---------------------------------
        void AttachObserver(std::shared_ptr<Observer> observer);
        void RemoveObserver(Observer* observerToRemove);

        virtual void Notify(Observer::Event event);

    protected:
        //---------------------------------
        //Observer related functions
        //---------------------------------
        std::vector<std::shared_ptr<Observer>> m_Observers;
    };
}
