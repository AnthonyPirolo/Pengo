#include "CreateEntity.h"

#include "GameObject.h"
#include "Transform.h"
#include "TextureComponent.h"
#include "MoveComponent.h"
#include "CharacterComponent.h"
#include "IdleState.h"
#include "RunningState.h"
#include "PushingState.h"
#include "DeadState.h"
#include "HealthComponent.h"
#include "PointsComponent.h"
#include "SoundPlayer.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GameCommands.h"
#include "XInputManager.h"
#include "CollisionSystem.h"
#include <SDL.h>

namespace dae
{
    std::shared_ptr<GameObject> CreatePlayer(const glm::vec3& worldPosition)
    {
        auto player = std::make_shared<GameObject>();
        player->SetLocalPosition(worldPosition);
        player->SetTag("Player");
        return player;
    }

    std::shared_ptr<GameObject> CreateWall(const glm::vec3& worldPosition)
    {
        auto wall = std::make_shared<GameObject>();
        wall->SetLocalPosition(worldPosition);
        wall->SetTag("Wall");
        return wall;
    }

    std::shared_ptr<GameObject> CreateEnemy(const glm::vec3& worldPosition)
    {
        auto enemy = std::make_shared<GameObject>();
        enemy->SetLocalPosition(worldPosition);
        enemy->SetTag("Enemy");

        return enemy;
    }

    std::shared_ptr<GameObject> CreateEgg(const glm::vec3& worldPosition)
    {
        auto egg = std::make_shared<GameObject>();
        egg->SetLocalPosition(worldPosition);
        egg->SetTag("Egg");

        return egg;
    }
}
