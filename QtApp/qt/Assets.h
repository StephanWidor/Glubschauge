#pragma once
#include <filesystem>

namespace qt {

class Assets
{
public:
    static std::filesystem::path provideCascadeData();

    static std::filesystem::path provideFacemarkData();
};

}    // namespace qt
