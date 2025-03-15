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

	void BaseComponent::AttachObserver(Observer* observer)
	{
		m_Observers.emplace_back(observer);
		observer->OnNotify(this, Observer::Event::subjectAttached);
	}

	void BaseComponent::RemoveObserver(Observer* observer)
	{
		std::erase_if(
			m_Observers,
			[observer](Observer* observer) {return observer == observer; }
		);
	}
}