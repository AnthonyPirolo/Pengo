#include "HighscoreManager.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

using json = nlohmann::json;

HighscoreManager::HighscoreManager(const std::string& saveFilename)
    : m_Filename(saveFilename)
{
}

void HighscoreManager::Load()
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Entries.clear();
    std::ifstream file(m_Filename);
    if (!file.is_open()) {
        return;
    }
    json j;
    file >> j;
    for (auto& entry : j) {
        HighscoreEntry e;
        e.initials = entry.value("initials", "");
        e.score = entry.value("score", 0);
        m_Entries.push_back(e);
    }
    std::sort(m_Entries.begin(), m_Entries.end(), [](auto& a, auto& b) {
        return a.score > b.score;
        });
}

void HighscoreManager::AddEntry(const HighscoreEntry& entry)
{
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Entries.push_back(entry);
        std::sort(m_Entries.begin(), m_Entries.end(), [](auto& a, auto& b) {
            return a.score > b.score;
            });
        if (m_Entries.size() > 100) {
            m_Entries.resize(100);
        }
    }
    std::thread saveThread([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        SaveToDisk();
        });
    saveThread.detach();
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
    json j = json::array();
    for (auto& e : m_Entries) {
        j.push_back({ {"initials", e.initials}, {"score", e.score} });
    }
    std::ofstream file(m_Filename);
    if (!file.is_open()) {
        std::cerr << "[HighscoreManager] Failed to open file for writing: " << m_Filename << std::endl;
        return;
    }
    file << std::setw(4) << j << std::endl;
}
