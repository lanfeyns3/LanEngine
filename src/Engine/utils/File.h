#pragma once

#include <string>
#include <fstream>
#include <expected>
#include <variant>

#include <nlohmann/json.hpp>
#include <nfd.hpp>

namespace LANE
{
    enum class FileLoadType
    {
        Raw,
        Json
    };

    using ReadResult = std::variant<std::string, nlohmann::json>;

    enum class FileLoadError
    {
        FileNotFound,
        FileCouldNotLoad
    };

    enum class DialogType
    {
        Save,
        Open
    };

    class File
    {
    public:
        File(std::string path)
            : m_path(path)
        {}

        std::expected<ReadResult,FileLoadError> Read(FileLoadType type);
        void Write(std::string data);
        std::string PromptFileDialog(DialogType type,std::vector<nfdu8filteritem_t> filters);
    private:
        std::string m_path;
    };
} // namespace LANE