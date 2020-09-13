#pragma once
#include <string>

namespace qt {

class FileSystem
{
public:
    enum class AccessType
    {
        Read,
        Write
    };

    static bool requestPermission(AccessType);

    static std::string provideAppDataDir();

    static std::string providePicturesDir();

    static std::string generatePathForNewPicture(const std::string &fileEnding = "jpg");
};

}    // namespace qt
