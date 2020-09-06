#include "qt/Assets.h"
#include "FileSystem.h"
#include "Logger.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

std::string qt::Assets::provideResource(const std::string &file)
{
    QString appDataPath = QString::fromStdString(FileSystem::provideAppDataDir());
    QString qFile = QString::fromStdString(file);
    QString cascadeDataPath = appDataPath + qFile;
    if (QFile::exists(cascadeDataPath) || QFile::copy(":/" + qFile, cascadeDataPath))
        return cascadeDataPath.toStdString();
    Logger::debug() << ("Couldn't copy " + file);
    return "";
}

std::string qt::Assets::provideCascadeData()
{
    return provideResource("haarcascade_frontalface_alt2.xml");
}

std::string qt::Assets::provideFacemarkData()
{
    return provideResource("lbfmodel.yaml");
}
