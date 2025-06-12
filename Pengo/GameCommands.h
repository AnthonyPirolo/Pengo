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
#include "PlayerComponent.h"
#include <glm.hpp>
#include <cmath>
#include "LevelManager.h"
#include <functional>

class GameObjectCommand : public Command
{
public:
    GameObjectCommand(dae::GameObject* actor)
        : m_actor(actor)
    {
    }
    virtual ~GameObjectCommand() = default;

    dae::GameObject* GetGameObject() const { return m_actor; }

private:
    dae::GameObject* m_actor;
};


class MoveCommand : public GameObjectCommand
{
public:

    MoveCommand(dae::GameObject* owner, float speed = 50.0f)
        : GameObjectCommand(owner)
        , m_Speed(speed)
        , m_Direction{ 0.f, 0.f, 0.f }
    {
    }

    ~MoveCommand() override = default;

    MoveCommand(const MoveCommand& other)
        : GameObjectCommand(other)
        , m_Speed(other.m_Speed)
        , m_Direction(other.m_Direction)
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
        : GameObjectCommand(std::move(other))
        , m_Speed(other.m_Speed)
        , m_Direction(std::move(other.m_Direction))
    {
        other.m_Speed = 50.0f;
        other.m_Direction = glm::vec3(0.f);
    }

    MoveCommand& operator=(MoveCommand&& other) noexcept
    {
        if (this == &other) return *this;
        GameObjectCommand::operator=(std::move(other));
        m_Speed = other.m_Speed;
        m_Direction = std::move(other.m_Direction);
        other.m_Speed = 50.0f;
        other.m_Direction = glm::vec3(0.f);
        return *this;
    }

    void SetDirection(const glm::vec3& direction) { m_Direction = direction; }

    virtual void Execute() override
    {
        dae::GameObject* owner = GetGameObject();
        if (!owner)
        {
            std::cout << "MoveCommand executed but owner is null!" << std::endl;
            return;
        }

        if (auto* pComp = owner->GetComponent<dae::PlayerComponent>())
        {
            int dx = 0, dy = 0;
            if (std::fabs(m_Direction.x) > 0.5f)
            {
                dx = (m_Direction.x > 0.0f ? 1 : -1);
            }
            else if (std::fabs(m_Direction.y) > 0.5f)
            {
                dy = (m_Direction.y > 0.0f ? 1 : -1);
            }
            pComp->SetDesiredDirection(dx, dy);

            if (auto* character = owner->GetComponent<dae::CharacterComponent>())
            {
                character->SetMoving(true);
            }
            return;
        }

        {
            glm::vec3 newPos = owner->GetWorldPosition() +
                (m_Direction * m_Speed * dae::GameTime::GetInstance().GetDeltaTime());
            owner->SetLocalPosition(newPos);

            if (auto* character = owner->GetComponent<dae::CharacterComponent>())
            {
                character->SetMoving(true);
            }
        }
    }

private:
    glm::vec3 m_Direction;
    float m_Speed;
};

class AttackCommand : public GameObjectCommand
{
public:
    AttackCommand(dae::GameObject* owner, float attackRange, float damage)
        : GameObjectCommand(owner)
        , m_AttackRange(attackRange)
        , m_Damage(damage)
    {
    }

    ~AttackCommand() override = default;

    AttackCommand(const AttackCommand& other)
        : GameObjectCommand(other)
        , m_AttackRange(other.m_AttackRange)
        , m_Damage(other.m_Damage)
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
        : GameObjectCommand(std::move(other))
        , m_AttackRange(other.m_AttackRange)
        , m_Damage(other.m_Damage)
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

    virtual void Execute() override
    {
        dae::GameObject* owner = GetGameObject();
        if (!owner) return;
    }

private:
    float m_AttackRange;
    float m_Damage;

    dae::GameObject* FindClosestEnemy();
};

class SoundCommand : public GameObjectCommand
{
public:
    SoundCommand(dae::GameObject* owner, sound_id id, float volume)
        : GameObjectCommand(owner)
        , m_SoundId(id)
        , m_Volume(volume)
    {
    }

    ~SoundCommand() override = default;

    virtual void Execute() override
    {

        std::cout << "F2 pressed -> toggling mute\n";
        ServiceLocator::GetSoundSystem().ToggleMute();
    }

private:
    sound_id m_SoundId;
    float    m_Volume;
};

class SkipLevelCommand : public Command
{
public:
    SkipLevelCommand(LevelManager* mgr, dae::GridViewComponent* view)
        : m_Mgr(mgr), m_View(view) 
{
    }
    virtual void Execute() override {
        if (!m_Mgr->LoadNextLevel(m_View)) {
            std::cout << "No more levels to load!\n";
        }
    }
private:
    LevelManager* m_Mgr;
    dae::GridViewComponent* m_View;
};

class LambdaCommand final : public Command
{
public:
    explicit LambdaCommand(std::function<void()> func)
        : m_Func(std::move(func)) 
    {
    }

    virtual void Execute() override
    {
        if (m_Func) m_Func();
    }

private:
    std::function<void()> m_Func;
};

