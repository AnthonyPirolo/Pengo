#pragma once
#include <string>
#include <filesystem>

namespace dae {
    class IFileService {
    public:
        virtual ~IFileService() = default;

        virtual std::string ReadFile(const std::filesystem::path& filePath) const = 0;

        virtual void WriteFile(const std::filesystem::path& filePath, const std::string& content) const = 0;

        virtual void AppendToFile(const std::filesystem::path&, const std::string& content) const = 0;
    };
}