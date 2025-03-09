#pragma once
#include "GameObject.h"
#include "Time.h"

class Command
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
};


class GameObjectCommand : public Command
{
public:
	GameObjectCommand(dae::GameObject* actor)
	{
		m_actor = actor;
	};
	virtual ~GameObjectCommand() = default;
	dae::GameObject* GetGameObject() const { return m_actor; }
private:
	dae::GameObject* m_actor;
};

class MoveCommand : public GameObjectCommand
{
public:
	MoveCommand(dae::GameObject* owner, float speed = 50.0f) :
		GameObjectCommand(owner), m_Speed(speed)
	{
		m_Direction = {};
	};

	void SetDirection(glm::vec3 direction)
	{
		m_Direction = direction;
	}

	virtual void Execute() override
	{		
		dae::GameObject* owner = GetGameObject();
		if (owner) owner->SetLocalPosition(owner->GetWorldPosition() + m_Direction * m_Speed * dae::Time::GetInstance().GetDeltaTime());
	}

private:
	glm::vec3 m_Direction{ 0.f, 0.f, 0.f };
	float m_Speed{ };
};


