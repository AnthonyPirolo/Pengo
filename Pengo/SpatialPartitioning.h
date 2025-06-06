#pragma once
#include <unordered_map>
#include <vector>
#include <glm.hpp>

namespace dae
{
	class GameObject;

	class SpatialPartitionGrid
	{
	public:
		void Register(GameObject* obj, const glm::ivec2& gridPos);
		void Unregister(GameObject* obj);
		void UpdatePosition(GameObject* obj, const glm::ivec2& newGridPos);
		std::vector<GameObject*> GetObjectsAt(const glm::ivec2& gridPos) const;

	private:
		std::unordered_map<glm::ivec2, std::vector<GameObject*>, std::hash<int>> m_GridMap;
		std::unordered_map<GameObject*, glm::ivec2> m_ReverseLookup;
	};
}