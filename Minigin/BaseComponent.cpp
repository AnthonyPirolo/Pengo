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

	BaseComponent::~BaseComponent()
	{
		for (auto& observer : m_Observers)
		{
			if (observer)
				observer->OnSubjectRemoved(this);
		}
		m_Observers.clear();
	}

	void BaseComponent::AttachObserver(std::shared_ptr<Observer> observer)
	{
		if (!observer)
			return;

		auto it = std::find_if(m_Observers.begin(), m_Observers.end(),
			[observer](const std::shared_ptr<Observer>& obs) { return obs.get() == observer.get(); });
		if (it == m_Observers.end())
		{
			m_Observers.emplace_back(observer);
			observer->AddSubject(this);
			observer->OnNotify(this, Observer::Event::subjectAttached);
		}
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

		observer->OnSubjectRemoved(this);
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
