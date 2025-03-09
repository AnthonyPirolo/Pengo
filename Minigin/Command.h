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

class MoveCommand : public GameObjectCommand {
public:
    MoveCommand(dae::GameObject* owner, float speed = 50.0f)
        : GameObjectCommand(owner), m_Speed(speed) {
    }

    // Rule of five
    ~MoveCommand() = default;

    MoveCommand(const MoveCommand& other)
        : GameObjectCommand(other), m_Speed(other.m_Speed), m_Direction(other.m_Direction) {
    }

    MoveCommand& operator=(const MoveCommand& other) {
        if (this == &other) return *this;
        GameObjectCommand::operator=(other);
        m_Speed = other.m_Speed;
        m_Direction = other.m_Direction;
        return *this;
    }

    MoveCommand(MoveCommand&& other) noexcept
        : GameObjectCommand(std::move(other)), m_Speed(other.m_Speed), m_Direction(std::move(other.m_Direction)) {
        other.m_Speed = 0.0f;
    }

    MoveCommand& operator=(MoveCommand&& other) noexcept {
        if (this == &other) return *this;
        GameObjectCommand::operator=(std::move(other));
        m_Speed = other.m_Speed;
        m_Direction = std::move(other.m_Direction);
        other.m_Speed = 0.0f;
        return *this;
    }

    void SetDirection(glm::vec3 direction) { m_Direction = direction; }

    virtual void Execute() override
    {
        dae::GameObject* owner = GetGameObject();
        if (owner) owner->SetLocalPosition(owner->GetWorldPosition() + m_Direction * m_Speed * dae::Time::GetInstance().GetDeltaTime());
    }

private:
    glm::vec3 m_Direction{ 0.f, 0.f, 0.f };
    float m_Speed{ };
};