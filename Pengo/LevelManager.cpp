#include "LevelManager.h"
#include "GridViewComponent.h"
#include <iostream>

LevelManager::LevelManager(const std::vector<std::string>& levelFiles)
    : m_LevelFilenames(levelFiles), m_CurrentIndex(0)
{
}

bool LevelManager::LoadNextLevel(dae::GridViewComponent* gridView)
{
    if (m_CurrentIndex >= m_LevelFilenames.size()) {
        return false;
    }
    const std::string& fn = m_LevelFilenames[m_CurrentIndex];
    dae::LevelData ld = dae::LevelLoader::LoadLevel(fn);
    if (ld.width == 0 || ld.height == 0) {
        std::cerr << "[LevelManager] Failed to load level: " << fn << std::endl;
        return false;
    }
    gridView->ClearLevel();
    gridView->LoadFromLevelData(ld);
    ++m_CurrentIndex;
    return true;
}

void LevelManager::Reset()
{
    m_CurrentIndex = 0;
}
