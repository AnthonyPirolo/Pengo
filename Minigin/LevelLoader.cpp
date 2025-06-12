#include "LevelLoader.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include "ResourceManager.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace dae
{
    LevelData LevelLoader::LoadLevel(const std::string& filename)
    {
        const fs::path fullPath = ResourceManager::GetInstance().m_dataPath / filename;

        std::ifstream file(fullPath);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot open level JSON: " + fullPath.string());
        }

        json j;
        try
        {
            file >> j;
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("JSON parse error: " + std::string(e.what()));
        }

        if (!j.contains("width") || !j.contains("height") || !j.contains("tiles"))
        {
            throw std::runtime_error("Level JSON missing required keys: width, height, or tiles");
        }

        LevelData result;
        result.width = j["width"].get<int>();
        result.height = j["height"].get<int>();

        if (result.width <= 0 || result.height <= 0)
        {
            throw std::runtime_error("Level JSON has invalid width or height");
        }

        for (auto& entry : j["tiles"])
        {
            if (!entry.contains("x") || !entry.contains("y") || !entry.contains("type"))
            {
                std::cerr << "[LevelLoader] Warning: skipping invalid tile entry\n";
                continue;
            }

            int tx = entry["x"].get<int>();
            int ty = entry["y"].get<int>();
            std::string tstr = entry["type"].get<std::string>();
            TileType t = TileType::Empty;

            if (tstr == "wall")       t = TileType::Wall;
            else if (tstr == "player") t = TileType::Player;
            else if (tstr == "enemy")  t = TileType::Enemy;
            else if (tstr == "egg") t = TileType::Egg;
            else {
                std::cerr << "[LevelLoader] Warning: unknown tile type '" << tstr << "'\n";
                continue;
            }

            if (tx < 0 || tx >= result.width || ty < 0 || ty >= result.height)
            {
                std::cerr << "[LevelLoader] Warning: tile out of bounds (" << tx << "," << ty << ")\n";
                continue;
            }

            result.tiles.push_back({ tx, ty, t });

            if (t == TileType::Wall && entry.contains("egg") && entry["egg"].get<bool>() == true)
            {
                result.tiles.push_back({ tx, ty, TileType::Egg });
            }
        }


        return result;
    }
}
