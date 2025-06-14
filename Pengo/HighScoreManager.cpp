#include "HighscoreManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "ResourceManager.h"

HighscoreManager::HighscoreManager(const std::string& saveFilename)
    : m_Filename(saveFilename)
    , m_FileService(std::make_unique<dae::FileService>())
{
    dae::ResourceManager& resourceManager = dae::ResourceManager::GetInstance();
    auto path = resourceManager.m_dataPath;
    const auto fullPath = path / m_Filename;
    m_FilePath = fullPath;

    EnsureFileExists();
}

void HighscoreManager::EnsureFileExists() const
{
    std::ifstream file(m_FilePath);

    if (!file.good()) {
        m_FileService->WriteFile(m_FilePath, "");
    }
}

void HighscoreManager::Load()
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Entries.clear();
    std::istringstream iss(m_FileService->ReadFile(m_FilePath));
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        std::string initials;
        int score;
        if (lineStream >> initials >> score) {
            m_Entries.push_back({ initials, score });
        }
    }
    std::sort(m_Entries.begin(), m_Entries.end(), [](auto& a, auto& b) {
        return a.score > b.score;
        });
}

void HighscoreManager::AddEntry(const HighscoreEntry& entry)
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Entries.push_back(entry);
    std::sort(m_Entries.begin(), m_Entries.end(), [](auto& a, auto& b) {
        return a.score > b.score;
        });
    if (m_Entries.size() > 100) {
        m_Entries.resize(100);
    }
    SaveToDisk_NoLock();
}

void HighscoreManager::SaveToDisk_NoLock() const
{
    std::ostringstream oss;
    for (const auto& e : m_Entries) {
        oss << std::setw(3) << std::left << e.initials << " " << e.score << "\n";
    }
    m_FileService->WriteFile(m_FilePath, oss.str());
}

std::vector<HighscoreEntry> HighscoreManager::GetTopN(int n) const
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    std::vector<HighscoreEntry> out;
    int count = std::min<int>(n, static_cast<int>(m_Entries.size()));
    out.insert(out.begin(), m_Entries.begin(), m_Entries.begin() + count);
    return out;
}

void HighscoreManager::SaveToDisk() const
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    std::ostringstream oss;
    for (const auto& e : m_Entries) {
        oss << std::setw(3) << std::left << e.initials << " " << e.score << "\n";
    }
    m_FileService->WriteFile(m_FilePath, oss.str());
}