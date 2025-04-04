#include "qt/Assets.h"
#include "qt/FileSystem.h"
#include <QDir>
#include <QFile>
#include <logger.h>

namespace qt {

namespace {

std::filesystem::path provideCopiedResource(const std::string &resourceFileName)
{
    const auto copiedResourcePath = appDataDir() / resourceFileName;
    if (std::filesystem::exists(copiedResourcePath) ||
        QFile::copy(":/" + QString::fromStdString(resourceFileName), QString(copiedResourcePath.native().c_str())))
    {
        return copiedResourcePath;
    }
    logger::out << std::format("Couldn't copy {} to app data folder", resourceFileName);
    return "";
}

}    // namespace

std::filesystem::path provideCascadeData()
{
    return provideCopiedResource("haarcascade_frontalface_default.xml");
}

std::filesystem::path provideFacemarkData()
{
    return provideCopiedResource("lbfmodel.yaml");
}

}    // namespace qt
