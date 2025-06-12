#include "GridModel.h"
#include <stdexcept>

namespace dae {

	GridModel::GridModel(int width, int height)
		: m_Width(width), m_Height(height)
	{
		ValidateDimensions(width, height);
		CreateEmptyGrid();
	}

	void GridModel::Initialize(int width, int height)
	{
		ValidateDimensions(width, height);
		m_Width = width;
		m_Height = height;
		CreateEmptyGrid();
	}

	void GridModel::InitializeFromLevelData(const LevelData& level)
	{
		Initialize(level.width, level.height);

		for (const auto& tile : level.tiles) {
			if (tile.type == TileType::Wall) {
				SetWall(tile.x, tile.y);
			}
		}
	}

	bool GridModel::IsWall(int x, int y) const
	{
		return IsInBounds(x, y) && m_Walls[x][y];
	}

	void GridModel::SetWall(int x, int y)
	{
		if (IsInBounds(x, y)) {
			m_Walls[x][y] = true;
		}
	}

	void GridModel::ClearWall(int x, int y)
	{
		if (IsInBounds(x, y)) {
			m_Walls[x][y] = false;
		}
	}

	SlideResult GridModel::SlideOrBreak(int x, int y, int dx, int dy) {
		if (!IsWall(x, y)) return SlideResult::None;

		int nextX = x + dx;
		int nextY = y + dy;

		if (!IsInBounds(nextX, nextY) || IsWall(nextX, nextY)) {
			ClearWall(x, y);
			return SlideResult::Broken;
		}

		int slideX = nextX;
		int slideY = nextY;

		while (IsInBounds(slideX + dx, slideY + dy) && !IsWall(slideX + dx, slideY + dy)) {
			slideX += dx;
			slideY += dy;
		}

		ClearWall(x, y);
		SetWall(slideX, slideY);
		return SlideResult::Moved;
	}

	bool GridModel::IsInBounds(int x, int y) const
	{
		return x >= 0 && x < m_Width && y >= 0 && y < m_Height;
	}

	void GridModel::ValidateDimensions(int width, int height) const
	{
		if (width <= 0 || height <= 0) {
			throw std::runtime_error("GridModel: invalid dimensions");
		}
	}

	void GridModel::CreateEmptyGrid()
	{
		m_Walls.assign(m_Width, std::vector<bool>(m_Height, false));
	}
}
