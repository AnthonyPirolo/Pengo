#pragma once
class GridCreator
{
	enum class TileType
	{
		Empty,
		Wall,
		Enemy,
		Player,
		Diamond
	};
public:
	GridCreator(float width, float height, int columns, int rows);
	virtual ~GridCreator() = default;

	void SetTileType(int column, int row, TileType type);

	void GetGrid();

private:
	float m_Width{ 100 };
	float m_Height{ 100 };

	int m_Columns{ 5 };
	int m_Rows{ 5 };
};

