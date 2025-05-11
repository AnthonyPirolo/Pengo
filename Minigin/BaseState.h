#pragma once
//---------------------------
//Not used for the time being
//---------------------------

class BaseState
{
public:
	BaseState() = default;
	virtual ~BaseState() = default;
	virtual void Enter() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Exit() = 0;

private:
	BaseState(const BaseState&) = delete;
	BaseState(BaseState&&) = delete;
	BaseState& operator=(const BaseState&) = delete;
	BaseState& operator=(BaseState&&) = delete;
};

