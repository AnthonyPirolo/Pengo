#include "IFileService.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace dae {
    class FileService : public IFileService {
    public:
        std::string ReadFile(const std::filesystem::path& filePath) const override {
            std::ifstream file(filePath);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file");
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        virtual void WriteFile(const std::filesystem::path& filePath, const std::string& content) const override {
            std::ofstream file(filePath, std::ios::trunc);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file");
            }

            file << content;
        }

        virtual void AppendToFile(const std::filesystem::path& filePath, const std::string& content) const override {
            std::ofstream file(filePath, std::ios::app);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file");
            }

            file << content;
        }
    };
}