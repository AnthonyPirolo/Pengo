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

	void BaseComponent::AttachObserver(std::shared_ptr<Observer> observer)
	{
		if (!observer)
			return;

		m_Observers.emplace_back(observer);
		observer->OnNotify(this, Observer::Event::subjectAttached);
	}

	void BaseComponent::RemoveObserver(Observer* observer)
	{
		if (!observer || m_Observers.empty())
			return;

		m_Observers.erase(
			std::remove_if(
				m_Observers.begin(),
				m_Observers.end(),
				[observer](const std::shared_ptr<Observer>& obs) {
					return obs.get() == observer;
				}),
			m_Observers.end());
	}

	void BaseComponent::Notify(Observer::Event event)
	{
		for (const auto& observer : m_Observers)
		{
			if (observer)
				observer->OnNotify(this, event);
		}
	}
}
