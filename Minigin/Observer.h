#pragma once
#include <vector>
class Observer
{
	class BaseComponent;
public:

	enum class Event
	{
		enemyDied,
		playerDied,
		enemyHit,
		playerHit,
		subjectAttached,
		subjectDetached
	};

	virtual ~Observer() = default;
	virtual void OnNotify(dae::BaseComponent* entity, Event event) = 0;

	void AddSubject(dae::BaseComponent* subject);

private:
	std::vector<dae::BaseComponent*> m_Subjects;
};

