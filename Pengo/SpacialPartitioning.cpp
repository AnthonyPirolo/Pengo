#include "SpatialPartitioning.h"
#include <algorithm>

namespace dae
{
	void SpatialPartitionGrid::Register(GameObject* obj, const glm::ivec2& gridPos)
	{
		m_GridMap[gridPos].push_back(obj);
		m_ReverseLookup[obj] = gridPos;
	}

	void SpatialPartitionGrid::Unregister(GameObject* obj)
	{
		auto it = m_ReverseLookup.find(obj);
		if (it != m_ReverseLookup.end())
		{
			const auto& gridPos = it->second;
			auto& vec = m_GridMap[gridPos];
			vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end());
			if (vec.empty()) m_GridMap.erase(gridPos);
			m_ReverseLookup.erase(it);
		}
	}

	void SpatialPartitionGrid::UpdatePosition(GameObject* obj, const glm::ivec2& newGridPos)
	{
		Unregister(obj);
		Register(obj, newGridPos);
	}

	std::vector<GameObject*> SpatialPartitionGrid::GetObjectsAt(const glm::ivec2& gridPos) const
	{
		auto it = m_GridMap.find(gridPos);
		if (it != m_GridMap.end())
			return it->second;
		return {};
	}
}