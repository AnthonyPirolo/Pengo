#include "WallComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "GridViewComponent.h"

namespace dae
{
	WallComponent::WallComponent(GameObject* owner)
		: BaseComponent(owner),
		m_State(State::Idle),
		m_HasEgg(false),
		m_CurrentBreaker(nullptr),
		m_BreakTimer(0.0f),
		m_pGridView(nullptr),
		m_GridX(-1),
		m_GridY(-1)
	{
	}

	void WallComponent::Initialize(GridViewComponent* view, int gridX, int gridY)
	{
		m_pGridView = view;
		m_GridX = gridX;
		m_GridY = gridY;
	}

	void WallComponent::SetHasEgg(bool hasEgg)
	{
		m_HasEgg = hasEgg;
	}

	bool WallComponent::HasEgg() const
	{
		return m_HasEgg;
	}

	void WallComponent::BeginBreaking(GameObject* breaker)
	{
		if (m_State == State::Idle || breaker == m_CurrentBreaker)
		{
			if (m_CurrentBreaker != breaker)
			{
				m_BreakTimer = 0.0f;
				m_CurrentBreaker = breaker;
			}
			m_State = State::BeingBroken;
		}
	}

	void WallComponent::CancelBreaking(GameObject* breaker)
	{
		if (breaker == m_CurrentBreaker)
		{
			m_State = State::Idle;
			m_BreakTimer = 0.0f;
			m_CurrentBreaker = nullptr;
		}
	}

	void WallComponent::Update()
	{
		if (m_State != State::BeingBroken || m_CurrentBreaker == nullptr)
			return;

		m_BreakTimer += GameTime::GetInstance().GetDeltaTime();

		if (m_BreakTimer >= m_BreakDuration)
		{
			m_State = State::Broken;

			if (m_pGridView)
				m_pGridView->OnWallBroken(m_GridX, m_GridY);
		}
	}
}
