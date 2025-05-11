#include "Command.h"
#include "GameObject.h"
#include "GameTime.h"
#include "PointsComponent.h"
#include "ServiceLocator.h"
#include "ISoundSystem.h"
#include "SDLSoundSystem.h"
#include <iostream>
#include <memory>
#include "CharacterComponent.h"

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

            if (auto character = owner->GetComponent<dae::CharacterComponent>())
             {
                 character->SetMoving(true);
             }
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

class SoundCommand : public GameObjectCommand
{
public:
    SoundCommand(dae::GameObject* owner, sound_id id, float volume)
        : GameObjectCommand(owner), m_SoundId(id), m_Volume(volume) {
    }

    virtual ~SoundCommand() = default;

    virtual void Execute() override
    {
        std::cout << "Playing using Sound System at: " << &ServiceLocator::GetSoundSystem() << std::endl;
        dae::GameObject* owner = GetGameObject();
        if (owner)
        {
            // Fetch the sound system from ServiceLocator
            auto& soundSystem = ServiceLocator::GetSoundSystem();
            soundSystem.Play(m_SoundId, m_Volume);
            std::cout << "Executing SoundCommand with ID: " << m_SoundId << " and Volume: " << m_Volume << std::endl;
        }
        else
        {
            std::cout << "SoundCommand executed but owner is null!" << std::endl;
        }
    }

private:
    sound_id m_SoundId;
    float m_Volume;
};


