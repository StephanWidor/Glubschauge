#pragma once
#include <filesystem>

namespace qt {

class FileSystem
{
public:
    static void moveToUserChoiceDir(const std::filesystem::path &);

    static std::filesystem::path appDataDir();

    static std::filesystem::path picturesDir();

    static std::filesystem::path glubschConfigPath();
};

}    // namespace qt
