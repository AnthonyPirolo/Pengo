#include "Command.h"  
#include "SceneManager.h"  
#include "GameObject.h"  
#include "HealthComponent.h"  
#include "Scene.h"  

std::vector<dae::GameObject*> GetAllEnemies()
{
    std::vector<dae::GameObject*> enemies;
    auto& scene = dae::SceneManager::GetInstance().GetActiveScene();

    for (const auto& gameObject : scene.GetGameObjects()) // Assuming GetGameObjects() returns std::vector<std::shared_ptr<dae::GameObject>>
    {
        if (gameObject->GetComponent<dae::HealthComponent>()) // Checks if it has a HealthComponent (is an enemy)
        {
            enemies.push_back(gameObject.get()); // Convert shared_ptr to raw pointer
        }
    }

    return enemies;
}


dae::GameObject* AttackCommand::FindClosestEnemy()  
{  
   dae::GameObject* attacker = GetGameObject();  
   if (!attacker) return nullptr;  

   glm::vec3 attackerPos = attacker->GetWorldPosition();  
   dae::GameObject* closestEnemy = nullptr;  
   float closestDistance = m_AttackRange;  

   for (dae::GameObject* enemy : GetAllEnemies())  
   {  
       if (enemy == attacker) continue; // Don't attack yourself  

       float distance = glm::distance(attackerPos, enemy->GetWorldPosition());  
       if (distance < closestDistance)  
       {  
           closestEnemy = enemy;  
           closestDistance = distance;  
       }  
   }  

   return closestEnemy;  
}  

void AttackCommand::Execute()  
{  
   dae::GameObject* enemy = FindClosestEnemy();  
   if (enemy)  
   {  
       auto health = enemy->GetComponent<dae::HealthComponent>();  
       if (health)  
       {  
           health->DecreaseHealth(m_Damage);  
           std::cout << "Attacked enemy! New health: " << health->GetHealth() << std::endl;  
       }  
   }  
   else  
   {  
       std::cout << "No enemy in range!" << std::endl;  
   }  
}