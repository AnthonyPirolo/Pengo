#pragma once

#include <vector>
#include <memory>
#include <glm.hpp>

namespace dae
{
	class GameObject;

	class SpatialPartitionGrid
	{
	public:
		SpatialPartitionGrid(int width, int height);

		void Clear();
		void Register(const std::shared_ptr<GameObject>& obj, int x, int y);
		void Unregister(const std::shared_ptr<GameObject>& obj, int x, int y);
		void Move(const std::shared_ptr<GameObject>& obj, int oldX, int oldY, int newX, int newY);

		std::vector<std::shared_ptr<GameObject>> GetObjectsAt(int x, int y) const;
		bool IsInBounds(int x, int y) const;

	private:
		int m_Width{};
		int m_Height{};
		std::vector<std::vector<std::vector<std::shared_ptr<GameObject>>>> m_Grid;
	};
}
