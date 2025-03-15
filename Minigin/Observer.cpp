#include "Observer.h"
#include "BaseComponent.h"

Observer::~Observer()
{
	for (dae::BaseComponent* subject : m_Subjects)
	{
		subject->RemoveObserver(this);
	}
}

void Observer::AddSubject(dae::BaseComponent* subject)
{
	m_Subjects.push_back(subject);
}