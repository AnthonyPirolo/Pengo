#pragma once

class BaseState
{
public:
	BaseState() = default;
	virtual ~BaseState() = default;
	virtual void OnEnter() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void OnExit() = 0;
	virtual void Render() = 0;

private:
	BaseState(const BaseState&) = delete;
	BaseState(BaseState&&) = delete;
	BaseState& operator=(const BaseState&) = delete;
	BaseState& operator=(BaseState&&) = delete;
};

