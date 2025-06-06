#include "SpatialPartitioning.h"
#include "GameObject.h"
#include <stdexcept>
#include <algorithm>

namespace dae {

	SpatialPartitionGrid::SpatialPartitionGrid(int width, int height)
		: m_Width(width), m_Height(height)
	{
		if (width <= 0 || height <= 0)
			throw std::runtime_error("SpatialPartitionGrid: Invalid grid dimensions");

		m_Grid.resize(width, std::vector<std::vector<std::shared_ptr<GameObject>>>(height));
	}

	void SpatialPartitionGrid::Clear()
	{
		for (int x = 0; x < m_Width; ++x)
			for (int y = 0; y < m_Height; ++y)
				m_Grid[x][y].clear();
	}

	void SpatialPartitionGrid::Register(const std::shared_ptr<GameObject>& obj, int x, int y)
	{
		if (!IsInBounds(x, y) || !obj) return;
		m_Grid[x][y].push_back(obj);
	}

	void SpatialPartitionGrid::Unregister(const std::shared_ptr<GameObject>& obj, int x, int y)
	{
		if (!IsInBounds(x, y) || !obj) return;

		auto& cell = m_Grid[x][y];
		cell.erase(std::remove_if(cell.begin(), cell.end(),
			[&](const std::shared_ptr<GameObject>& go) { return go == obj; }),
			cell.end());
	}

	void SpatialPartitionGrid::Move(const std::shared_ptr<GameObject>& obj, int oldX, int oldY, int newX, int newY)
	{
		Unregister(obj, oldX, oldY);
		Register(obj, newX, newY);
	}

	std::vector<std::shared_ptr<GameObject>> SpatialPartitionGrid::GetObjectsAt(int x, int y) const
	{
		if (!IsInBounds(x, y)) return {};
		return m_Grid[x][y];
	}

	bool SpatialPartitionGrid::IsInBounds(int x, int y) const
	{
		return x >= 0 && x < m_Width && y >= 0 && y < m_Height;
	}
}
