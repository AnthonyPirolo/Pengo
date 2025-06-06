#pragma once
#include <string>
#include <vector>
#include <mutex>

struct HighscoreEntry {
    std::string initials;
    int         score;
};

class HighscoreManager {
public:
    HighscoreManager(const std::string& saveFilename = "highscores.json");

    void Load();

    void AddEntry(const HighscoreEntry& entry);

    std::vector<HighscoreEntry> GetTopN(int n = 10) const;

private:
    std::string m_Filename;
    std::vector<HighscoreEntry> m_Entries;

    mutable std::mutex m_Mutex;

    void SaveToDisk() const;
};
