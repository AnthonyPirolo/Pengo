#include "PointsComponent.h"

dae::PointsComponent::PointsComponent(GameObject* owner):
	BaseComponent(owner), m_Points(0)
{

}

void dae::PointsComponent::FixedUpdate(float deltaTime)
{
	(void)deltaTime;
}

void dae::PointsComponent::Update()
{

}

void dae::PointsComponent::LateUpdate()
{

}

void dae::PointsComponent::Render() const
{
	//No need to render anything
}

void dae::PointsComponent::AddPoints()
{
	m_Points += 100;
	Notify(Observer::Event::addPoints);
	Notify(Observer::Event::pointsPickup);
}

void dae::PointsComponent::Notify(Observer::Event event) const
{
	for (auto observer : m_Observers)
	{
		if (observer)
		{
			observer->OnNotify(const_cast<PointsComponent*>(this), event);
		}
	}
}

int dae::PointsComponent::GetPoints()
{
	return m_Points;
}
