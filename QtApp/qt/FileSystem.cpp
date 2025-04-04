#include "qt/FileSystem.h"
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
#include <format>
#include <logger.h>

void qt::FileSystem::moveToUserChoiceDir(const std::filesystem::path &path)
{
    QFileDialog fileDialog(nullptr, "Save Captured Image",
                           QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                           QString(std::format("Images (*{})", path.extension().string()).c_str()));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setViewMode(QFileDialog::List);
    fileDialog.selectFile(QString(path.filename().c_str()));
    if (fileDialog.exec())
    {
        const auto selectedFiles = fileDialog.selectedFiles();
        if (!selectedFiles.empty() && !selectedFiles.front().isEmpty())
        {
            const auto &newPath = selectedFiles.front();
            if (newPath.toStdString() == path.string())
                return;

            QFile qFile(QString(path.native().c_str()));
            qFile.open(QIODevice::ReadWrite);
            if (!qFile.rename(newPath))
                logger::out << std::format("failed saving capture to {}", newPath.toStdString());
            qFile.close();
        }
    }

    std::filesystem::remove(path);
}

std::filesystem::path qt::FileSystem::appDataDir()
{
    auto dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toStdString();
    if (!std::filesystem::exists(dir))
        std::filesystem::create_directories(dir);
    return dir;
}

std::filesystem::path qt::FileSystem::picturesDir()
{
#ifdef ANDROID
    return QStandardPaths::writableLocation(QStandardPaths::PicturesLocation).toStdString();
#else
    auto dir = std::filesystem::path(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation).toStdString()) /
               "Glubschauge";
    if (!std::filesystem::exists(dir))
        std::filesystem::create_directories(dir);
    return dir;
#endif
}

std::filesystem::path qt::FileSystem::glubschConfigPath()
{
    return appDataDir() / "GlubschConfig.yaml";
}
