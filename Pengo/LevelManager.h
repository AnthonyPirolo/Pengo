#pragma once
#include <vector>
#include <string>
#include "LevelLoader.h"
#include <memory>
#include "GridViewComponent.h"

class LevelManager {
public:
    LevelManager(const std::vector<std::string>& levelFiles);

    bool LoadNextLevel(dae::GridViewComponent* gridView);

    void Reset();

private:
    std::vector<std::string> m_LevelFilenames;
    size_t m_CurrentIndex;
};
