#include "RigidbodyComponent.h"
#include "GameObject.h"

namespace dae
{
    RigidbodyComponent::RigidbodyComponent(GameObject* owner, int width, int height)
        : BaseComponent(owner)
        , m_Width(width)
        , m_Height(height)
    {
    }
}
