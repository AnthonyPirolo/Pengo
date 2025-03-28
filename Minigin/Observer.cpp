#include "Observer.h"
#include "BaseComponent.h"

	dae::Observer::~Observer()
	{
		for (BaseComponent* subject : m_Subjects)
		{
			if (subject)
				subject->RemoveObserver(this);
		}
	}

	void dae::Observer::AddSubject(BaseComponent* subject)
	{
		m_Subjects.push_back(subject);
	}
