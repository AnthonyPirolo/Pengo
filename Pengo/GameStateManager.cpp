#include "GameStateManager.h"

dae::GameStateManager& dae::GameStateManager::GetInstance() {
    static GameStateManager instance;
    return instance;
}

void dae::GameStateManager::ChangeState(BaseState* newState) {
    if (m_CurrentState) {
        m_CurrentState->OnExit();
    }
	m_CurrentState = std::unique_ptr<BaseState>(newState);
    if (m_CurrentState) m_CurrentState->OnEnter();
}

void dae::GameStateManager::Update(float deltaTime) {
    if (m_CurrentState) m_CurrentState->Update(deltaTime);
}
void dae::GameStateManager::Render() {
    if (m_CurrentState) m_CurrentState->Render();
}

dae::GameStateManager::~GameStateManager() {
    if (m_CurrentState) {
        m_CurrentState->OnExit();
    }
}
