#include "qt/VideoChooser.h"
#include "FileSystem.h"

#include <QFileDialog>
#include <QStandardPaths>
#ifdef ANDROID
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QtAndroid>
#endif

QString qt::VideoChooser::chooseVideo()
{
    FileSystem::requestPermission(FileSystem::AccessType::Read);
    const auto openFileName = QFileDialog::getOpenFileName(
      nullptr, "Play Video", QStandardPaths::writableLocation(QStandardPaths::MoviesLocation),
      "Videos (*.3gp *.avi *.m4v *.mov *.mp4 *.mpg  *.ogg *.vob *.wmv)");
#ifdef ANDROID
    // This is a workaround to get the local file path from Android ContentUri (qml MediaPlayer won't open uri).
    // Will only work if we stay in QStandardPaths::MoviesLocation!
    // TODO: fix this
    const auto fileName = contentUriInfo(openFileName, "DISPLAY_NAME");
    return "file://" + QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + "/" + fileName;
#else
    return "file://" + openFileName;
#endif
}

// taken from https://github.com/stephenquan/QtFileDialogApp.git
#ifdef ANDROID
QString qt::VideoChooser::contentUriInfo(const QString &url, const QString &columnName)
{
    QAndroidJniEnvironment env;
    QAndroidJniObject urlString = QAndroidJniObject::fromString(url);
    QAndroidJniObject uri = QAndroidJniObject::callStaticObjectMethod(
      "android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;", urlString.object<jstring>());
    if (!uri.isValid())
    {
        env->ExceptionClear();
        return "";
    }
    QAndroidJniObject context = QtAndroid::androidContext();
    if (!context.isValid())
    {
        env->ExceptionClear();
        return "";
    }
    QAndroidJniObject contentResolver =
      context.callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");
    if (!contentResolver.isValid())
    {
        env->ExceptionClear();
        return "";
    }
    QAndroidJniObject _columnName = QAndroidJniObject::getStaticObjectField<jstring>(
      "android/provider/MediaStore$MediaColumns", columnName.toUtf8().data());
    if (!_columnName.isValid())
    {
        env->ExceptionClear();
        return "";
    }
    jobjectArray stringArray = env->NewObjectArray(1, env->FindClass("java/lang/String"), nullptr);
    env->SetObjectArrayElement(stringArray, 0, _columnName.object<jstring>());
    QAndroidJniObject cursor = contentResolver.callObjectMethod(
      "query",
      "(Landroid/net/Uri;[Ljava/lang/String;Landroid/os/Bundle;Landroid/os/CancellationSignal;)Landroid/database/Cursor;",
      uri.object(), stringArray, nullptr, nullptr);
    if (!cursor.isValid())
    {
        env->ExceptionClear();
        return "";
    }
    jboolean ok = cursor.callMethod<jboolean>("moveToFirst");
    if (!ok)
    {
        env->ExceptionClear();
        return "";
    }
    QAndroidJniObject str = cursor.callObjectMethod("getString", "(I)Ljava/lang/String;", 0);
    if (!ok)
    {
        env->ExceptionClear();
        return "";
    }
    return str.toString();
}
#endif
