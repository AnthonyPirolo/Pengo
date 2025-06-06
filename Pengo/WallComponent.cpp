#include "WallComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "GridViewComponent.h"
#include "EnemyAIComponent.h"

namespace dae
{
	WallComponent::WallComponent(GameObject* owner, GridViewComponent* view, int gridX, int gridY)
		: BaseComponent(owner),
		m_State(State::Idle),
		m_HasEgg(false),
		m_CurrentBreaker(nullptr),
		m_BreakTimer(0.0f),
		m_pGridView(view),
		m_GridX(gridX),
		m_GridY(gridY)
	{
	}

	void WallComponent::SetHasEgg(bool hasEgg)
	{
		m_HasEgg = hasEgg;
	}

	bool WallComponent::HasEgg() const
	{
		return m_HasEgg;
	}

    void WallComponent::FixedUpdate(float deltaTime)
    {
        switch (m_State)
        {
        case State::BeingBroken:
            if (!m_CurrentBreaker) return;

            m_BreakTimer += deltaTime;
            if (m_BreakTimer >= m_BreakDuration)
            {
                m_State = State::Broken;
                if (m_pGridView)
                    m_pGridView->OnWallBroken(m_GridX, m_GridY);
            }
            break;

        case State::Sliding:
            if (!m_pGridView) return;

            if (GameObject* enemy = m_pGridView->GetEnemyAt(m_GridX, m_GridY))
            {
                if (auto* ai = enemy->GetComponent<EnemyAIComponent>())
                {
                    glm::vec3 wallPos = GetOwner()->GetWorldPosition();
                    glm::vec3 enemyPos = enemy->GetWorldPosition();
                    glm::vec3 pushDir = glm::normalize(wallPos - enemyPos);
                    ai->SetPushed(pushDir, 300.0f);
                }
            }

        case State::Broken:
            break;
        case State::Idle:
            break;
        default:
            break;
        }
    }

}
