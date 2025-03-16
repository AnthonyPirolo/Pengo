#include "BaseComponent.h"
#include "Observer.h"

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
		m_Observers.erase(
			std::remove_if(
				m_Observers.begin(),
				m_Observers.end(),
				[observer](Observer* obs) { return obs == observer; }
			),
			m_Observers.end()
		);
	}
	void BaseComponent::Notify(Observer::Event event) const
	{
		(void)event;
	}
}