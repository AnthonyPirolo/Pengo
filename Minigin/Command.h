#pragma once
#include "GameObject.h"
#include "GameTime.h"
#include <iostream>
#include "PointsComponent.h"

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
    MoveCommand(dae::GameObject* owner, float speed = 50.0f)
        : GameObjectCommand(owner), m_Speed(speed) 
    {
    }

    // Rule of five
    ~MoveCommand() = default;

    MoveCommand(const MoveCommand& other)
        : GameObjectCommand(other), m_Speed(other.m_Speed), m_Direction(other.m_Direction) 
    {
    }

    MoveCommand& operator=(const MoveCommand& other) 
    {
        if (this == &other) return *this;
        GameObjectCommand::operator=(other);
        m_Speed = other.m_Speed;
        m_Direction = other.m_Direction;
        return *this;
    }

    MoveCommand(MoveCommand&& other) noexcept
        : GameObjectCommand(std::move(other)), m_Speed(other.m_Speed), m_Direction(std::move(other.m_Direction)) 
    {
        other.m_Speed = 50.0f;
    }

    MoveCommand& operator=(MoveCommand&& other) noexcept 
    {
        if (this == &other) return *this;
        GameObjectCommand::operator=(std::move(other));
        m_Speed = other.m_Speed;
        m_Direction = std::move(other.m_Direction);
        other.m_Speed = 50.0f;
        return *this;
    }

    void SetDirection(glm::vec3 direction) { m_Direction = direction; }

    virtual void Execute() override
    {
        dae::GameObject* owner = GetGameObject();
        if (owner) {
            owner->SetLocalPosition(owner->GetWorldPosition() + m_Direction * m_Speed * dae::GameTime::GetInstance().GetDeltaTime());
        }
        else {
            std::cout << "MoveCommand executed but owner is null!" << std::endl;
        }
    }

private:
    glm::vec3 m_Direction{ 0.f, 0.f, 0.f };
    float m_Speed{ };
};

class AttackCommand : public GameObjectCommand
{
public:
    AttackCommand(dae::GameObject* owner, float attackRange, float damage)
        : GameObjectCommand(owner), m_AttackRange(attackRange), m_Damage(damage) 
    {
    }

    // Rule of five
    ~AttackCommand() = default;

    AttackCommand(const AttackCommand& other)
        : GameObjectCommand(other), m_AttackRange(other.m_AttackRange), m_Damage(other.m_Damage) 
    {
    }

    AttackCommand& operator=(const AttackCommand& other) 
    {
        if (this == &other) return *this;
        GameObjectCommand::operator=(other);
        m_AttackRange = other.m_AttackRange;
        m_Damage = other.m_Damage;
        return *this;
    }

    AttackCommand(AttackCommand&& other) noexcept
        : GameObjectCommand(std::move(other)), m_AttackRange(other.m_AttackRange), m_Damage(other.m_Damage) 
    {
        other.m_AttackRange = 0.f;
        other.m_Damage = 0;
    }

    AttackCommand& operator=(AttackCommand&& other) noexcept 
    {
        if (this == &other) return *this;
        GameObjectCommand::operator=(std::move(other));
        m_AttackRange = other.m_AttackRange;
        m_Damage = other.m_Damage;
        other.m_AttackRange = 0.f;
        other.m_Damage = 0;
        return *this;
    }

    virtual void Execute() override;

private:
    float m_AttackRange;
    float m_Damage;

    dae::GameObject* FindClosestEnemy();
};

class AddPointsCommand : public GameObjectCommand
{
public:
    AddPointsCommand(dae::GameObject* owner, int points)
        : GameObjectCommand(owner), m_Points(points) 
    {
    }

    ~AddPointsCommand() = default;

    AddPointsCommand(const AddPointsCommand& other)
        : GameObjectCommand(other), m_Points(other.m_Points) 
    {
    }

    AddPointsCommand& operator=(const AddPointsCommand& other) 
    {
        if (this == &other) return *this;
        GameObjectCommand::operator=(other);
        m_Points = other.m_Points;
        return *this;
    }

    AddPointsCommand(AddPointsCommand&& other) noexcept
        : GameObjectCommand(std::move(other)), m_Points(other.m_Points) 
    {
        other.m_Points = 0;
    }

    AddPointsCommand& operator=(AddPointsCommand&& other) noexcept 
    {
        if (this == &other) return *this;
        GameObjectCommand::operator=(std::move(other));
        m_Points = other.m_Points;
        other.m_Points = 0;
        return *this;
    }

    virtual void Execute() override
    {
        dae::GameObject* owner = GetGameObject();
        if (owner) 
        {
			owner->GetComponent<dae::PointsComponent>()->AddPoints();
        }
        else {
            std::cout << "AddPointsCommand executed but owner is null!" << std::endl;
        }
    }

private:
    int m_Points;
};