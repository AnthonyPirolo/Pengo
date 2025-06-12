#pragma once
#include <string>

namespace dae {
    class IFileService {
    public:
        virtual ~IFileService() = default;

        virtual std::string ReadFile(const std::string& filePath) const = 0;

        virtual void WriteFile(const std::string& filePath, const std::string& content) const = 0;

        virtual void AppendToFile(const std::string& filePath, const std::string& content) const = 0;
    };
}