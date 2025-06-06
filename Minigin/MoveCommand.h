#pragma once
#include "Command.h"
#include "GameObject.h"

//class MoveCommand : public Command
//{
//public:
//    enum Direction { Up, Down, Left, Right };
//
//    MoveCommand(dae::GameObject* gameObject, Direction direction, float speed)
//        : m_GameObject(gameObject), m_Direction(direction), m_Speed(speed) {}
//
//    void Execute() override
//    {
//        /*switch (m_Direction)
//        {
//        case Up:
//            m_GameObject->MoveUp();
//            break;
//        case Down:
//            m_GameObject->MoveDown();
//            break;
//        case Left:
//            m_GameObject->MoveLeft();
//            break;
//        case Right:
//            m_GameObject->MoveRight();
//            break;
//        }*/
//
//        //if (m_Direction == Direction::Up) m_GameObject->MoveUp();
//        //if (m_Direction == Direction::Right) m_GameObject->MoveRight();
//        //if (m_Direction == Direction::Down) m_GameObject->MoveDown();
//        //if (m_Direction == Direction::Left) m_GameObject->MoveLeft();
//    }
//
//private:
//    dae::GameObject* m_GameObject;
//    Direction m_Direction;
//    float m_Speed;
//};
