#include "qt/FileSystem.h"
#include "Logger.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

#include <chrono>
#include <iomanip>
#include <sstream>

std::string qt::FileSystem::provideAppDataDir()
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/";
    if (QDir appDataDir(appDataPath); appDataDir.exists() || appDataDir.mkdir(appDataPath))
        return appDataPath.toStdString();
    Logger::debug() << "couldn't create appDataPath";
    return "";
}

std::string qt::FileSystem::providePicturesDir()
{
    QString picturesPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/Glubschauge/";
    if (QDir picturesDir(picturesPath); picturesDir.exists() || picturesDir.mkdir(picturesPath))
        return picturesPath.toStdString();
    Logger::debug() << "couldn't create picturesPath";
    return "";
}

std::string qt::FileSystem::generatePathForNewPicture(const std::string &fileEnding)
{
    const auto t = std::time(nullptr);
    const auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
    return providePicturesDir() + oss.str() + "." + fileEnding;
}
