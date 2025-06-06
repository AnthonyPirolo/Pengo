#pragma once

#include "GridModel.h"
#include <glm.hpp>

namespace dae {
	class GridLogic {
	public:
		GridLogic(GridModel* model, int tileSize, const glm::vec3& gridOffset);

		void WorldToGrid(const glm::vec3& worldPos, int& outX, int& outY) const;
		glm::vec3 GridToWorld(int x, int y) const;

		bool IsWallAt(const glm::vec3& topLeft) const;

		bool SlideOrBreakAt(const glm::vec3& topLeft, int dirX, int dirY, int& outNewWallX, int& outNewWallY) const;

		GridModel* GetModel() const { return m_pModel; }

	private:
		GridModel* m_pModel;
		int m_TileSize;
		glm::vec3 m_GridOffset;
	};
}