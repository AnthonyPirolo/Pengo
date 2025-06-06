#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace dae
{
    enum class TileType
    {
        Empty,
        Wall,
        Player,
        Enemy,
        Egg
    };

    struct TileRecord
    {
        int x;
        int y;
        TileType type;
    };

    struct LevelData
    {
        int width = 0;
        int height = 0;
        std::vector<TileRecord> tiles;
    };

    class LevelLoader
    {
    public:
        static LevelData LoadLevel(const std::string& filename);
    };
}
