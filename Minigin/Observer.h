#pragma once
#include <vector>
#include <memory> 

namespace dae
{
	class BaseComponent;
	class Observer : public std::enable_shared_from_this<Observer>
	{
	public:

		enum class Event
		{
			enemyDied,
			playerDied,
			enemyHit,
			playerHit,
			subjectAttached,
			subjectDetached,

			addPoints,

			breakWall,
			moveWall,
			pointsPickup
		};

		virtual ~Observer();
		virtual void OnNotify(BaseComponent* entity, Event event) = 0;

		void AddSubject(BaseComponent* subject);

	private:
		std::vector<BaseComponent*> m_Subjects;
	};
}
