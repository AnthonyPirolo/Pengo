#include "Observer.h"
#include "BaseComponent.h"

dae::Observer::~Observer()
{
    for (BaseComponent* subject : m_Subjects)
    {
        if (subject)
            subject->RemoveObserver(this);
    }
    m_Subjects.clear();
}

void dae::Observer::AddSubject(BaseComponent* subject)
{
    if (!subject) return;

    if (std::find(m_Subjects.begin(), m_Subjects.end(), subject) == m_Subjects.end())
        m_Subjects.push_back(subject);
}

void dae::Observer::RemoveSubject(BaseComponent* subject)
{
    m_Subjects.erase(
        std::remove(m_Subjects.begin(), m_Subjects.end(), subject),
        m_Subjects.end());
}

void dae::Observer::OnSubjectRemoved(BaseComponent* subject)
{
    RemoveSubject(subject);
}