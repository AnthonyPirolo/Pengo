#pragma once

#include <vector>
#include "LevelLoader.h"

namespace dae
{
    enum class SlideResult
    {
        Moved,
        Broken,
        None
    };

    class GridModel
    {
    public:
        GridModel() = default;
        GridModel(int width, int height);

        void Initialize(int width, int height);
        void InitializeFromLevelData(const LevelData& level);

        bool IsWall(int x, int y) const;
        void SetWall(int x, int y);
        void ClearWall(int x, int y);

        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

        SlideResult SlideOrBreak(int x, int y, int dx, int dy);

        bool HasWall(int x, int y) const { return IsWall(x, y); }

        bool IsInBounds(int x, int y) const;

    private:
        void ValidateDimensions(int width, int height) const;
        void CreateEmptyGrid();

        int m_Width = 0;
        int m_Height = 0;
        std::vector<std::vector<bool>> m_Walls;
    };
}
