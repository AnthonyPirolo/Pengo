#pragma once  
#include "BaseComponent.h"  
#include <glm.hpp>  
#include <random>  
#include <unordered_map>  
#include <memory>

namespace dae  
{  
   class GridLogic;  
   class GridViewComponent;  

   class EnemyAIComponent : public BaseComponent
   {  
   public:  
       EnemyAIComponent(GameObject* owner, GridLogic* logic, GridViewComponent* view, float tileSize, float moveSpeed);  

       void FixedUpdate(float dt) override;  
       void Update() override {}  
       void LateUpdate() override {}  
       void Render() const override {}  

       void ResetToSpawn();

       void SetPushed(glm::vec3 dir, float speed)
       {
           m_State = EnemyState::BeingPushed;
           m_PushDirection = glm::normalize(dir);
           m_PushSpeed = speed;
       }

       void Die();

   private:  
       enum class Direction { Up, Down, Left, Right, None };  
       enum class EnemyState
       {
           Roaming,
           BeingPushed,
           Dead
       };

       void AttemptStep();  
       void HandleWallPush(int wallX, int wallY);  
       void StartMoveTo(int gx, int gy);  
       void FinishMove();  
       void PickNewDirection();  
       glm::ivec2 DirectionToOffset(Direction dir) const;  
       void HandlePushedMovement();

       glm::vec3 m_SpawnPos;  

       GridLogic* m_pGridLogic;  
       GridViewComponent* m_pGridView;  

       float m_TileSize;  
       float m_MoveSpeed;  

       bool m_IsMoving;  
       glm::vec3 m_TargetPosition;  
       glm::vec3 m_MoveDirection;  

       Direction m_CurrentDirection;  
       std::mt19937 m_RNG;  

       struct WallBreakState  
       {  
           bool isBreaking = false;  
           float timer = 0.0f;  
       };  

       std::unordered_map<std::pair<int, int>, WallBreakState,  
           std::hash<int>, std::equal_to<>> m_BreakingWalls;  

       EnemyState m_State{ EnemyState::Roaming };
       glm::vec3 m_PushDirection{};
       float m_PushSpeed{};
   };  
}
