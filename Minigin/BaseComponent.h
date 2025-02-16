#pragma once
#include <memory>

namespace dae
{
    class GameObject;
    class BaseComponent
    {
    public:
        virtual void FixedUpdate(float deltaTime) = 0;
		virtual void Update() = 0;
		virtual void LateUpdate() = 0;
        virtual void Render() const = 0;

        void SetOwner(std::shared_ptr<GameObject> owner) { m_pOwner = owner; }
        std::shared_ptr<GameObject> GetOwner() const { return m_pOwner; }

        BaseComponent() = default;
        virtual ~BaseComponent() = default;
        BaseComponent(const BaseComponent& other) = delete;
        BaseComponent(BaseComponent&& other) = delete;
        BaseComponent& operator=(const BaseComponent& other) = delete;
        BaseComponent& operator=(BaseComponent&& other) = delete;

    protected:
        std::shared_ptr<GameObject> m_pOwner{};
    };
}
