#include "GridLogic.h"
#include <cmath>

namespace dae
{
    GridLogic::GridLogic(GridModel* model, int tileSize, const glm::vec3& gridOffset)
        : m_pModel(model), m_TileSize(tileSize), m_GridOffset(gridOffset)
    {
    }

    void GridLogic::WorldToGrid(const glm::vec3& topLeft, int& outGridX, int& outGridY) const
    {
        const float centerX = topLeft.x + 0.5f * static_cast<float>(m_TileSize);
        const float centerY = topLeft.y + 0.5f * static_cast<float>(m_TileSize);

        const float localX = centerX - m_GridOffset.x;
        const float localY = centerY - m_GridOffset.y;

        outGridX = static_cast<int>(std::floor(localX / static_cast<float>(m_TileSize)));
        outGridY = static_cast<int>(std::floor(localY / static_cast<float>(m_TileSize)));
    }

    glm::vec3 GridLogic::GridToWorld(int gridX, int gridY) const
    {
        const float worldX = m_GridOffset.x + static_cast<float>(gridX * m_TileSize);
        const float worldY = m_GridOffset.y + static_cast<float>(gridY * m_TileSize);
        return { worldX, worldY, m_GridOffset.z };
    }

    bool GridLogic::IsWallAt(const glm::vec3& topLeft) const
    {
        int gx = 0, gy = 0;
        WorldToGrid(topLeft, gx, gy);
        return m_pModel->IsWall(gx, gy);
    }

    bool GridLogic::SlideOrBreakAt(const glm::vec3& topLeft, int dirX, int dirY,
        int& outNewWallX, int& outNewWallY) const
    {
        int gx = 0, gy = 0;
        WorldToGrid(topLeft, gx, gy);

        const SlideResult result = m_pModel->SlideOrBreak(gx, gy, dirX, dirY);

        if (result == SlideResult::Moved)
        {
            int scanX = gx + dirX;
            int scanY = gy + dirY;
            while (m_pModel->IsInBounds(scanX, scanY))
            {
                if (m_pModel->IsWall(scanX, scanY))
                {
                    outNewWallX = scanX;
                    outNewWallY = scanY;
                    return true;
                }
                scanX += dirX;
                scanY += dirY;
            }
        }

        outNewWallX = -1;
        outNewWallY = -1;
        return result != SlideResult::None;
    }
}