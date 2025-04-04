#pragma once
#include <filesystem>

namespace qt {

void moveToUserChoiceDir(const std::filesystem::path &);

std::filesystem::path appDataDir();

std::filesystem::path picturesDir();

std::filesystem::path glubschConfigPath();

}    // namespace qt
