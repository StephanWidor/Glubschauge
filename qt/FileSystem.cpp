#include "qt/FileSystem.h"
#include "Logger.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#ifdef ANDROID
#include <QAndroidJniEnvironment>
#include <QUrl>
#include <QtAndroid>
#endif

#include <chrono>
#include <iomanip>
#include <sstream>

bool qt::FileSystem::requestPermission([[maybe_unused]] AccessType type)
{
#ifdef ANDROID
    QString permissionString = type == AccessType::Read ? "android.permission.READ_EXTERNAL_STORAGE" :
                                                          "android.permission.WRITE_EXTERNAL_STORAGE";
    return QtAndroid::checkPermission(permissionString) == QtAndroid::PermissionResult::Granted ||
           QtAndroid::requestPermissionsSync({permissionString}).value(permissionString) ==
             QtAndroid::PermissionResult::Granted;
#endif
    return true;
}

void qt::FileSystem::triggerMediaScan([[maybe_unused]] const std::string &filePath)
{
#ifdef ANDROID
    QAndroidJniEnvironment env;
    QAndroidJniObject context =
      QtAndroid::androidActivity().callObjectMethod("getApplicationContext", "()Landroid/content/Context;");
    QAndroidJniObject action = QAndroidJniObject::fromString("android.intent.action.MEDIA_SCANNER_SCAN_FILE");
    QAndroidJniObject path =
      QAndroidJniObject::fromString(QUrl::fromLocalFile(QString::fromStdString(filePath)).toString());
    QAndroidJniObject uri = QAndroidJniObject::callStaticObjectMethod(
      "android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;", path.object<jstring>());
    QAndroidJniObject intent("android/content/Intent");
    intent = intent.callObjectMethod("setData", "(Landroid/net/Uri;)Landroid/content/Intent;", uri.object());
    intent = intent.callObjectMethod("setAction", "(Ljava/lang/String;)Landroid/content/Intent;", action.object());
    context.callMethod<void>("sendBroadcast", "(Landroid/content/Intent;)V", intent.object());
    if (env->ExceptionCheck())
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
#endif
}

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
