#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include <glm.hpp>
#include <iostream>
#include "ICollisionSystem.h"
#include "ServiceLocator.h"
#include "RigidbodyComponent.h"

namespace dae
{
    class CollisionComponent : public BaseComponent
    {
    public:
        explicit CollisionComponent(GameObject* owner)
            : BaseComponent(owner)
        {
        }

        ~CollisionComponent() override
        {
            ServiceLocator::GetCollisionSystem().Unregister(this);
        }

        void FixedUpdate(float) override {}
        void Update() override {}
        void LateUpdate() override {}
        void Render() const override {}

        AABB GetAABB() const
        {
            auto* rb = GetOwner()->GetComponent<RigidbodyComponent>();
            if (!rb)
            {
                std::cerr << "[CollisionComponent] No Rigidbody found\n";
                return {};
            }

            glm::vec3 center3D = GetOwner()->GetWorldPosition();
            glm::vec2 center{ center3D.x, center3D.y };
            glm::vec2 halfExtents{ rb->m_Width * 0.5f, rb->m_Height * 0.5f };
            return { center, halfExtents };
        }

        CollisionTag m_Tag{ CollisionTag::None };

        void Notify(Observer::Event event) override
        {
            for (auto& obs : m_Observers)
            {
                if (obs)
                    obs->OnNotify(this, event);
            }
        }

        void AttachObserver(std::shared_ptr<Observer> observer)
        {
            if (!observer) return;
            m_Observers.push_back(observer);
            observer->AddSubject(this);
            observer->OnNotify(this, Observer::Event::subjectAttached);
        }

        void RemoveObserver(Observer* observer)
        {
            if (!observer || m_Observers.empty()) return;
            m_Observers.erase(
                std::remove_if(
                    m_Observers.begin(),
                    m_Observers.end(),
                    [observer](auto& obs) { return obs.get() == observer; }
                ),
                m_Observers.end()
            );
        }

    private:
        std::vector<std::shared_ptr<Observer>> m_Observers;
    };
}
