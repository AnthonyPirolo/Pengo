#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include "FileService.h"

struct HighscoreEntry {
    std::string initials;
    int score;
};

class HighscoreManager {
public:
    HighscoreManager(const std::string& saveFilename = "highscores.txt");

    void Load();
    void AddEntry(const HighscoreEntry& entry);
    std::vector<HighscoreEntry> GetTopN(int n = 10) const;

    static void SetPendingScore(int score);
    static int GetPendingScore();

private:
    std::string m_Filename;
    std::filesystem::path m_FilePath;
    std::vector<HighscoreEntry> m_Entries;
    mutable std::mutex m_Mutex;
    std::unique_ptr<dae::FileService> m_FileService;

    void SaveToDisk() const;
    void EnsureFileExists() const;
    void SaveToDisk_NoLock() const;

    static int s_PendingScore;
};