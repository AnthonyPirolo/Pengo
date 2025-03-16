#pragma once
#include <vector>

namespace dae
{
	class BaseComponent;
	class Observer
	{
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

		virtual ~Observer();
		virtual void OnNotify(BaseComponent* entity, Event event) = 0;

		void AddSubject(BaseComponent* subject);

	private:
		std::vector<BaseComponent*> m_Subjects;
	};
}
