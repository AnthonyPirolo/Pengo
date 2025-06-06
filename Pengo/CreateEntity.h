#pragma once

#include <memory>
#include "GameObject.h"

namespace dae
{
    std::shared_ptr<GameObject> CreatePlayer(const glm::vec3& worldPosition);
    std::shared_ptr<GameObject> CreateWall(const glm::vec3& worldPosition);
	std::shared_ptr<GameObject> CreateEnemy(const glm::vec3& worldPosition);
}