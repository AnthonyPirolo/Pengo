#include "HealthComponent.h"

//---------------------------------
//Constructor & Destructor
//---------------------------------
dae::HealthComponent::HealthComponent(GameObject* owner) :
	BaseComponent(owner)
{
	m_Lives = m_MaxLives;
}

void dae::HealthComponent::FixedUpdate(float deltaTime) 
{
	(void)deltaTime;
}

void dae::HealthComponent::Update() 
{

}

void dae::HealthComponent::LateUpdate() 
{

}

void dae::HealthComponent::Render() const 
{
	//No need to render anything
}


//---------------------------------
//Health Related Functions
//---------------------------------
void dae::HealthComponent::DecreaseHealth(float amount)
{
	m_Lives -= amount;
	if (m_Lives <= 0)
	{
		m_Lives = 0;
	}

	Notify(Observer::Event::playerHit);
}


void dae::HealthComponent::IncreaseHealth(float amount)
{
	m_Lives += amount;
	if (m_Lives >= m_MaxLives) m_Lives = m_MaxLives;
}

void dae::HealthComponent::SetStartHealth(float amount)
{
	m_StartLives = amount;
}

void dae::HealthComponent::SetHealth(float amount)
{
	m_Lives = amount;
}

float dae::HealthComponent::GetHealth() const
{
	return m_Lives;
}

void dae::HealthComponent::Notify(Observer::Event event) const
{
	for (auto observer : m_Observers)
	{
		if (observer)
		{
			observer->OnNotify(const_cast<HealthComponent*>(this), event);
		}
	}
}



