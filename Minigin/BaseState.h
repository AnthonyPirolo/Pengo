#pragma once

enum class StateTransition
{
	None,
	ToMainMenu,
	ToSinglePlayer,
	ToCoOp,
	ToHighScore
};

class BaseState
{

public:
	BaseState() = default;
	virtual ~BaseState() = default;
	virtual void OnEnter() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void OnExit() = 0;
	virtual void Render() = 0;
	virtual StateTransition GetRequestedTransition() const { return StateTransition::None; }
	virtual void ClearTransitionRequest() {}

private:
	BaseState(const BaseState&) = delete;
	BaseState(BaseState&&) = delete;
	BaseState& operator=(const BaseState&) = delete;
	BaseState& operator=(BaseState&&) = delete;
};

