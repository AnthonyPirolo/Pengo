#include "FpsComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "GameTime.h"

dae::FpsComponent::FpsComponent(GameObject* owner) :
	BaseComponent(owner) {

}

void dae::FpsComponent::FixedUpdate(float deltaTime)
{
	(void)deltaTime;
}

void dae::FpsComponent::Update() 
{
	auto& time = dae::GameTime::GetInstance();
	float deltaTime = time.GetDeltaTime();
	CalculateFPS(deltaTime);
}

void dae::FpsComponent::LateUpdate()
{
	BaseComponent::GetOwner()->GetComponent<TextComponent>()->SetText("FPS: " + std::to_string(m_FPS));
}

void dae::FpsComponent::Render() const
{

}

void dae::FpsComponent::CalculateFPS(float deltaTime)
{
	m_ElapsedTime += deltaTime;
	++m_FrameCount;

	if (m_ElapsedTime >= 0.3f)
	{
		m_FPS = static_cast<float>(m_FrameCount) / m_ElapsedTime;
		m_FrameCount = 0;
		m_ElapsedTime = 0.0f;
	}
	
}