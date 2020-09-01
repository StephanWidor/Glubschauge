#pragma once
#include <QString>
#include <string>

namespace qt {

class Assets
{
public:
    static std::string provideResource(const std::string &file);

    static std::string provideCascadeData();

    static std::string provideFacemarkData();
};

}    // namespace qt
