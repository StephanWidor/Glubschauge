#include "qt/Assets.h"
#include "qt/FileSystem.h"
#include <QDir>
#include <QFile>
#include <logger.h>

namespace {

std::filesystem::path provideCopiedResource(const std::string &resourceFileName)
{
    const auto copiedResourcePath = qt::FileSystem::appDataDir() / resourceFileName;
    if (std::filesystem::exists(copiedResourcePath) ||
        QFile::copy(":/" + QString::fromStdString(resourceFileName), QString::fromStdString(copiedResourcePath)))
    {
        return copiedResourcePath;
    }
    logger::out << std::format("Couldn't copy {} to app data folder", resourceFileName);
    return "";
}

}    // namespace

std::filesystem::path qt::Assets::provideCascadeData()
{
    return provideCopiedResource("haarcascade_frontalface_default.xml");
}

std::filesystem::path qt::Assets::provideFacemarkData()
{
    return provideCopiedResource("lbfmodel.yaml");
}
