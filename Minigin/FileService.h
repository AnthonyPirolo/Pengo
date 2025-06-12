#include "IFileService.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace dae {
    class FileService : public IFileService {
    public:
        std::string ReadFile(const std::string& filePath) const override {
            std::ifstream file(filePath);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file: " + filePath);
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        void WriteFile(const std::string& filePath, const std::string& content) const override {
            std::ofstream file(filePath, std::ios::trunc);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file: " + filePath);
            }

            file << content;
        }

        void AppendToFile(const std::string& filePath, const std::string& content) const override {
            std::ofstream file(filePath, std::ios::app);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file: " + filePath);
            }

            file << content;
        }
    };
}