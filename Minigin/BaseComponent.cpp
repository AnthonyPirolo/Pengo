#include "BaseComponent.h"

namespace dae
{
	void BaseComponent::SetOwner(GameObject* owner)
	{
		m_pOwner = owner;
	}

	GameObject* BaseComponent::GetOwner() const
	{
		return m_pOwner;
	}
}